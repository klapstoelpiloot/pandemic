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

	// Total number of rounds to reach
	uint totalrounds;

	// Number of pucks to play
	uint totalpucks;

	// Number of bonus pucks to receive upon scoring all in the first round
	uint bonuspucks1;

	// Number of bonus pucks to receive upon scoring all in the second round
	uint bonuspucks2;

	// Number of points to receive for a completed set
	uint setpoints;

	// The rounds played. Last round is the active round.
	vector<RoundData> rounds;

	// When finished, this holds the achieved score.
	ScoreRecord result;

	// When cheating was detected, this is set to True
	bool cheated;

public:

	// Constructorage
	GameData(GameType type);

	// Methods
	GameType GetType() const { return type; }
	uint TotalRounds() const { return totalrounds; }
	uint TotalPucks() const { return totalpucks; }
	uint BonusPucks1() const { return bonuspucks1; }
	uint BonusPucks2() const { return bonuspucks2; }
	uint CurrentRoundNumber() const { return rounds.size(); }		// Zero before any rounds are started
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
	bool IsCheated() const { return cheated; }
	void SetIsCheated() { cheated = true; }
	bool IsLastRound() const { return CurrentRoundNumber() == TotalRounds(); }
};
