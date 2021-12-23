#pragma once
#include "Arduino.h"

class PollMonitor
{
private:

	// The pins to monitor
	int pin;

	// Last polled state
	PinStatus laststate;

public:

	PollMonitor();
	virtual ~PollMonitor();

	// Initialization
	void Init(int sensorpin);

	// To be polled for result
	bool HasChanged();
	PinStatus GetState() { return laststate; }
};
