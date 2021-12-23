#include "PollMonitor.h"

PollMonitor::PollMonitor()
{
	pin = 0;
	laststate = LOW;
}

PollMonitor::~PollMonitor()
{
}

void PollMonitor::Init(int sensorpin)
{
	pin = sensorpin;
	pinMode(pin, PinMode::INPUT_PULLUP);
}

bool PollMonitor::HasChanged()
{
	PinStatus newstate = digitalRead(pin);
	if(newstate != laststate)
	{
		laststate = newstate;
		return true;
	}
	else
	{
		return false;
	}
}
