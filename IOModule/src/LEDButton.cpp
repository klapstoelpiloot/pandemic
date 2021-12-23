#include "LEDButton.h"

// Constructor
LEDButton::LEDButton()
{
}

// Destructor
LEDButton::~LEDButton()
{
}

void LEDButton::Init(int buttonpin, int ledpin)
{
	Button::Init(buttonpin);
	led.Init(ledpin);
}

void LEDButton::SetLED(PinStatus state)
{
	led.Set(state);
}
