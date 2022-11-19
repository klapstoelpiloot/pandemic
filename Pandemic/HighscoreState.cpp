#include "HighscoreState.h"
#include "MenuStateMachine.h"
#include "GameStateMachine.h"
#include "Main.h"

#define ALTERNATE_TITLE_TIME	30000

HighscoreState::HighscoreState(GameStateMachine* _statemachine) :
	statemachine(_statemachine),
	gametype(GameType::Casual)
{
}

void HighscoreState::Enter()
{
	// TODO: Temporary fix. Remove when renderers are fixed so that menu can stay open.
	Main::GetMenu().Hide();

	Main::GetResources().GetMusic("inter_short.mp3").Stop();
	Main::GetResources().GetMusic("inter_long.mp3").Stop();

	statemachine->GetScreenMelt().Begin();

	Main::GetButtons().SetAllGameLEDs(false, false, false, true);

	// Set IO module in normal operation mode
	Main::GetIO().SetNormalMode();

	Setup();

	// Setup rendering
	Main::GetGraphics().ClearRenderers();
	Main::GetGraphics().AddRenderer(&statemachine->GetBackground());
	Main::GetGraphics().AddRenderer(&renderer);
	Main::GetGraphics().AddRenderer(&statemachine->GetScreenMelt());
}

void HighscoreState::Leave()
{
	Main::GetButtons().SetAllGameLEDsOff();
}

void HighscoreState::Update()
{
	statemachine->PlayOrRepeatTitleMusic();

	if(Main::GetMenu().IsShown())
	{
		// When the menu is shown, we don't want to switch to another screen
		SetAlternatingTime();
	}
	else if(ch::IsTimeSet(showtitletime) && (Clock::now() > showtitletime))
	{
		// Go to the title when it is time
		statemachine->ChangeState(statemachine->GetTitleState());
	}
}

bool HighscoreState::HandleMessage(const IOModule_IOMessage& msg)
{
	switch(msg.which_Content)
	{
		case IOModule_IOMessage_CancelButtonPressed_tag:
			Main::GetMenu().Show();
			return true;

		default:
			return false;
	}
}

void HighscoreState::SetGameType(GameType _gametype)
{
	gametype = _gametype;
}

void HighscoreState::Setup()
{
	HighscoreManager scores = Main::GetScores();
	scores.Load();
	renderer.Setup(scores.GetCasualHighscores(), scores.GetProHighscores());
	SetAlternatingTime();
}

void HighscoreState::SetAlternatingTime()
{
	// Alternating between title and highscore screens?
	if(Main::GetConfig().GetBool("General.AlternateTitleHighscores", false))
		showtitletime = Clock::now() + ch::milliseconds(ALTERNATE_TITLE_TIME);
	else
		showtitletime = TimePoint();
}
