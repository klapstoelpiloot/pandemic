#pragma once
#include "IState.h"
#include "IMessageHandler.h"
#include "Sound.h"
#include "MenuItemRenderer.h"
#include "HorizontalMenuRenderer.h"
#include "GameDesc.h"

class MenuStateMachine;

class SelectGameState : public virtual IState, public virtual IMessageHandler
{
public:
	typedef std::function<void(GameType selectedtype)> Callback;

private:

	MenuStateMachine* statemachine;
	HorizontalMenuRenderer horizontalrenderer;
	vector<MenuItemRenderer*> gameitems;
	const Sound& woosh;
	const Sound& cancel;
	const Sound& accept;
	int currentitem;
	Callback callback;

public:

	SelectGameState(MenuStateMachine* _statemachine);
	virtual ~SelectGameState();

	virtual void Enter() override final;
	virtual void Leave() override final;
	virtual void Update() override final;
	virtual bool HandleMessage(const IOModule_IOMessage& msg) override final;
	void SetCallback(Callback onselect) { callback = onselect; }
};
