#pragma once
#include "IState.h"
#include "Sound.h"
#include "TitleRenderer.h"
#include "IMessageHandler.h"

class GameStateMachine;

// The title screen
class TitleState : public virtual IState, public virtual IMessageHandler
{
private:

	GameStateMachine* statemachine;
	TitleRenderer renderer;
	TimePoint flashtime;
	bool flashbuttonson;
	TimePoint flashstarttime;
	TimePoint buttonunlocktime;
	TimePoint showhighscorestime;

	// Methods
	void SetAlternatingTime();

public:

	TitleState(GameStateMachine* _statemachine);
	
	virtual void Enter() override final;
	virtual void Leave() override final;
	virtual void Update() override final;
	virtual bool HandleMessage(const IOModule_IOMessage& msg) override final;
	TitleRenderer* GetRenderer() { return &renderer; }
};
