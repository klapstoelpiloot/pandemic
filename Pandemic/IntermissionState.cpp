#include "IntermissionState.h"
#include "GameStateMachine.h"
#include "MenuStateMachine.h"
#include "Main.h"

#define SHOW_ROUND_TIME			1600
#define BUTTON_FLASH_INTERVAL	400
#define ROUND_COMPLETE_TIME		3000

IntermissionState::IntermissionState(GameStateMachine* _statemachine) :
	statemachine(_statemachine),
	step(IntermissionStep::RoundComplete),
	flashbuttonson(false)
{
}

void IntermissionState::Enter()
{
	GameData& gd = statemachine->GetData();

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

	Main::GetIO().SetButtonLights(false, false, false, true);

	// Begin showing the round is completed (or empty screen on first round)
	// Always waits for at least the screen melt time
	step = IntermissionStep::RoundComplete;
	if(gd.NumRounds() == 0)
	{
		nextsteptime = Clock::now() + ch::milliseconds(1);
		Main::GetResources().GetMusic("inter_short.mp3").Stop();
	}
	else
	{
		nextsteptime = Clock::now() + ch::milliseconds(ROUND_COMPLETE_TIME);
		renderer.ShowRoundComplete(gd.NumRounds());
		Main::GetResources().GetMusic("inter_short.mp3").Play();
	}
}

void IntermissionState::Leave()
{
	Main::GetIO().SetButtonLightsAllOff();
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
		{
			bool buttonson = (((ch::ToMilliseconds(Clock::now()) / BUTTON_FLASH_INTERVAL) % 2) == 0);
			if(buttonson != flashbuttonson)
			{
				flashbuttonson = buttonson;
				if(buttonson)
					Main::GetIO().SetButtonLights(false, false, true, true);
				else
					Main::GetIO().SetButtonLights(false, false, false, true);
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
	if(gd.NumRounds() == 0)
	{
		BeginShowRound();
	}
	else
	{
		// Show pucks left for next round
		bool isbonusround = false;
		int nextroundpucks = gd.CalculateNextRoundPucks(&isbonusround);
		renderer.ShowPucksLeft(nextroundpucks, (gd.NumRounds() == (GAME_ROUNDS - 1)));
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
	RoundData& rd = gd.AddRound(gd.NumRounds(), nextroundpucks);
	rd.isbonus = isbonusround;

	// Theatrical sound and graphics
	String soundfile = String("round") + gd.NumRounds() + String(".wav");
	Main::GetResources().GetSound(soundfile).Play();
	renderer.ShowRound(rd.index + 1);
	Main::GetIO().SetButtonLights(false, false, false, true);
	Main::GetIO().SendRoundReset();

	step = IntermissionStep::ShowRound;
	nextsteptime = Clock::now() + ch::milliseconds(SHOW_ROUND_TIME);
}

bool IntermissionState::HandleMessage(const IOModule_IOMessage& msg)
{
	switch(msg.which_Content)
	{
		case IOModule_IOMessage_AcceptButtonPressed_tag:
			if(step == IntermissionStep::ShowPucksLeft)
			{
				Main::GetResources().GetMusic("inter_short.mp3").Stop();
				Main::GetResources().GetSound("continue.wav").Play();
				BeginShowRound();
			}
			break;

		case IOModule_IOMessage_CancelButtonPressed_tag:
			Main::GetMenu().Show();
			return true;
	}

	return false;
}
