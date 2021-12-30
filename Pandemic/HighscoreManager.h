#pragma once
#include "Configuration.h"
#include "ScoresTable.h"
#include "GameDesc.h"
#include "ScoresPeriod.h"

class HighscoreManager
{
private:

	// Members
	String scorespath;
	GameType gametype;
	ScoresPeriod interval;
	ScoresTable highscores;
	ScoresTable periodscores;

	// Methods
	String GetHighscoreFilename() const;
	String GetPeriodscoreFilename() const;

public:

	HighscoreManager(const Configuration& config);

	ScoresPeriod GetPeriodScoresInterval() const { return interval; }
	const ScoresTable& GetHighscores() const { return highscores; }
	const ScoresTable& GetPeriodscores() const { return periodscores; }

	void Load(GameType _gametype);
	GameType GetCurrentGameType() const { return gametype; }
	bool IsWorthyHighscore(int score) const { return highscores.IsWorthy(score); }
	bool IsWorthyPeriodscore(int score) const { return periodscores.IsWorthy(score); }
	bool Insert(const ScoreRecord& score);
};

