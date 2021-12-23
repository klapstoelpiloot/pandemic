#pragma once
#include "IState.h"
#include "IMessageHandler.h"
#include "Sound.h"
#include "SystemTestRenderer.h"

class GameStateMachine;

// This state is for testing the system components
class SystemTestState : public virtual IState, public virtual IMessageHandler
{
private:

	GameStateMachine* statemachine;
	SystemTestRenderer renderer;
	const Sound& changesound;
	bool acceptpressed;
	bool cancelpressed;

public:

	SystemTestState(GameStateMachine* _statemachine);

	virtual void Enter() override final;
	virtual void Leave() override final;
	virtual void Update() override final;
	virtual bool HandleMessage(const IOModule_IOMessage& msg) override final;
};
