#pragma once
#include "IState.h"
#include "IMessageHandler.h"
#include "RemovePuckRenderer.h"

class GameStateMachine;

// State to pause the game so that the user can remove pucks which came back and/or are blocking the sensor
class RemovePuckState : public virtual IState, public virtual IMessageHandler
{
private:

	// Members
	GameStateMachine* statemachine;
	RemovePuckRenderer renderer;
	bool sensorblocked;
	bool flashbuttonson;

public:

	RemovePuckState(GameStateMachine* _statemachine);

	// Methods
	virtual void Enter() override final;
	virtual void Leave() override final;
	virtual void Update() override final;
	virtual bool HandleMessage(const IOModule_IOMessage& msg) override final;
	void SetSensorBlocked(bool isblocked) { sensorblocked = isblocked; }
};
