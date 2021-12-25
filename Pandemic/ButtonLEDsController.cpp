#include "ButtonLEDsController.h"
#include "Main.h"

// Constructor
ButtonLEDsController::ButtonLEDsController() :
	gameleds{ false, false, false, false },
	menuleds{ false, false, false, false },
	showmenuleds(false)
{
}

// Destructor
ButtonLEDsController::~ButtonLEDsController()
{
}

void ButtonLEDsController::SetGameLED(Button button, bool state)
{
	gameleds[(int)button] = state;
	UpdateIO();
}

void ButtonLEDsController::SetAllGameLEDs(bool left, bool right, bool accept, bool cancel)
{
	gameleds[(int)Button::Left] = left;
	gameleds[(int)Button::Right] = right;
	gameleds[(int)Button::Accept] = accept;
	gameleds[(int)Button::Cancel] = cancel;
	UpdateIO();
}

void ButtonLEDsController::SetMenuLED(Button button, bool state)
{
	menuleds[(int)button] = state;
	UpdateIO();
}

void ButtonLEDsController::SetAllMenuLEDs(bool left, bool right, bool accept, bool cancel)
{
	menuleds[(int)Button::Left] = left;
	menuleds[(int)Button::Right] = right;
	menuleds[(int)Button::Accept] = accept;
	menuleds[(int)Button::Cancel] = cancel;
	UpdateIO();
}

void ButtonLEDsController::ShowMenuLEDs(bool show)
{
	if(showmenuleds != show)
	{
		showmenuleds = show;
		UpdateIO();
	}
}

void ButtonLEDsController::UpdateIO()
{
	if(showmenuleds)
	{
		Main::GetIO().SetButtonLights(menuleds[(int)Button::Left], menuleds[(int)Button::Right],
			menuleds[(int)Button::Accept], menuleds[(int)Button::Cancel]);
	}
	else
	{
		Main::GetIO().SetButtonLights(gameleds[(int)Button::Left], gameleds[(int)Button::Right],
			gameleds[(int)Button::Accept], gameleds[(int)Button::Cancel]);
	}
}
