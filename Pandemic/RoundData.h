#pragma once
#include "Tools.h"
#include "GameDefines.h"

struct RoundData
{
public:

	// The round number where this game is at (0 based)
	uint index;

	// True when this is a bonus round.
	// The pucks in this round do not count for a full house.
	bool isbonus;

	// Number of pucks available for this round.
	int startpucks;

	// Number of throws done in this round.
	int pucksthrown;

	// Number of pucks removed from the game.
	int pucksremoved;

	// The number of pucks that has entered the gates
	int gatepucks[GAME_GATES];

	// The speed at which pucks were thrown (in millimeters per second, 0 = none)
	uint speeds[GAME_PUCKS];

	// Constructor
	RoundData();
	
	// Methods
	int PucksOnTable() const;
};
