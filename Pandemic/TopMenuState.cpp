#include "TopMenuState.h"
#include "MenuStateMachine.h"
#include "GameStateMachine.h"
#include "Main.h"

TopMenuState::TopMenuState(MenuStateMachine* _statemachine) :
	statemachine(_statemachine),
	newgameitem("START GAME"),
	highscoresitem("HIGHSCORES"),
	instructionsitem("INSTRUCTIONS"),
	optionsitem("OPTIONS"),
	restartitem("RESTART GAME"),
	stopitem("STOP GAME"),
	woosh(Main::GetResources().GetSound("woosh1.wav")),
	cancel(Main::GetResources().GetSound("cancel.wav")),
	accept(Main::GetResources().GetSound("confirm.wav")),
	currentitem(0)
{
	statemachine->GetVerticalRenderer()->AddItem(&horizontalrenderer);
}

void TopMenuState::Enter()
{
	// Set up the items which we show
	horizontalrenderer.Clear();
	itemslist.clear();

	newgameitem.SetName(statemachine->IsInGame() ? "NEW GAME" : "START GAME");
	horizontalrenderer.AddItem(&newgameitem);
	itemslist.push_back(TopMenuItems::NewGame);

	if(!statemachine->IsInGame())
	{
		horizontalrenderer.AddItem(&highscoresitem);
		itemslist.push_back(TopMenuItems::Highscores);
	}

	//horizontalrenderer.AddItem(&instructionsitem);
	//itemslist.push_back(TopMenuItems::Instructions);

	horizontalrenderer.AddItem(&optionsitem);
	itemslist.push_back(TopMenuItems::Options);

	if(statemachine->IsInGame())
	{
		//horizontalrenderer.AddItem(&restartitem);
		//itemslist.push_back(TopMenuItems::RestartGame);

		horizontalrenderer.AddItem(&stopitem);
		itemslist.push_back(TopMenuItems::StopGame);
	}

	horizontalrenderer.ShowArrows(true);
	Main::GetButtons().SetAllMenuLEDsOn();
}

void TopMenuState::Leave()
{
	horizontalrenderer.ShowArrows(false);
	Main::GetButtons().SetAllMenuLEDsOff();
}

void TopMenuState::Update()
{
}

bool TopMenuState::HandleMessage(const IOModule_IOMessage& msg)
{
	switch(msg.which_Content)
	{
		case IOModule_IOMessage_LeftButtonPressed_tag:
			if(currentitem == 0)
				currentitem = static_cast<int>(itemslist.size()) - 1;
			else
				currentitem--;
			horizontalrenderer.MoveToIndex(currentitem, false);
			woosh.Play();
			return true;

		case IOModule_IOMessage_RightButtonPressed_tag:
			if(currentitem == static_cast<int>(itemslist.size()) - 1)
				currentitem = 0;
			else
				currentitem++;
			horizontalrenderer.MoveToIndex(currentitem, true);
			woosh.Play();
			return true;

		case IOModule_IOMessage_AcceptButtonPressed_tag:
			ChooseItem();
			accept.Play();
			return true;

		case IOModule_IOMessage_CancelButtonPressed_tag:
			statemachine->Hide();
			cancel.Play();
			return true;

		default:
			return false;
	}
}

void TopMenuState::ChooseItem()
{
	switch(itemslist[currentitem])
	{
		case TopMenuItems::NewGame:
			statemachine->GetSelectGameState()->SetCallback(std::bind(&TopMenuState::StartGame, this, _1));
			statemachine->ChangeState(statemachine->GetSelectGameState());
			break;

		case TopMenuItems::Highscores:
			statemachine->GetSelectGameState()->SetCallback(std::bind(&TopMenuState::ShowHighscores, this, _1));
			statemachine->ChangeState(statemachine->GetSelectGameState());
			break;

		case TopMenuItems::Instructions:
			break;

		case TopMenuItems::Options:
			statemachine->ChangeState(statemachine->GetOptionsState());
			break;

		case TopMenuItems::RestartGame:
		{
			statemachine->Hide();
			GameType type = Main::GetGame().GetData().GetType();
			Main::GetGame().EndGame();
			Main::GetGame().CreateNewGame(type);
			break;
		}

		case TopMenuItems::StopGame:
			Main::GetGame().AllowTitleMusicRestart();
			statemachine->Hide();
			Main::GetGame().EndGame();
			break;

		default:
			NOT_IMPLEMENTED;
			break;
	}
}

void TopMenuState::SetBeginPosition(bool newgameposition)
{
	// Don't want to accedentially start a new game, so don't
	// show NEW GAME as the first item when opening the menu in game.
	if(newgameposition)
		currentitem = 0;
	else
		currentitem = 1;

	horizontalrenderer.SetAtIndex(currentitem);
}

void TopMenuState::StartGame(GameType selectedtype)
{
	statemachine->Hide();
	Main::GetResources().GetMusic("title.mp3").Stop();
	Main::GetGame().CreateNewGame(selectedtype);
}

void TopMenuState::ShowHighscores(GameType selectedtype)
{
	statemachine->Hide();
	Main::GetGame().GetHighscoreState()->SetGameType(selectedtype);
	Main::GetGame().ChangeState(Main::GetGame().GetHighscoreState());
}
