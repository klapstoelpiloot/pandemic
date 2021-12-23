#pragma once
#include "IState.h"
#include "IMessageHandler.h"
#include "ClassicGameRenderer.h"
#include "RemovePuckRenderer.h"

class GameStateMachine;

// The game is played in this state
class PlayingState : public virtual IState, public virtual IMessageHandler
{
private:

	// Members
	GameStateMachine* statemachine;
	ClassicGameRenderer gamerenderer;
	TimePoint roundfinishtime;
	TimePoint throwfinishtime;
	TimePoint complimenttime;
	bool scoringenabled;
	int complimentchance;

	// Timing
	int slidetimeout;
	int roundtimeout;

	// Methods
	void UpdateDisplay();
	void PlayShotSound();
	void KillShotSounds();

public:

	PlayingState(GameStateMachine* _statemachine);

	// Methods
	virtual void Enter() override final;
	virtual void Leave() override final;
	virtual void Update() override final;
	virtual bool HandleMessage(const IOModule_IOMessage& msg) override final;
};
