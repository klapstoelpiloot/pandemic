#pragma once
#include "IState.h"
#include "IMessageHandler.h"
#include "FinishRenderer.h"
#include "ScreenDissolveRenderer.h"

class GameStateMachine;

// We go to this state when the last round is finished
class FinishedState : public virtual IState, public virtual IMessageHandler
{
private:

	enum class FinishedStep
	{
		GameOver,
		ShowTotalScore,
		HighscoreText,
		Finished
	};

	// Members
	GameStateMachine* statemachine;
	FinishRenderer renderer;
	ScreenDissolveRenderer screendissolve;
	FinishedStep step;
	TimePoint nextsteptime;
	bool flashbuttonson;
	bool ishighscore;

	// Methods
	void BeginShowTotalScore();
	void BeginHighScoreTextIfWorthy();

public:

	FinishedState(GameStateMachine* _statemachine);

	// Methods
	virtual void Enter() override final;
	virtual void Leave() override final;
	virtual void Update() override final;
	virtual bool HandleMessage(const IOModule_IOMessage& msg) override final;
};
