#include "DigitalTrigger.h"

// This is for registration of object instances so that the interrupt handler can find them.
bool DigitalTrigger_staticinit = false;
DigitalTrigger* DigitalTrigger_instances[MAX_DIGITALTRIGGERS] = { nullptr, nullptr, nullptr, nullptr };
void(*DigitalTrigger_handlers[MAX_DIGITALTRIGGERS])();

// Constructor
DigitalTrigger::DigitalTrigger() :
	index(-1),
	pin(0),
	mode(PinStatus::LOW),
	triggertime(0),
	triggerstate(PinStatus::LOW)
{
	// Static initialization
	if(!DigitalTrigger_staticinit)
	{
		DigitalTrigger_handlers[0] = DigitalTrigger_InterruptHandler0;
		DigitalTrigger_handlers[1] = DigitalTrigger_InterruptHandler1;
		DigitalTrigger_handlers[2] = DigitalTrigger_InterruptHandler2;
		DigitalTrigger_handlers[3] = DigitalTrigger_InterruptHandler3;
		DigitalTrigger_staticinit = true;
	}
}

// Destructor
DigitalTrigger::~DigitalTrigger()
{
}

void DigitalTrigger::Init(int sensorpin, PinStatus triggermode)
{
	pin = sensorpin;
	mode = triggermode;

	if(mode == PinStatus::FALLING)
		triggerstate = PinStatus::LOW;
	else
		triggerstate = PinStatus::HIGH;
	
	// Register in static array so that the interrupt handlers can find me
	for(int i = 0; i < MAX_DIGITALTRIGGERS; i++)
	{
		if(DigitalTrigger_instances[i] == nullptr)
		{
			index = i;
			DigitalTrigger_instances[i] = this;
			break;
		}
	}
	
	// Setup pins
	pinMode(pin, PinMode::INPUT_PULLUP);
	attachInterrupt(digitalPinToInterrupt(pin), DigitalTrigger_handlers[index], triggermode);

	triggertime = 0;
}

void DigitalTrigger::Term()
{
	// Release interrupt
	if(pin > 0)
		detachInterrupt(digitalPinToInterrupt(pin));

	// Free our spot in the static array
	if(index > -1)
		DigitalTrigger_instances[index] = nullptr;

	index = -1;
	pin = 0;
}

void DigitalTrigger::Trigger()
{
	triggertime = millis();
	if(mode == PinStatus::CHANGE)
		triggerstate = digitalRead(pin);
}

// The real interrupt handlers
void DigitalTrigger_InterruptHandler0() { DigitalTrigger_instances[0]->Trigger(); }
void DigitalTrigger_InterruptHandler1() { DigitalTrigger_instances[1]->Trigger(); }
void DigitalTrigger_InterruptHandler2() { DigitalTrigger_instances[2]->Trigger(); }
void DigitalTrigger_InterruptHandler3() { DigitalTrigger_instances[3]->Trigger(); }
