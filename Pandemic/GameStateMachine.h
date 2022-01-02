#pragma once
#include "Tools.h"
#include "StateMachine.h"
#include "IMessageHandler.h"
#include "GameData.h"
#include "TitleState.h"
#include "SystemTestState.h"
#include "PlayingState.h"
#include "FinishedState.h"
#include "IntermissionState.h"
#include "RemovePuckState.h"
#include "ScreenMeltRenderer.h"
#include "EnterNameState.h"
#include "HighscoreState.h"
#include "GameBackgroundRenderer.h"
#include "Sound.h"

class GameStateMachine final : public virtual StateMachine, virtual public IMessageHandler
{
private:

	// This stores the game information
	GameData* data;

	// States
	TitleState titlestate;
	PlayingState playingstate;
	FinishedState finishedstate;
	IntermissionState intermissionstate;
	SystemTestState systemteststate;
	RemovePuckState removepuckstate;
	EnterNameState enternamestate;
	HighscoreState highscorestate;

	// Stuff that transcends some states
	GameBackgroundRenderer background;
	ScreenMeltRenderer screenmelt;
	Sound titlemusic;
	bool titlemusicchecked;
	bool titlemusicrepeat;

public:

	// Constructor/destructor
	GameStateMachine();
	virtual ~GameStateMachine();

	// States
	TitleState* GetTitleState() { return &titlestate; }
	PlayingState* GetPlayingState() { return &playingstate; }
	FinishedState* GetFinishedState() { return &finishedstate; }
	IntermissionState* GetIntermissionState() { return &intermissionstate; }
	SystemTestState* GetSystemTestState() { return &systemteststate; }
	RemovePuckState* GetRemovePuckState() { return &removepuckstate; }
	EnterNameState* GetEnterNameState() { return &enternamestate; }
	HighscoreState* GetHighscoreState() { return &highscorestate; }

	// Methods
	virtual bool HandleMessage(const IOModule_IOMessage& msg) override;
	inline bool HasData() const { return (data != nullptr); }
	void CreateNewGame(GameType type);
	void EndGame();
	inline GameData& GetData() { return *data; }
	inline const GameData& GetData() const { return *data; }
	inline GameBackgroundRenderer& GetBackground() { return background; }
	inline ScreenMeltRenderer& GetScreenMelt() { return screenmelt; }
	void AllowTitleMusicRestart();
	void PlayOrRepeatTitleMusic();
	void StopTitleMusic();
};
