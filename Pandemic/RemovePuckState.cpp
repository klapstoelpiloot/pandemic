#include "RemovePuckState.h"
#include "MenuStateMachine.h"
#include "GameStateMachine.h"
#include "Main.h"

#define BUTTON_FLASH_INTERVAL		400

RemovePuckState::RemovePuckState(GameStateMachine* _statemachine) :
	statemachine(_statemachine),
	sensorblocked(false),
	flashbuttonson(false)
{
}

void RemovePuckState::Enter()
{
	// TODO: Temporary fix. Remove when renderers are fixed so that menu can stay open.
	Main::GetMenu().Hide();

	// Setup rendering
	Main::GetGraphics().ClearRenderers();
	Main::GetGraphics().AddRenderer(&statemachine->GetBackground());
	Main::GetGraphics().AddRenderer(&renderer);
	Main::GetGraphics().AddRenderer(&statemachine->GetScreenMelt());

	Main::GetButtons().SetAllGameLEDs(false, false, false, true);
}

void RemovePuckState::Leave()
{
	Main::GetButtons().SetAllGameLEDsOff();
}

void RemovePuckState::Update()
{
	bool buttonson = !sensorblocked && (((ch::ToMilliseconds(Clock::now()) / BUTTON_FLASH_INTERVAL) % 2) == 0);
	if(buttonson != flashbuttonson)
	{
		flashbuttonson = buttonson;
		if(buttonson)
			Main::GetButtons().SetGameLED(Button::Accept, true);
		else
			Main::GetButtons().SetGameLED(Button::Accept, false);
	}
}

bool RemovePuckState::HandleMessage(const IOModule_IOMessage& msg)
{
	switch(msg.which_Content)
	{
		case IOModule_IOMessage_SensorBlocked_tag:
			sensorblocked = true;
			return true;

		case IOModule_IOMessage_SensorCleared_tag:
			sensorblocked = false;
			return true;

		case IOModule_IOMessage_AcceptButtonPressed_tag:
			if(!sensorblocked)
			{
				Main::GetResources().GetSound("continue.wav").Play();
				statemachine->ChangeState(statemachine->GetPlayingState());
			}
			else
			{
				Main::GetResources().GetSound("error.wav").Play();
			}
			return true;

		case IOModule_IOMessage_CancelButtonPressed_tag:
			Main::GetMenu().Show();
			return true;

		case IOModule_IOMessage_StartSlide_tag:
			if(!sensorblocked)
			{
				statemachine->ChangeState(statemachine->GetPlayingState());
				return statemachine->GetPlayingState()->HandleMessage(msg);
			}
			else
			{
				Main::GetResources().GetSound("error.wav").Play();
				return true;
			}
	}

	return false;
}
