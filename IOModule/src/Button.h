#pragma once
#include "DigitalTrigger.h"

class Button
{
private:

	// Timings
	static unsigned long debouncetimeout;
	static unsigned long repeattimeout;
	static unsigned long repeatinterval;
	static bool allowrepeat;

	// The pin we are monitoring
	int pin;

	// Time at which the button was pushed first.
	// This value is 0 when the button is released.
	unsigned long starttime;
	unsigned long releasetime;

	// Last time we checked if the button was pressed.
	unsigned long checktime;

public:

	Button();
	virtual ~Button();

	static void SetTiming(unsigned long debounce, unsigned long timeout, unsigned long interval);
	static void SetRepeat(bool enable);

	void Init(int buttonpin);

	bool IsPressed();
};
