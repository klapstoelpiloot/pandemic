#pragma once
#include "ScoreRecord.h"

class ScoresTable : public vector<ScoreRecord>
{
private:

	size_t maxitems;

public:

	ScoresTable(size_t maxitems);

	void Load(String filename);
	void Save(String filename) const;
	bool IsWorthy(int score) const;
	bool Insert(const ScoreRecord& newrecord);
};
