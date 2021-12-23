#include "Button.h"

unsigned long Button::debouncetimeout = 50;
unsigned long Button::repeattimeout = 500;
unsigned long Button::repeatinterval = 100;
bool Button::allowrepeat = false;

// Constructor
Button::Button()
{
	starttime = 0;
	checktime = 0;
}

// Destructor
Button::~Button()
{
}

void Button::Init(int buttonpin)
{
	pin = buttonpin;
	pinMode(pin, INPUT_PULLUP);
}

bool Button::IsPressed()
{
	PinStatus pinstate = digitalRead(pin);

	if(starttime == 0)
	{
		// Check if the button is pressed after debounce
		if((pinstate == LOW) && (millis() > (releasetime + debouncetimeout)))
		{
			releasetime = 0;
			starttime = millis();
		}
	}
	else
	{
		// Check if the button is released after debounce
		if((pinstate == HIGH) && (millis() > (starttime + debouncetimeout)))
		{
			releasetime = millis();
			starttime = 0;
			checktime = 0;
		}
	}

	// Button pressed?
	if(starttime > 0)
	{
		unsigned long t = millis();

		// First time we check since this button was pressed?
		if(checktime == 0)
		{
			// Consider it pressed once
			checktime = t;
			return true;
		}

		// Repeat this when the button is held down long enough
		if(allowrepeat && (t > (starttime + repeattimeout)) && (t > (checktime + repeatinterval)))
		{
			checktime = t;
			return true;
		}
	}

	return false;
}

void Button::SetTiming(unsigned long debounce, unsigned long timeout, unsigned long interval)
{
	debouncetimeout = debounce;
	repeattimeout = timeout;
	repeatinterval = interval;
}

void Button::SetRepeat(bool enable)
{
	allowrepeat = enable;
}
