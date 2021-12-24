#pragma once
#include <thread>
#include <queue>
#include "Configuration.h"
#include "Tools.h"
#include "IOModule.pb.h"

#define SERIAL_BUFFER_SIZE	256

class IOModuleLink final
{
private:

	// Configuration
	const Configuration& config;

	// The serial port handle
	int streamhandle;

	// Receiving
	std::thread receivethread;
	volatile bool stopthread;
	byte rxbuffer[SERIAL_BUFFER_SIZE];
	size_t rxlength;
	std::queue<IOModule_IOMessage> receivedmessages;
	std::mutex receivedmutex;
	void ReceiveThread();

	// Sending
	bool SendMessage(const IOModule_PiMessage& msg);
	bool SendKeepAlive();

	// Keep alive timing
	TimePoint lastkeepalive;

public:

	// Constructor/destructor
	IOModuleLink(const Configuration& cfg);
	~IOModuleLink();

	// This returns a message when one was received.
	// Returns False when no message was received.
	bool ReceiveMessage(IOModule_IOMessage& msg);

	// Sending
	void SetTestMode();
	void SetNormalMode();
	void SendSettings(bool hardmode);
	void SendButtonRepeat(bool enablerepeat);
	void SendRoundReset();
	void SetButtonLights(bool left, bool right, bool accept, bool cancel);
};
