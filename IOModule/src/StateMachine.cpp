#include "StateMachine.h"
#include "IState.h"

StateMachine::StateMachine() :
	previousstate(nullptr),
	currentstate(nullptr),
	nextstate(nullptr)
{
}

void StateMachine::ChangeState(IState* tostate)
{
	nextstate = tostate;

	// Notify current state
	if(currentstate != nullptr)
		currentstate->Leave();

	// Change state
	previousstate = currentstate;
	currentstate = nextstate;
	nextstate = nullptr;

	// Notify new state
	if(currentstate != nullptr)
		currentstate->Enter();

	previousstate = nullptr;
}

void StateMachine::Update()
{
	if(currentstate != nullptr)
		currentstate->Update();
}
