#include "IntermissionState.h"
#include "GameStateMachine.h"
#include "MenuStateMachine.h"
#include "Main.h"

#define SHOW_ROUND_TIME				1600
#define BUTTON_FLASH_INTERVAL		400
#define ROUND_COMPLETE_TIME			3000
#define FIST_ROUND_MIN_WAIT_TIME	200

IntermissionState::IntermissionState(GameStateMachine* _statemachine) :
	statemachine(_statemachine),
	step(IntermissionStep::RoundComplete),
	flashbuttonson(false),
	sensorblocked(false)
{
}

void IntermissionState::Enter()
{
	GameData& gd = statemachine->GetData();
	statemachine->StopTitleMusic();

	// TODO: Temporary fix. Remove when renderers are fixed so that menu can stay open.
	Main::GetMenu().Hide();

	// Setup rendering
	renderer.Clear();
	Main::GetGraphics().ClearRenderers();
	Main::GetGraphics().AddRenderer(&statemachine->GetBackground());
	Main::GetGraphics().AddRenderer(&renderer);
	Main::GetGraphics().AddRenderer(&screendissolve);
	Main::GetGraphics().AddRenderer(&statemachine->GetScreenMelt());
	flashbuttonson = false;

	Main::GetButtons().SetAllGameLEDs(false, false, false, true);

	// Begin showing the round is completed (or empty screen on first round)
	// Always waits for at least the screen melt time
	step = IntermissionStep::RoundComplete;
	if(gd.CurrentRoundNumber() == 0)
	{
		nextsteptime = Clock::now() + ch::milliseconds(FIST_ROUND_MIN_WAIT_TIME);
		Main::GetResources().GetMusic("inter_short.mp3").Stop();
	}
	else
	{
		nextsteptime = Clock::now() + ch::milliseconds(ROUND_COMPLETE_TIME);
		renderer.ShowRoundComplete(gd.CurrentRoundNumber());
		Main::GetResources().GetMusic("inter_short.mp3").Play();
	}
}

void IntermissionState::Leave()
{
	Main::GetButtons().SetAllGameLEDsOff();
}

void IntermissionState::Update()
{
	switch(step)
	{
		case IntermissionStep::RoundComplete:
			if(statemachine->GetScreenMelt().IsFinished() && (Clock::now() > nextsteptime))
				BeginShowPucksLeft();
			break;

		case IntermissionStep::ShowPucksLeft:
		case IntermissionStep::CheckSensors:
		{
			bool buttonson = (((ch::ToMilliseconds(Clock::now()) / BUTTON_FLASH_INTERVAL) % 2) == 0);
			if(buttonson != flashbuttonson)
			{
				flashbuttonson = buttonson;
				if(buttonson)
					Main::GetButtons().SetGameLED(Button::Accept, true);
				else
					Main::GetButtons().SetGameLED(Button::Accept, false);
			}
			break;
		}

		case IntermissionStep::ShowRound:
			if(Clock::now() > nextsteptime)
			{
				if(Random(0, 10) == 0)
					Main::GetResources().GetSound("goodluck.wav").Play();
				else
					Main::GetResources().GetSound("go.wav").Play();
				statemachine->ChangeState(statemachine->GetPlayingState());
			}
			break;
	}
}

void IntermissionState::BeginShowPucksLeft()
{
	GameData& gd = statemachine->GetData();

	// If we just started the game (as opposed to finishing a round) then
	// immediately introduce the first round...
	if(gd.CurrentRoundNumber() == 0)
	{
		CheckSensorsForNextRound();
	}
	else
	{
		// Show pucks left for next round
		renderer.ShowPucksLeft(gd);
		step = IntermissionStep::ShowPucksLeft;
		screendissolve.Begin();
	}
}

void IntermissionState::BeginShowRound()
{
	// Set up a new round
	GameData& gd = statemachine->GetData();
	bool isbonusround = false;
	int nextroundpucks = gd.CalculateNextRoundPucks(&isbonusround);
	RoundData& rd = gd.AddRound(gd.CurrentRoundNumber(), nextroundpucks);
	rd.isbonus = isbonusround;

	// Theatrical sound and graphics
	String soundfile = String("round") + String::From(gd.CurrentRoundNumber()) + String(".wav");
	Main::GetResources().GetSound(soundfile).Play();
	renderer.ShowRound(rd.index + 1);
	Main::GetButtons().SetAllGameLEDs(false, false, false, true);
	Main::GetIO().SendRoundReset();

	step = IntermissionStep::ShowRound;
	nextsteptime = Clock::now() + ch::milliseconds(SHOW_ROUND_TIME);
}

void IntermissionState::CheckSensorsForNextRound()
{
	step = IntermissionStep::CheckSensors;
	Main::GetIO().RequestGateStates();
}

bool IntermissionState::HandleMessage(const IOModule_IOMessage& msg)
{
	switch(msg.which_Content)
	{
		case IOModule_IOMessage_SensorBlocked_tag:
			sensorblocked = true;
			return true;

		case IOModule_IOMessage_SensorCleared_tag:
			sensorblocked = false;
			return true;

		case IOModule_IOMessage_StartSlide_tag:
			if((step == IntermissionStep::ShowPucksLeft) || (step == IntermissionStep::CheckSensors))
			{
				Main::GetResources().GetSound("error.wav").Play();
				renderer.FlashGreenForNextRound();
			}
			return true;

		case IOModule_IOMessage_AcceptButtonPressed_tag:
			if((step == IntermissionStep::ShowPucksLeft) || (step == IntermissionStep::CheckSensors))
			{
				Main::GetResources().GetSound("continue.wav").Play();
				CheckSensorsForNextRound();
			}
			break;

		case IOModule_IOMessage_CancelButtonPressed_tag:
			Main::GetMenu().Show();
			return true;

		case IOModule_IOMessage_GateStates_tag:
		{
			bool gateblocked[4];
			gateblocked[0] = msg.Content.GateStates.Gate1Blocked;
			gateblocked[1] = msg.Content.GateStates.Gate2Blocked;
			gateblocked[2] = msg.Content.GateStates.Gate3Blocked;
			gateblocked[3] = msg.Content.GateStates.Gate4Blocked;
			if(sensorblocked)
			{
				// One or more pucks are under the bridge (blocking the sensor)
				Main::GetResources().GetSound("error.wav").Play();
				renderer.ShowPucksBlockingBridge();
			}
			else if(gateblocked[0] || gateblocked[1] || gateblocked[2] || gateblocked[3])
			{
				// One or more pucks are in the gates (blocking the sensors)
				Main::GetResources().GetSound("error.wav").Play();
				renderer.ShowPucksBlockingGates(gateblocked);
			}
			else
			{
				// We're goo to go! Begin next round.
				Main::GetResources().GetMusic("inter_short.mp3").Stop();
				BeginShowRound();
			}
			return true;
		}
	}

	return false;
}
