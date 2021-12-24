#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <csignal>
#include "IOModuleLink.h"
#include "Main.h"
#include "pb_common.h"
#include "pb_encode.h"
#include "pb_decode.h"
#include "CRC32.h"

///
/// Communication between IO Module and the Pi uses the nanopb Protobuf implementation.
/// The protobuf messages are sent as follows:
///
///    2 bytes  1 byte     # bytes    
///   |_______|_______|______________|
///       1       2          3
///
/// 1: Bytes 0xBA and 0xBE indicate the start of the message.
/// 2: Total size of the message in bytes. This includes the start bytes and this length byte.
/// 3: Protobuf message data.
/// 
/// Since we're communicating over a USB connection (which has a guaranteed transfer protocol) and the chances
/// of losing data are neglectable, the code below does not implement a system to guarantee correct arrival.
///

#define NUM_START_BYTES				2
#define NUM_LENGTH_BYTES			1
#define MINIMUM_MESSAGE_LENGTH		(NUM_START_BYTES + NUM_LENGTH_BYTES)
#define STARTBYTE1					0xBA
#define STARTBYTE2					0xBE
#define KEEP_ALIVE_INTERVAL			1000

IOModuleLink::IOModuleLink(const Configuration& cfg) :
	config(cfg),
	streamhandle(0),
	stopthread(false),
	rxlength(0)
{
	CRC32::Initialize();

	// Open serial port
	String portname = cfg.GetString("IO.SerialPort", "");
	std::cout << "Initializing IO module connection at " << portname << "..." << std::endl;
	streamhandle = open(portname, O_RDWR | O_NOCTTY | O_NONBLOCK);
	ENSURE(streamhandle >= 0);
	if(streamhandle < 0)
		std::cout << "ERROR: Unable to open serial port." << std::endl;

	// Configure port
	termios tty;
	memset(&tty, 0, sizeof(tty));
	int result = tcgetattr(streamhandle, &tty);
	ENSURE(result == 0);
	cfsetospeed(&tty, (speed_t)B9600);
	cfsetispeed(&tty, (speed_t)B9600);
	tty.c_cflag &= ~PARENB;			// Make 8n1
	tty.c_cflag &= ~CSTOPB;
	tty.c_cflag &= ~CSIZE;
	tty.c_cflag |= CS8;
	tty.c_cflag &= ~CRTSCTS;		// no flow control
	tty.c_cc[VMIN] = 1;				// read doesn't block
	tty.c_cc[VTIME] = 5;			// 0.5 seconds read timeout
	tty.c_cflag |= CREAD | CLOCAL;	// turn on READ & ignore ctrl lines
	cfmakeraw(&tty);
	tcflush(streamhandle, TCIFLUSH);
	result = tcsetattr(streamhandle, TCSANOW, &tty);
	ENSURE(result == 0);

	// Wait for the port to initialize
	std::this_thread::sleep_for(std::chrono::milliseconds(500));

	// Start receiving thread
	receivethread = std::thread(&IOModuleLink::ReceiveThread, this);
	pthread_setname_np(receivethread.native_handle(), "IOModuleLink");

	// Send settings and set the firmware in normal state
	SendSettings(false);
	SetNormalMode();
	SetButtonLights(false, false, false, false);
}

IOModuleLink::~IOModuleLink()
{
	// Shutdown receive thread
	if(receivethread.joinable())
	{
		stopthread = true;
		receivethread.join();
	}

	// Clean up
	close(streamhandle);
	streamhandle = 0;
}

void IOModuleLink::ReceiveThread()
{
	while(!stopthread)
	{
		// Read characters from the serial port
		ssize_t bytesread = -1;
		if(rxlength < SERIAL_BUFFER_SIZE)
			bytesread = read(streamhandle, rxbuffer + rxlength, SERIAL_BUFFER_SIZE - rxlength);
		if(bytesread > 0)
			rxlength += bytesread;

		// Make sure the first bytes are the start bytes and discard the data if invalid
		if((rxlength >= 1) && (rxbuffer[0] != STARTBYTE1))
		{
			std::cout << "Message trashed: First byte is not a start byte." << std::endl;
			rxlength = 0;
		}
		else if((rxlength >= 2) && (rxbuffer[1] != STARTBYTE2))
		{
			std::cout << "Message trashed: Second byte is not a start byte." << std::endl;
			rxlength = 0;
		}
		// Check if the length of the message can be determined
		else if(rxlength >= (NUM_START_BYTES + NUM_LENGTH_BYTES))
		{
			// Determine the message length
			size_t msglen = static_cast<size_t>(rxbuffer[2]);
			if((msglen < MINIMUM_MESSAGE_LENGTH) || (msglen > SERIAL_BUFFER_SIZE))
			{
				std::cout << "Message trashed: Length is invalid." << std::endl;

				// Invalid message size. Discard until the next start bytes.
				uint32_t newstart = 0;
				for(uint32_t i = 2; i < (rxlength - 1); i++)
				{
					if((rxbuffer[i] == STARTBYTE1) && (rxbuffer[i + 1] == STARTBYTE2))
					{
						newstart = i;
						break;
					}
				}

				if(newstart > 0)
				{
					// Start found. Move remaining data to the start of the buffer.
					memmove(rxbuffer, rxbuffer + newstart, rxlength - newstart);
					rxlength -= newstart;
				}
				else
				{
					rxlength = 0;
				}
			}

			if(rxlength >= msglen)
			{
				// Enough data received for a complete message
				const byte* payload = rxbuffer + NUM_START_BYTES + NUM_LENGTH_BYTES;
				size_t payloadlen = msglen - MINIMUM_MESSAGE_LENGTH;
				if(payloadlen > 0)
				{
					// Deserialize the message
					IOModule_IOMessage msg;
					pb_istream_t stream = pb_istream_from_buffer(payload, payloadlen);
					bool result = pb_decode(&stream, IOModule_IOMessage_fields, &msg);
					if(result)
					{
						// We have received a message!
						lock_guard<mutex> lock(receivedmutex);
						receivedmessages.push(msg);
					}
				}

				// Move remaining data to the start of the buffer.
				if(rxlength > msglen)
					memmove(rxbuffer, rxbuffer + msglen, rxlength - msglen);
				rxlength -= msglen;
			}
		}

		// Send keep alive signals
		if(ToMilliseconds(Clock::now()) > (ToMilliseconds(lastkeepalive) + KEEP_ALIVE_INTERVAL))
		{
			SendKeepAlive();
			lastkeepalive = Clock::now();
		}

		// Relax
		std::this_thread::sleep_for(ch::milliseconds(10));
	}
}

