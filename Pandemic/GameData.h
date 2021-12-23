#pragma once
#include "Tools.h"
#include "GameDefines.h"
#include "RoundData.h"
#include "GameDesc.h"
#include "ScoreRecord.h"

class GameData
{
private:

	// Type of game
	GameType type;

	// The rounds played. Last round is the active round.
	vector<RoundData> rounds;

	// When finished, this holds the achieved score.
	ScoreRecord result;

public:

	// Constructorage
	GameData(GameType type);

	// Methods
	GameType GetType() const { return type; }
	int NumRounds() const { return rounds.size(); }
	RoundData& CurrentRound() { return rounds.back(); }
	RoundData& GetRound(uint index);
	const RoundData& GetRound(uint index) const;
	RoundData& AddRound(uint index, int startpucks);
	int CalculateSets() const;
	int CalculateScore() const;
	void GetGatesNeededForSet(bool* gates, bool equalstate) const;
	int CalculateAllLostPucks() const;
	int CalculateNextRoundPucks(bool* isbonus) const;
	void Finish();
	const ScoreRecord& GetResult() const { return result; }
};
