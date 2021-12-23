#pragma once
#include "IState.h"
#include "IMessageHandler.h"
#include "Sound.h"
#include "HorizontalMenuRenderer.h"
#include "MenuItemRenderer.h"
#include "GameDesc.h"

class MenuStateMachine;

enum class TopMenuItems
{
	NewGame,
	Highscores,
	Instructions,
	Options,
	RestartGame,
	StopGame
};

class TopMenuState : public virtual IState, public virtual IMessageHandler
{
private:

	// Members
	MenuStateMachine* statemachine;
	HorizontalMenuRenderer horizontalrenderer;
	MenuItemRenderer newgameitem;
	MenuItemRenderer highscoresitem;
	MenuItemRenderer instructionsitem;
	MenuItemRenderer optionsitem;
	MenuItemRenderer restartitem;
	MenuItemRenderer stopitem;
	vector<TopMenuItems> itemslist;
	const Sound& woosh;
	const Sound& cancel;
	const Sound& accept;
	int currentitem;

	// Methods
	void ChooseItem();

public:

	TopMenuState(MenuStateMachine* _statemachine);

	virtual void Enter() override final;
	virtual void Leave() override final;
	virtual void Update() override final;
	virtual bool HandleMessage(const IOModule_IOMessage& msg) override final;

	void SetBeginPosition(bool newgameposition);
	void StartGame(GameType selectedtype);
	void ShowHighscores(GameType selectedtype);
};
