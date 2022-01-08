#pragma once
#include "IState.h"
#include "IMessageHandler.h"
#include "ParticleOverlayRenderer.h"
#include "ClassicHUDRenderer.h"
#include "RemovePuckRenderer.h"
#include "SetAnimation.h"
#include "GreatShotAnimation.h"
#include "HotShotAnimation.h"
#include "MegaShotAnimation.h"
#include "ImpressiveAnimation.h"
#include "ExcellentAnimation.h"
#include "IncredibleAnimation.h"
#include "UnstoppableAnimation.h"

class GameStateMachine;

// The game is played in this state
class PlayingState : public virtual IState, public virtual IMessageHandler
{
private:

	// Members
	GameStateMachine* statemachine;
	ParticleOverlayRenderer particlesoverlay;
	ClassicHUDRenderer hud;
	SetAnimation setanimation;
	GreatShotAnimation greatshotanimation;
	HotShotAnimation hotshotanimation;
	MegaShotAnimation megashotanimation;
	ImpressiveAnimation impressiveanimation;
	ExcellentAnimation excellentanimation;
	IncredibleAnimation incredibleanimation;
	UnstoppableAnimation unstoppableanimation;
	vector<IAnimationRenderer*> comboanimations;
	std::queue<IAnimationRenderer*> aniqueue;
	TimePoint roundfinishtime;
	TimePoint throwfinishtime;
	TimePoint anistarttime;
	bool showinganimations;
	bool scoringenabled;
	bool throwcomboscored;
	bool easycombos;
	int combocount;

	// Timing
	int slidetimeout;
	int roundtimeout;

	// Methods
	void UpdateDisplay();
	void PlayShotSound();
	void KillShotSounds();
	void CheckComboAchievement();

public:

	PlayingState(GameStateMachine* _statemachine);

	// Methods
	virtual void Enter() override final;
	virtual void Leave() override final;
	virtual void Update() override final;
	virtual bool HandleMessage(const IOModule_IOMessage& msg) override final;
};
