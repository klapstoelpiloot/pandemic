#include "TitleState.h"
#include "GameStateMachine.h"
#include "MenuStateMachine.h"
#include "Main.h"

#define BUTTON_FIRST_BLOCK_TIME		600
#define BUTTON_BLOCK_TIME			600
#define BUTTON_FIRST_FLASH_DELAY	11500
#define BUTTON_FLASH_DELAY			11500
#define BUTTON_FLASH_INTERVAL		400
#define ALTERNATE_HIGHSCORES_TIME	30000

TitleState::TitleState(GameStateMachine* _statemachine) :
	statemachine(_statemachine),
	flashbuttonson(false)
{
	renderer.SetPlayTitleMusicFunction(std::bind(&GameStateMachine::PlayOrRepeatTitleMusic, statemachine));
	renderer.BeginFirstStart();
	flashstarttime = Clock::now() + ch::milliseconds(BUTTON_FIRST_FLASH_DELAY);
	buttonunlocktime = Clock::now() + ch::milliseconds(BUTTON_FIRST_BLOCK_TIME);
}

void TitleState::Enter()
{
	// TODO: Temporary fix. Remove when renderers are fixed so that menu can stay open.
	Main::GetMenu().Hide();

	// Set IO module in normal operation mode
	Main::GetIO().SetNormalMode();

	Main::GetResources().GetMusic("inter_short.mp3").Stop();
	Main::GetResources().GetMusic("inter_long.mp3").Stop();

	Main::GetGraphics().ClearRenderers();
	Main::GetGraphics().AddRenderer(&renderer);
	Main::GetGraphics().AddRenderer(&statemachine->GetScreenMelt());

	// If the time has passed the button unlock, then reinitialize the button/flash times
	if(Clock::now() > buttonunlocktime)
	{
		statemachine->GetScreenMelt().Begin();
		renderer.BeginReturnToTitle();
		flashstarttime = Clock::now() + ch::milliseconds(BUTTON_FLASH_DELAY);
		buttonunlocktime = Clock::now() + ch::milliseconds(BUTTON_BLOCK_TIME);
	}

	SetAlternatingTime();
}

void TitleState::Leave()
{
	Main::GetButtons().SetAllGameLEDsOff();
}

void TitleState::Update()
{
	int64 t = ToMilliseconds(Clock::now() - flashstarttime);
	bool buttonson = (t > 0) && (((t / BUTTON_FLASH_INTERVAL) % 2) == 0);
	renderer.SetKeyFlashState(buttonson);
	if(buttonson != flashbuttonson)
	{
		flashbuttonson = buttonson;
		if(flashbuttonson)
		{
			// Turn all button LEDs on
			Main::GetButtons().SetAllGameLEDsOn();
		}
		else
		{
			// Turn all button LEDs off
			Main::GetButtons().SetAllGameLEDsOff();
		}
	}

	if(Main::GetMenu().IsShown())
	{
		// When the menu is shown, we don't want to switch to another screen
		SetAlternatingTime();
	}
	else if(ch::IsTimeSet(showhighscorestime) && (Clock::now() > showhighscorestime))
	{
		// Go to the highscores when it is time
		statemachine->ChangeState(statemachine->GetHighscoreState());
	}
}

bool TitleState::HandleMessage(const IOModule_IOMessage& msg)
{
	int64 t = ToMilliseconds(Clock::now() - buttonunlocktime);
	switch(msg.which_Content)
	{
		// Press any button to start
		case IOModule_IOMessage_LeftButtonPressed_tag:
		case IOModule_IOMessage_RightButtonPressed_tag:
		case IOModule_IOMessage_AcceptButtonPressed_tag:
		case IOModule_IOMessage_CancelButtonPressed_tag:
			if(t > 0)
				Main::GetMenu().Show();
			return true;

		default:
			return false;
	}
}

void TitleState::SetAlternatingTime()
{
	// Alternating between title and highscore screens?
	if(Main::GetConfig().GetBool("General.AlternateTitleHighscores", false))
		showhighscorestime = Clock::now() + ch::milliseconds(ALTERNATE_HIGHSCORES_TIME);
	else
		showhighscorestime = TimePoint();
}
