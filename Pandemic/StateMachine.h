#pragma once

class IState;

class StateMachine
{
protected:

	IState* previousstate;
	IState* currentstate;
	IState* nextstate;

public:

	StateMachine();

	virtual void ChangeState(IState* tostate);
	virtual void Update();
	IState* GetCurrentState() { return currentstate; }
	const IState* GetCurrentState() const { return currentstate; }
};
