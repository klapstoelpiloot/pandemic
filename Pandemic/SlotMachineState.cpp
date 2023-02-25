#include "SlotMachineState.h"
#include "GameStateMachine.h"
#include "Main.h"
#include "MenuStateMachine.h"

SlotMachineState::SlotMachineState(GameStateMachine* _statemachine) :
	statemachine(_statemachine)
{
}

void SlotMachineState::Enter()
{
	// TODO: Temporary fix. Remove when renderers are fixed so that menu can stay open.
	Main::GetMenu().Hide();

	// Setup rendering
	// Animations before the game renderer, so that particles are renderer on top
	Main::GetGraphics().ClearRenderers();
	Main::GetGraphics().AddRenderer(&statemachine->GetBackground());
	//Main::GetGraphics().AddRenderer(&particlesoverlay);
	Main::GetGraphics().AddRenderer(&statemachine->GetScreenMelt());

	Main::GetButtons().SetAllGameLEDs(false, false, false, true);
}

void SlotMachineState::Leave()
{
}

void SlotMachineState::Update()
{
	TimePoint now = Clock::now();
	GameData& gd = statemachine->GetData();
}

bool SlotMachineState::HandleMessage(const IOModule_IOMessage& msg)
{
	GameData& gd = statemachine->GetData();
	switch(msg.which_Content)
	{
		case IOModule_IOMessage_AcceptButtonPressed_tag:
			return true;

		case IOModule_IOMessage_LeftButtonPressed_tag:
			return true;

		case IOModule_IOMessage_RightButtonPressed_tag:
			return true;

		case IOModule_IOMessage_CancelButtonPressed_tag:
			Main::GetMenu().Show();
			return true;

		default:
			return false;
	}
}
