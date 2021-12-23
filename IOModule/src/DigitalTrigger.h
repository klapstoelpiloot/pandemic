#pragma once
#include "Arduino.h"

// The maximum number of of these digital triggers we will ever use in one program
#define MAX_DIGITALTRIGGERS 4

class DigitalTrigger
{
private:
	
	// DigitalTrigger index for static registration
	int index;

	// The pins assigned to this sensor
	int pin;
	PinStatus mode;

	// Indicates the time the sensor was triggered, in milliseconds.
	// This value is 0 when the sensor was not (yet) triggered or Reset() was called.
	volatile unsigned long triggertime;

	// Indicates the state (LOW or HIGH) after the sensor was triggered.
	volatile PinStatus triggerstate;
	
public:
	
	DigitalTrigger();
	virtual ~DigitalTrigger();
	
	// Initialization/termination
	// Mode can be CHANGE, FALLING or RISING
	void Init(int sensorpin, PinStatus triggermode);
	void Term();

	// To be called by the interrupt handlers only
	void Trigger();
	
	// To be polled for result
	bool IsTriggered() { return triggertime > 0; }
	unsigned long GetTime() { return triggertime; }
	PinStatus GetState() { return triggerstate; }

	// Resets all counters
	void Reset() { triggertime = 0; }
};

// This is for registration of object instances so that the interrupt handler can find them.
extern bool DigitalTrigger_staticinit;
extern DigitalTrigger* DigitalTrigger_instances[MAX_DIGITALTRIGGERS];
extern void(*DigitalTrigger_handlers[MAX_DIGITALTRIGGERS])();

// The real interrupt handlers
void DigitalTrigger_InterruptHandler0();
void DigitalTrigger_InterruptHandler1();
void DigitalTrigger_InterruptHandler2();
void DigitalTrigger_InterruptHandler3();