bool IOModuleLink::ReceiveMessage(IOModule_IOMessage& msg)
{
	lock_guard<mutex> lock(receivedmutex);

	// Nothing in the buffer?
	if(receivedmessages.empty())
		return false;
	
	// Return next message
	msg = receivedmessages.front();
	receivedmessages.pop();

	return true;
}

bool IOModuleLink::SendMessage(const IOModule_PiMessage& msg)
{
	byte txbuffer[SERIAL_BUFFER_SIZE] = { STARTBYTE1, STARTBYTE2, 0 };

	// Serialize the data
	byte* payloaddata = txbuffer + NUM_START_BYTES + NUM_LENGTH_BYTES;
	pb_ostream_t stream = pb_ostream_from_buffer(payloaddata, SERIAL_BUFFER_SIZE - MINIMUM_MESSAGE_LENGTH);
	if(!pb_encode(&stream, IOModule_PiMessage_fields, &msg))
		return false;

	// Add the length
	size_t totalsize = NUM_START_BYTES + NUM_LENGTH_BYTES + stream.bytes_written;
	txbuffer[2] = checked_cast<byte>(totalsize & 0x000000FF);

	// Send the data
	ssize_t byteswritten = write(streamhandle, txbuffer, totalsize);
	return (byteswritten == (ssize_t)totalsize);
}

bool IOModuleLink::SendKeepAlive()
{
	char txbuffer[MINIMUM_MESSAGE_LENGTH] = { STARTBYTE1, STARTBYTE2, MINIMUM_MESSAGE_LENGTH };

	// Send the data
	ssize_t byteswritten = write(streamhandle, txbuffer, MINIMUM_MESSAGE_LENGTH);
	return (byteswritten == MINIMUM_MESSAGE_LENGTH);
}

void IOModuleLink::SetTestMode()
{
	IOModule_PiMessage msg;
	msg.which_Content = IOModule_PiMessage_CalibrateMode_tag;
	SendMessage(msg);
}

void IOModuleLink::SetNormalMode()
{
	IOModule_PiMessage msg;
	msg.which_Content = IOModule_PiMessage_NormalMode_tag;
	SendMessage(msg);
}

void IOModuleLink::SendSettings(bool hardmode)
{
	IOModule_PiMessage msg;
	msg.which_Content = IOModule_PiMessage_Settings_tag;
	msg.Content.Settings.SlideTimeoutMs = checked_cast<uint>(config.GetInt("Game.SlideTimeoutMs", 2000));
	msg.Content.Settings.PuckLostTimeout = checked_cast<uint>(config.GetInt("Game.PuckLostCheckTimeoutMs", 500));
	msg.Content.Settings.DebounceTimeout = checked_cast<uint>(config.GetInt("IO.ButtonDebounceTimeout", 50));
	msg.Content.Settings.ButtonRepeatTimeout = checked_cast<uint>(config.GetInt("IO.ButtonRepeatTimeout", 500));
	msg.Content.Settings.ButtonRepeatInterval = checked_cast<uint>(config.GetInt("IO.ButtonRepeatInterval", 100));
	msg.Content.Settings.HardMode = hardmode;
	SendMessage(msg);
}

void IOModuleLink::SendButtonRepeat(bool enablerepeat)
{
	IOModule_PiMessage msg;
	msg.which_Content = IOModule_PiMessage_SetButtonRepeat_tag;
	msg.Content.SetButtonRepeat.Enable = enablerepeat;
	SendMessage(msg);
}

void IOModuleLink::SendRoundReset()
{
	IOModule_PiMessage msg;
	msg.which_Content = IOModule_PiMessage_RoundReset_tag;
	SendMessage(msg);
}

void IOModuleLink::SetButtonLights(bool left, bool right, bool accept, bool cancel)
{
	IOModule_PiMessage msg;
	msg.which_Content = IOModule_PiMessage_SetButtonLights_tag;
	msg.Content.SetButtonLights.LeftButton = left;
	msg.Content.SetButtonLights.RightButton = right;
	msg.Content.SetButtonLights.AcceptButton = accept;
	msg.Content.SetButtonLights.CancelButton = cancel;
	SendMessage(msg);
}
