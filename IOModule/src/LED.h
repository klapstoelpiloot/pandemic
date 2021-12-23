#pragma once
#include "Arduino.h"

class LED
{
private:

	// The digital pin which powers the LED
	int led;
	bool state;

public:

	LED();
	virtual ~LED();

	void Init(int ledpin);
	void Set(PinStatus state);
	bool Get() { return state; }
};
