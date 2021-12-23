#pragma once
#include "Arduino.h"

// The maximum number of watchers we will ever use in one program
#define MAX_WATCHERS	4

class GateWatcher
{
private:

	// GateWatcher index for static registration
	int index;
	
	// True to count according to official (pro) rules: Puck must be fully inside the gate to score
	// False to count according to casual rules: Puck may be half inside the gate to score
	bool official;

	// The pins assigned to this counter
	int frontsensorpin;
	int backsensorpin;
	
	// Indicates if a new puck has arrived at the point of no return (back sensor).
	volatile bool newpuck;
	
	// Detection counters
	volatile int puckcount;
	
	// Respond to triggers?
	volatile bool enabled;
	
public:
	
	GateWatcher();
	virtual ~GateWatcher();

	// Initialization/termination
	void Init(int frontsensor, int backsensor, bool officialrules);
	void Term();
	
	// To be called by the interrupt handlers only
	void FrontSensorTrigger();
	void BackSensorTrigger();
	
	// To be polled for result.
	// When a result is detected, it is dequeued.
	bool HasPuckPassed();
	
	// Resets all counters
	void Reset();

	// Enable/disable triggers
	void Enable() { enabled = true; }
	void Disable() { enabled = false; }
};

// This is for registration of object instances so that the interrupt handler can find them.
extern bool GateWatcher_staticinit;
extern GateWatcher* GateWatcher_instances[MAX_WATCHERS];
extern void(*GateWatcher_frontsensorhandlers[MAX_WATCHERS])();
extern void(*GateWatcher_backsensorhandlers[MAX_WATCHERS])();

// The real interrupt handlers
void GateWatcher_FrontSensorInterruptHandler0();
void GateWatcher_FrontSensorInterruptHandler1();
void GateWatcher_FrontSensorInterruptHandler2();
void GateWatcher_FrontSensorInterruptHandler3();
void GateWatcher_BackSensorInterruptHandler0();
void GateWatcher_BackSensorInterruptHandler1();
void GateWatcher_BackSensorInterruptHandler2();
void GateWatcher_BackSensorInterruptHandler3();
