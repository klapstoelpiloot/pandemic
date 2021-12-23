#include "GateWatcher.h"

// This is for registration of object instances so that the interrupt handler can find them.
bool GateWatcher_staticinit = false;
GateWatcher* GateWatcher_instances[MAX_WATCHERS] = { nullptr, nullptr, nullptr, nullptr };
void(*GateWatcher_frontsensorhandlers[MAX_WATCHERS])();
void(*GateWatcher_backsensorhandlers[MAX_WATCHERS])();

// Constructor
GateWatcher::GateWatcher() :
	index(-1),
	frontsensorpin(0),
	backsensorpin(0),
	newpuck(false),
	puckcount(0),
	enabled(false)

{
	// Static initialization
	if(!GateWatcher_staticinit)
	{
		GateWatcher_frontsensorhandlers[0] = GateWatcher_FrontSensorInterruptHandler0;
		GateWatcher_frontsensorhandlers[1] = GateWatcher_FrontSensorInterruptHandler1;
		GateWatcher_frontsensorhandlers[2] = GateWatcher_FrontSensorInterruptHandler2;
		GateWatcher_frontsensorhandlers[3] = GateWatcher_FrontSensorInterruptHandler3;
		GateWatcher_backsensorhandlers[0] = GateWatcher_BackSensorInterruptHandler0;
		GateWatcher_backsensorhandlers[1] = GateWatcher_BackSensorInterruptHandler1;
		GateWatcher_backsensorhandlers[2] = GateWatcher_BackSensorInterruptHandler2;
		GateWatcher_backsensorhandlers[3] = GateWatcher_BackSensorInterruptHandler3;
		GateWatcher_staticinit = true;
	}
}

// Destructor
GateWatcher::~GateWatcher()
{
}

void GateWatcher::Init(int frontsensor, int backsensor, bool officialrules)
{
	official = officialrules;
	frontsensorpin = frontsensor;
	backsensorpin = backsensor;

	// Register in static array so that the interrupt handlers can find me
	for(int i = 0; i < MAX_WATCHERS; i++)
	{
		if(GateWatcher_instances[i] == nullptr)
		{
			index = i;
			GateWatcher_instances[i] = this;
			break;
		}
	}
	
	// Setup pins
	pinMode(frontsensorpin, PinMode::INPUT_PULLUP);
	pinMode(backsensorpin, PinMode::INPUT_PULLUP);
	if(officialrules)
	{
		attachInterrupt(digitalPinToInterrupt(frontsensorpin), GateWatcher_frontsensorhandlers[index], PinStatus::RISING);
		attachInterrupt(digitalPinToInterrupt(backsensorpin), GateWatcher_backsensorhandlers[index], PinStatus::FALLING);
	}
	else
	{
		attachInterrupt(digitalPinToInterrupt(frontsensorpin), GateWatcher_frontsensorhandlers[index], PinStatus::FALLING);
		attachInterrupt(digitalPinToInterrupt(backsensorpin), GateWatcher_backsensorhandlers[index], PinStatus::FALLING);
	}

	Reset();
}

void GateWatcher::Term()
{
	// Release interrupts
	if(frontsensorpin > 0)
		detachInterrupt(digitalPinToInterrupt(frontsensorpin));
	if(backsensorpin > 0)
		detachInterrupt(digitalPinToInterrupt(backsensorpin));

	// Free our spot in the static array
	if(index > -1)
		GateWatcher_instances[index] = nullptr;

	index = -1;
	frontsensorpin = 0;
	backsensorpin = 0;
}

bool GateWatcher::HasPuckPassed()
{
	if(puckcount > 0)
	{
		puckcount--;
		return true;
	}
	else
	{
		return false;
	}
}

void GateWatcher::Reset()
{
	puckcount = 0;
	newpuck = false;
}

void GateWatcher::FrontSensorTrigger()
{
	if(!enabled)
		return;

	if(official)
	{
		if(newpuck && (digitalRead(backsensorpin) == PinStatus::LOW))
		{
			newpuck = false;
			puckcount++;
		}
	}
	else
	{
		newpuck = true;
	}
}

void GateWatcher::BackSensorTrigger()
{
	if(!enabled)
		return;

	if(official)
	{
		newpuck = true;
	}
	else
	{
		if(newpuck && (digitalRead(frontsensorpin) == PinStatus::LOW))
		{
			newpuck = false;
			puckcount++;
		}
	}
}

// The real interrupt handlers
void GateWatcher_FrontSensorInterruptHandler0() { GateWatcher_instances[0]->FrontSensorTrigger(); }
void GateWatcher_FrontSensorInterruptHandler1() { GateWatcher_instances[1]->FrontSensorTrigger(); }
void GateWatcher_FrontSensorInterruptHandler2() { GateWatcher_instances[2]->FrontSensorTrigger(); }
void GateWatcher_FrontSensorInterruptHandler3() { GateWatcher_instances[3]->FrontSensorTrigger(); }
void GateWatcher_BackSensorInterruptHandler0() { GateWatcher_instances[0]->BackSensorTrigger(); }
void GateWatcher_BackSensorInterruptHandler1() { GateWatcher_instances[1]->BackSensorTrigger(); }
void GateWatcher_BackSensorInterruptHandler2() { GateWatcher_instances[2]->BackSensorTrigger(); }
void GateWatcher_BackSensorInterruptHandler3() { GateWatcher_instances[3]->BackSensorTrigger(); }
