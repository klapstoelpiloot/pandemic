#include "GameStateMachine.h"
#include "Main.h"

GameStateMachine::GameStateMachine() :
	data(nullptr),
	titlestate(this),
	playingstate(this),
	finishedstate(this),
	intermissionstate(this),
	systemteststate(this),
	removepuckstate(this),
	enternamestate(this),
	highscorestate(this),
	titlemusic(Main::GetResources().GetMusic("title.mp3")),
	titlemusicchecked(false),
	titlemusicrepeat(Main::GetConfig().GetBool("General.RepeatTitleMusic", true))
{
}

GameStateMachine::~GameStateMachine()
{
	SAFE_DELETE(data);
}

void GameStateMachine::CreateNewGame(GameType type)
{
	SAFE_DELETE(data);
	data = new GameData(type);
	const GameDesc& gd = GameDesc::Find(type);
	Main::GetIO().SendSettings(gd.officialrules);
	background.Begin();
	screenmelt.Begin();
	ChangeState(&intermissionstate);
}

void GameStateMachine::EndGame()
{
	SAFE_DELETE(data);
	ChangeState(&titlestate);
}

void GameStateMachine::AllowTitleMusicRestart()
{
	titlemusicchecked = false;
}

void GameStateMachine::PlayOrRepeatTitleMusic()
{
	// Always check again if we want to repeat the title music.
	// Otherwise only play the music when indicated using AllowTitleMusicRestart()
	if(titlemusicrepeat || !titlemusicchecked)
	{
		if(!titlemusic.IsPlaying())
			titlemusic.Play(true);

		titlemusicchecked = true;
	}
}

void GameStateMachine::StopTitleMusic()
{
	titlemusic.Stop();
}

bool GameStateMachine::HandleMessage(const IOModule_IOMessage& msg)
{
	// If the current state implements IMessageHandler, let it handle the message
	IMessageHandler* handler = dynamic_cast<IMessageHandler*>(currentstate);
	if(handler != nullptr)
	{
		if(handler->HandleMessage(msg))
			return true;
	}

	// If left/right buttons are not handled, then use them to change background color
	switch(msg.which_Content)
	{
		case IOModule_IOMessage_LeftButtonPressed_tag:
			background.SetPrevColor();
			return true;

		case IOModule_IOMessage_RightButtonPressed_tag:
			background.SetNextColor();
			return true;

		default:
			return false;
	}
}
