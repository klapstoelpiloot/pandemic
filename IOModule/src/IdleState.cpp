#include "IdleState.h"
#include "IOMode.h"

#define FLASH_INTERVAL	400

IdleState::IdleState(IOMode* mode) :
	mode(mode),
	flashtime(0),
	ledstate(false),
	flashledstate(true)
{
}

void IdleState::Enter()
{
	flashtime = millis();
	ledstate = true;
	buttonleft.Init(PIN_BUTTONLEFT, PIN_LEDLEFT);
	buttonleft.SetLED(HIGH);
	buttoncancel.Init(PIN_BUTTONCANCEL, PIN_LEDCANCEL);
	buttoncancel.SetLED(LOW);
}

void IdleState::Leave()
{
	buttonleft.SetLED(LOW);
	buttoncancel.SetLED(LOW);
}

void IdleState::Update()
{
	// Flash the button
	if(millis() > (flashtime + FLASH_INTERVAL))
	{
		if(flashledstate && ledstate)
			buttonleft.SetLED(HIGH);
		else
			buttonleft.SetLED(LOW);

		ledstate = !ledstate;
		flashtime = millis();
	}
}

void IdleState::HandleMessage(const IOModule_PiMessage& msg)
{
	switch(msg.which_Content)
	{
		// Indicate an error happend
		case IOModule_PiMessage_IndicateError_tag:
		{
			buttoncancel.SetLED(HIGH);
			flashledstate = false;
			break;
		}
	}
}
