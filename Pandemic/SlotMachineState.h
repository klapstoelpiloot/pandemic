#pragma once
#include "IState.h"
#include "IMessageHandler.h"
#include "SlotMachineRenderer.h"
#include "SemiRandom.h"

class GameStateMachine;

// The game is played in this state
class SlotMachineState : public virtual IState, public virtual IMessageHandler
{
private:

	// Members
	GameStateMachine* statemachine;
	SlotMachineRenderer renderer;
	bool isrolling;
	SemiRandom random;
	TweenFloat wheelroll[NUM_WHEELS];
	float wheelposition[NUM_WHEELS];
	TimePoint laststeptime;

	void StartRoll();

public:

	SlotMachineState(GameStateMachine* _statemachine);

	// Methods
	virtual void Enter() override final;
	virtual void Leave() override final;
	virtual void Update() override final;
	virtual bool HandleMessage(const IOModule_IOMessage& msg) override final;
};
