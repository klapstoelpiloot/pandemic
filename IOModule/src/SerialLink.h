#pragma once
#include "Arduino.h"
#include "Defines.h"
#include "IOModule.pb.h"

class IOMode;

class SerialLink
{
private:

	IOMode* mode;

	// Buffer for receiving
	uint8_t rxbuffer[SERIAL_BUFFER_SIZE];
	uint32_t rxlength;
	IOModule_PiMessage rxmsg;

	// Keep alive checking
	unsigned long lastkeepalive;

	// Methods
	bool SendMessage(const IOModule_IOMessage& msg);

public:

	// Constructor
	SerialLink(IOMode* mode);

	// Methods
	bool Update();

	// Receiving
	bool IsAlive() const { return (millis() < (lastkeepalive + KEEP_ALIVE_TIMEOUT)); }
	const IOModule_PiMessage& GetMessage() const { return rxmsg; }

	// Sending
	bool SendStartSlide(uint32_t speed);
	bool SendRemovePuck();
	bool SendSensorBlocked();
	bool SendSensorCleared();
	bool SendLeftButtonPress();
	bool SendRightButtonPress();
	bool SendAcceptButtonPress();
	bool SendCancelButtonPress();
	bool SendGateScore(uint32_t gateindex);
	bool SendPuckInGate(uint32_t gateindex);
	bool SendSensorState(uint32_t sensorindex, bool ishigh);
};
