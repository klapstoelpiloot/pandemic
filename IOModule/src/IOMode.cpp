#include "IOMode.h"

IOMode::IOMode() :
	link(this),
	idlestate(this),
	normalstate(this),
	calibratestate(this),
	lednexttime(0)
{
	boardled.Init(LED_BUILTIN);
}

void IOMode::Update()
{
	unsigned long t = millis();

	// Handle incoming serial data
	bool msgreceived = link.Update();
	if(msgreceived)
	{
		const IOModule_PiMessage& msg = link.GetMessage();
		switch(msg.which_Content)
		{
			// Switch to normal mode
			case IOModule_PiMessage_NormalMode_tag:
			{
				if(currentstate != static_cast<IState*>(&normalstate))
					ChangeState(&normalstate);
				break;
			}

			// Switch to calibration mode
			case IOModule_PiMessage_CalibrateMode_tag:
			{
				if(currentstate != static_cast<IState*>(&calibratestate))
					ChangeState(&calibratestate);
				break;
			}

			// Apply general settings
			case IOModule_PiMessage_Settings_tag:
			{
				const IOModule_SettingsMsg& settings = msg.Content.Settings;
				normalstate.SetSlideTimeout(settings.SlideTimeoutMs);
				normalstate.SetHardMode(settings.HardMode);
				normalstate.SetPuckLostTimeout(settings.PuckLostTimeout);
				Button::SetTiming(settings.DebounceTimeout, settings.ButtonRepeatTimeout, settings.ButtonRepeatInterval);
				ChangeState(currentstate);
				break;
			}

			// Apply button repeat setting
			case IOModule_PiMessage_SetButtonRepeat_tag:
			{
				const IOModule_SetButtonRepeatMsg& setting = msg.Content.SetButtonRepeat;
				Button::SetRepeat(setting.Enable);
				break;
			}

			// Any other message is handled by the current mode
			default:
			{
				if(currentstate != nullptr)
					currentstate->HandleMessage(msg);
				break;
			}
		}
	}
	// Change to idle state when no more keep alive signals are received
	else if(!link.IsAlive())
	{
		if(currentstate != static_cast<IState*>(&idlestate))
			ChangeState(&idlestate);
	}

	// Flash the board LED for debugging purpose
	if((morsecode.length() > 0) && (t >= lednexttime))
	{
		if(boardled.Get())
		{
			// Turn LED off for a fixed pause between digits
			boardled.Set(LOW);
			codeposition++;
			if(codeposition >= morsecode.length())
			{
				// Start over after long pause
				morsecode = nextmorsecode;
				codeposition = 0;
				lednexttime = t + 1400;
			}
			else
			{
				// Short pause between characters
				lednexttime = t + 200;
			}
		}
		else
		{
			// Turn LED on with a duration depending on digit
			boardled.Set(HIGH);
			switch(morsecode[codeposition])
			{
				case '.': lednexttime = t + 100; break;
				case '-': lednexttime = t + 600; break;
			}
		}
	}

	StateMachine::Update();
}

void IOMode::FlashDebugCode(String morse)
{
	if(morsecode.length() == 0)
	{
		// Stop flashing
		morsecode = morse;
		nextmorsecode = morse;
		codeposition = 0;
		boardled.Set(LOW);
		lednexttime = millis() + 1400;
	}
	else
	{
		// Begin flashing this code when the current flashing code is done
		nextmorsecode = morse;
	}
}
