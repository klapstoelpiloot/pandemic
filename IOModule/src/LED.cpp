#include "LED.h"

// Constructor
LED::LED() :
	led(0),
	state(false)
{
}

// Destructor
LED::~LED()
{
}

void LED::Init(int ledpin)
{
	led = ledpin;
	state = false;
	pinMode(led, OUTPUT);
	digitalWrite(led, LOW);
}

void LED::Set(PinStatus state)
{
	digitalWrite(led, state);
	this->state = (state == HIGH);
}
