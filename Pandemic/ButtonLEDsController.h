#pragma once
#include "Button.h"
#define NUM_BUTTON_LEDS		4

class ButtonLEDsController final
{
private:

	// The state of the LEDs for the game
	bool gameleds[NUM_BUTTON_LEDS];

	// The stat of the LEDs for the menu
	bool menuleds[NUM_BUTTON_LEDS];

	// True when the menu states are shown, False when the game states are shown
	bool showmenuleds;

	// Methods
	void UpdateIO();

public:

	ButtonLEDsController();
	~ButtonLEDsController();

	// Methods
	void SetGameLED(Button button, bool state);
	void SetAllGameLEDs(bool left, bool right, bool accept, bool cancel);
	void SetAllGameLEDsOff() { SetAllGameLEDs(false, false, false, false); };
	void SetAllGameLEDsOn() { SetAllGameLEDs(true, true, true, true); };
	void SetMenuLED(Button button, bool state);
	void SetAllMenuLEDs(bool left, bool right, bool accept, bool cancel);
	void SetAllMenuLEDsOff() { SetAllMenuLEDs(false, false, false, false); };
	void SetAllMenuLEDsOn() { SetAllMenuLEDs(true, true, true, true); };
	void ShowMenuLEDs(bool show);
};
