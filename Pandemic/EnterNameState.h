#pragma once
#include "IState.h"
#include "IMessageHandler.h"
#include "EnterNameRenderer.h"
#include "ScreenDissolveRenderer.h"

class GameStateMachine;

class EnterNameState : public virtual IState, public virtual IMessageHandler
{
private:

	// Members
	GameStateMachine* statemachine;
	EnterNameRenderer renderer;
	ScreenDissolveRenderer screendissolve;
	int charindex;
	String namestr;

	// Methods
	void UpdateEditChar(int change);

public:

	EnterNameState(GameStateMachine* _statemachine);

	// Methods
	virtual void Enter() override final;
	virtual void Leave() override final;
	virtual void Update() override final;
	virtual bool HandleMessage(const IOModule_IOMessage& msg) override final;
};
