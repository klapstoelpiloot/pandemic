#pragma once
#include "IState.h"
#include "IMessageHandler.h"

class GameStateMachine;

// The game is played in this state
class SlotMachineState : public virtual IState, public virtual IMessageHandler
{
private:

	// Members
	GameStateMachine* statemachine;
	bool isrolling;


public:

	SlotMachineState(GameStateMachine* _statemachine);

	// Methods
	virtual void Enter() override final;
	virtual void Leave() override final;
	virtual void Update() override final;
	virtual bool HandleMessage(const IOModule_IOMessage& msg) override final;
};
