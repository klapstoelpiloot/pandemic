#include <string.h>
#include "SerialLink.h"
#include "CRC32.h"
#include "IOMode.h"
#include "pb_common.h"
#include "pb_encode.h"
#include "pb_decode.h"

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

SerialLink::SerialLink(IOMode* mode) :
	mode(mode),
	rxlength(0),
	lastkeepalive(0)
{
}

// Returns True when the message was sent
bool SerialLink::SendMessage(const IOModule_IOMessage& msg)
{
	uint8_t txbuffer[SERIAL_BUFFER_SIZE] = { STARTBYTE1, STARTBYTE2, 0 };

	// Serialize the data
	uint8_t* payloaddata = txbuffer + NUM_START_BYTES + NUM_LENGTH_BYTES;
	pb_ostream_t stream = pb_ostream_from_buffer(payloaddata, SERIAL_BUFFER_SIZE - MINIMUM_MESSAGE_LENGTH);
	if(!pb_encode(&stream, IOModule_IOMessage_fields, &msg))
		return false;

	// Add the length
	size_t payloadlength = stream.bytes_written;
	size_t totalsize = NUM_START_BYTES + NUM_LENGTH_BYTES + payloadlength;
	txbuffer[2] = static_cast<uint8_t>(totalsize & 0x000000FF);

	// Check if the buffer has space to send this data
	if(Serial.availableForWrite() < totalsize)
		return false;

	// Send the data
	Serial.write(txbuffer, totalsize);
	return true;
}

// Returns True when a message was received
bool SerialLink::Update()
{
	bool newmessage = false;

	// Receive data
	int bytesready = Serial.available();
	if(bytesready > 0)
	{
		int readbytes = min(bytesready, SERIAL_BUFFER_SIZE - rxlength);
		rxlength += Serial.readBytes(rxbuffer + rxlength, readbytes);
		lastkeepalive = millis();
	}

	// Make sure the first bytes are the start bytes and discard the data if invalid
	if((rxlength >= 1) && (rxbuffer[0] != STARTBYTE1))
	{
		rxlength = 0;
	}
	else if((rxlength >= 2) && (rxbuffer[1] != STARTBYTE2))
	{
		rxlength = 0;
	}
	// Check if the length of the message can be determined
	else if(rxlength >= MINIMUM_MESSAGE_LENGTH)
	{
		// Determine the message length
		size_t msglen = static_cast<size_t>(rxbuffer[2]);
		if((msglen < MINIMUM_MESSAGE_LENGTH) || (msglen > SERIAL_BUFFER_SIZE))
		{
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
			const uint8_t* payload = rxbuffer + NUM_START_BYTES + NUM_LENGTH_BYTES;
			size_t payloadlen = msglen - (NUM_START_BYTES + NUM_LENGTH_BYTES);
			if(payloadlen > 0)
			{
				// Deserialize the message
				pb_istream_t stream = pb_istream_from_buffer(payload, payloadlen);
				newmessage = pb_decode(&stream, IOModule_PiMessage_fields, &rxmsg);
			}

			// Move remaining data to the start of the buffer.
			if(rxlength > msglen)
				memmove(rxbuffer, rxbuffer + msglen, rxlength - msglen);
			rxlength -= msglen;
		}
	}

	return newmessage;
}

bool SerialLink::SendStartSlide(uint32_t speed)
{
	IOModule_IOMessage msg;
	msg.which_Content = IOModule_IOMessage_StartSlide_tag;
	msg.Content.StartSlide.Speed = speed;
	return SendMessage(msg);
}

bool SerialLink::SendRemovePuck()
{
	IOModule_IOMessage msg;
	msg.which_Content = IOModule_IOMessage_RemovePuck_tag;
	return SendMessage(msg);
}

bool SerialLink::SendSensorBlocked()
{
	IOModule_IOMessage msg;
	msg.which_Content = IOModule_IOMessage_SensorBlocked_tag;
	return SendMessage(msg);
}

bool SerialLink::SendSensorCleared()
{
	IOModule_IOMessage msg;
	msg.which_Content = IOModule_IOMessage_SensorCleared_tag;
	return SendMessage(msg);
}

bool SerialLink::SendLeftButtonPress()
{
	IOModule_IOMessage msg;
	msg.which_Content = IOModule_IOMessage_LeftButtonPressed_tag;
	return SendMessage(msg);
}

bool SerialLink::SendRightButtonPress()
{
	IOModule_IOMessage msg;
	msg.which_Content = IOModule_IOMessage_RightButtonPressed_tag;
	return SendMessage(msg);
}

bool SerialLink::SendAcceptButtonPress()
{
	IOModule_IOMessage msg;
	msg.which_Content = IOModule_IOMessage_AcceptButtonPressed_tag;
	return SendMessage(msg);
}

bool SerialLink::SendCancelButtonPress()
{
	IOModule_IOMessage msg;
	msg.which_Content = IOModule_IOMessage_CancelButtonPressed_tag;
	return SendMessage(msg);
}

bool SerialLink::SendGateScore(uint32_t gateindex)
{
	IOModule_IOMessage msg;
	msg.which_Content = IOModule_IOMessage_GateScore_tag;
	msg.Content.GateScore.Gate = gateindex;
	return SendMessage(msg);
}

bool SerialLink::SendPuckInGate(uint32_t gateindex)
{
	IOModule_IOMessage msg;
	msg.which_Content = IOModule_IOMessage_PuckInGate_tag;
	msg.Content.PuckInGate.Gate = gateindex;
	return SendMessage(msg);
}

bool SerialLink::SendSensorState(uint32_t sensorindex, bool ishigh)
{
	IOModule_IOMessage msg;
	msg.which_Content = IOModule_IOMessage_SensorState_tag;
	msg.Content.SensorState.Sensor = static_cast<IOModule_Sensor>(sensorindex);
	msg.Content.SensorState.High = ishigh;
	return SendMessage(msg);
}
