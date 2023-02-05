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
	ScoresTable casual_highscores;
	ScoresTable pro_highscores;

	// Methods
	String GetFilename(GameType gametype) const;
	ScoresTable* GetTable(GameType gametype);

public:

	HighscoreManager(const Configuration& config);

	const ScoresTable& GetCasualHighscores() const { return casual_highscores; }
	const ScoresTable& GetProHighscores() const { return pro_highscores; }

	void Load();
	bool IsWorthy(GameType gametype, int score);
	bool Insert(GameType gametype, const ScoreRecord& score);
};

