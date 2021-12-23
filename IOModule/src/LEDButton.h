#pragma once
#include "Button.h"
#include "LED.h"

class LEDButton : public Button
{
private:

	LED led;

public:

	LEDButton();
	virtual ~LEDButton();

	void Init(int buttonpin, int ledpin);
	void SetLED(PinStatus state);
};
