#include "RoundData.h"

// Constructor
RoundData::RoundData() :
	index(0),
	isbonus(false),
	startpucks(0),
	pucksthrown(0),
	pucksremoved(0),
	gatepucks { 0, 0, 0, 0 },
	speeds { 0 }
{
}

int RoundData::PucksOnTable() const
{
	int scoredpucks = 0;
	for(int i = 0; i < GAME_GATES; i++)
		scoredpucks += gatepucks[i];
	return pucksthrown - scoredpucks - pucksremoved;
}
