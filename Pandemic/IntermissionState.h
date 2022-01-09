#pragma once
#include "IState.h"
#include "IMessageHandler.h"
#include "IntermissionRenderer.h"
#include "ScreenDissolveRenderer.h"

class GameStateMachine;

// This is the state we go to before a new round begins
class IntermissionState : public virtual IState, public virtual IMessageHandler
{
private:

	enum class IntermissionStep
	{
		RoundComplete,
		ShowPucksLeft,
		ShowRound
	};

	// Members
	GameStateMachine* statemachine;
	IntermissionRenderer renderer;
	ScreenDissolveRenderer screendissolve;
	IntermissionStep step;
	TimePoint nextsteptime;
	bool flashbuttonson;

	// Methods
	void BeginShowPucksLeft();
	void BeginShowRound();

public:

	IntermissionState(GameStateMachine* _statemachine);

	// Methods
	virtual void Enter() override final;
	virtual void Leave() override final;
	virtual void Update() override final;
	virtual bool HandleMessage(const IOModule_IOMessage& msg) override final;
};
