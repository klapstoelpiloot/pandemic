#pragma once
#include "Configuration.h"
#include "ScoresTable.h"
#include "GameDesc.h"

class HighscoreManager
{
private:

	// Members
	String scorespath;
	GameType gametype;
	ScoresTable highscores;
	ScoresTable dayscores;

	// Methods
	String GetHighscoreFilename() const;
	String GetDayscoreFilename() const;

public:

	HighscoreManager(const Configuration& config);

	const ScoresTable& GetHighscores() const { return highscores; }
	const ScoresTable& GetDayscores() const { return dayscores; }

	void Load(GameType _gametype);
	GameType GetCurrentGameType() const { return gametype; }
	bool IsWorthyHighscore(int score) const { return highscores.IsWorthy(score); }
	bool IsWorthyDayscore(int score) const { return dayscores.IsWorthy(score); }
	bool Insert(const ScoreRecord& score);
};

