#pragma once
#include "Defines.h"
#include "String.h"

struct ScoreRecord
{
	// Members
	String name;
	TimePoint date;
	int score;

	// Constructor
	ScoreRecord() : score(0) { }
};
