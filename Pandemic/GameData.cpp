#include "GameData.h"
#include "Main.h"

constexpr int GATE_VALUES[] = { 2, 3, 4, 1 };

GameData::GameData(GameType type) :
	type(type),
	totalrounds(Main::GetConfig().GetInt("Game.Rounds", 0)),
	totalpucks(Main::GetConfig().GetInt("Game.Pucks", 0)),
	bonuspucks1(Main::GetConfig().GetInt("Game.BonusPucks1", 0)),
	bonuspucks2(Main::GetConfig().GetInt("Game.BonusPucks2", 0)),
	setpoints(Main::GetConfig().GetInt("Game.SetPoints", 0)),
	cheated(false)
{
}

RoundData& GameData::GetRound(uint index)
{
	REQUIRE(index < rounds.size());
	return rounds[index];
}

const RoundData& GameData::GetRound(uint index) const
{
	REQUIRE(index < rounds.size());
	return rounds[index];
}

RoundData& GameData::AddRound(uint index, int startpucks)
{
	rounds.push_back(RoundData());
	RoundData& rd = rounds[rounds.size() - 1];
	rd.index = index;
	rd.startpucks = startpucks;
	return rd;
}

int GameData::CalculateSets() const
{
	// Calculate total pucks in gates, excluding bonus rounds
	int gatepucks[GAME_GATES] = { 0 };
	for(uint r = 0; r < rounds.size(); r++)
	{
		if(!rounds[r].isbonus)
		{
			for(uint g = 0; g < GAME_GATES; g++)
				gatepucks[g] += rounds[r].gatepucks[g];
		}
	}

	// Find the lowest number of pucks in the gates.
	// This is the number of sets we have scored.
	int sets = INT32_MAX;
	for(uint g = 0; g < GAME_GATES; g++)
		sets = std::min(sets, gatepucks[g]);

	return sets;
}

int GameData::CalculateScore() const
{
	int sets = CalculateSets();

	// Calculate total pucks in gates, minus the sets
	int gatepucks[GAME_GATES] = { 0 };
	for(uint g = 0; g < GAME_GATES; g++)
	{
		for(uint r = 0; r < rounds.size(); r++)
			gatepucks[g] += rounds[r].gatepucks[g];

		gatepucks[g] -= sets;
	}

	// Count the basic points for the remaining pucks in each gate
	int basicpoints = 0;
	for(uint g = 0; g < GAME_GATES; g++)
		basicpoints += GATE_VALUES[g] * gatepucks[g];

	// Combo points (1 point per combo)
	int combopoints = 0;
	if(type == GameType::Casual)
	{
		for(uint r = 0; r < rounds.size(); r++)
			combopoints += rounds[r].combos;
	}

	return (sets * setpoints) + basicpoints + combopoints;
}

void GameData::GetGatesNeededForSet(bool* gates, bool equalstate) const
{
	int sets = CalculateSets();

	// Calculate total pucks in gates, minus the sets
	int totalgatepucks = 0;
	int gatepucks[GAME_GATES] = { 0 };
	for(uint g = 0; g < GAME_GATES; g++)
	{
		for(uint r = 0; r < rounds.size(); r++)
			gatepucks[g] += rounds[r].gatepucks[g];

		gatepucks[g] -= sets;
		totalgatepucks += gatepucks[g];
	}

	if(totalgatepucks > 0)
	{
		// If there are any gates with more than 0 pucks, then the gates
		// with 0 pucks in them are the minimum required for a set.
		for(uint g = 0; g < GAME_GATES; g++)
			gates[g] = (gatepucks[g] == 0);
	}
	else
	{
		// If there are equal amount of pucks in all gates, then
		// just fill the array with the equalstate parameter.
		for(uint g = 0; g < GAME_GATES; g++)
			gates[g] = equalstate;
	}
}

int GameData::CalculateAllLostPucks() const
{
	int total = 0;
	for(uint r = 0; r < rounds.size(); r++)
		total += rounds[r].pucksremoved;
	return total;
}

int GameData::CalculateNextRoundPucks(bool* isbonus) const
{
	// Presume this is not going to be a bonus round
	if(isbonus != nullptr)
		*isbonus = false;

	// The first round always stars with the same number of pucks
	if(rounds.size() == 0)
		return totalpucks;

	// In general, the pucks for the next round are all the pucks left on the table.
	// However, if all pucks were scored in the first round, the player gets 2 extra pucks for the second round and
	// when all pucks are scored in the second round, the player gets 1 extra puck for a third round.
	const RoundData& lastrd = rounds[rounds.size() - 1];
	int pucksleft = lastrd.PucksOnTable();
	int puckslost = CalculateAllLostPucks();

	// All scored in first round
	if((pucksleft == 0) && (puckslost == 0) && (lastrd.index == 0))
	{
		if(isbonus != nullptr) *isbonus = true;
		return bonuspucks1;
	}

	// All scored in second round
	if((pucksleft == 0) && (puckslost == 0) && (lastrd.index == 1))
	{
		if(isbonus != nullptr) *isbonus = true;
		return bonuspucks2;
	}

	// If this was the last round, then no pucks for next round
	if(lastrd.index == (totalrounds - 1))
		return 0;

	return pucksleft;
}

void GameData::Finish()
{
	result.date = Clock::now();
	result.score = CalculateScore();
}
