#pragma once
#include "IState.h"
#include "IMessageHandler.h"
#include "GameDesc.h"
#include "HighscoreRenderer.h"
#include "Sound.h"

class GameStateMachine;

// This state is for testing the system components
class HighscoreState : public virtual IState, public virtual IMessageHandler
{
private:

	// Members
	GameStateMachine* statemachine;
	HighscoreRenderer renderer;
	GameType gametype;
	TimePoint showtitletime;

	// Methods
	void Setup();
	void SetAlternatingTime();

public:

	HighscoreState(GameStateMachine* _statemachine);

	void SetGameType(GameType _gametype);
	virtual void Enter() override final;
	virtual void Leave() override final;
	virtual void Update() override final;
	virtual bool HandleMessage(const IOModule_IOMessage& msg) override final;
};
