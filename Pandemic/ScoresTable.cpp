#include "ScoresTable.h"
#include "TomlParser.h"
#include "File.h"

ScoresTable::ScoresTable(size_t maxitems) :
	maxitems(maxitems)
{
}

void ScoresTable::Load(String filename)
{
	std::cout << "Loading highscores file " << filename << "..." << std::endl;

	TomlParser file;
	this->clear();
	file.LoadFile(filename);
	ptr<cpptoml::table_array> tables = file.GetTableArray("Scores");
	for(ptr<cpptoml::table>& t : *tables)
	{
		TomlParser record(t);
		ScoreRecord score;
		score.name = record.GetString("Name", "");
		score.date = TimePoint(ch::milliseconds(record.GetLong("Date", 0)));
		score.score = record.GetInt("Score", 0);
		this->push_back(score);
	}
}

void ScoresTable::Save(String filename) const
{
	std::cout << "Writing highscores file " << filename << "..." << std::endl;

	TomlParser file;
	ptr<cpptoml::table_array> tables = cpptoml::make_table_array();
	for(const ScoreRecord& score : *this)
	{
		TomlParser record;
		record.SetString("Name", score.name);
		record.SetLong("Date", ch::ToMilliseconds(score.date));
		record.SetInt("Score", score.score);
		tables->push_back(record.GetRoot());
	}
	file.SetTableArray("Scores", tables);
	file.WriteFile(filename);
}

bool ScoresTable::IsWorthy(int score) const
{
	// We don't want zeros in the highscores
	if(score <= 0)
		return false;

	// Go through the list to see if the given score is higher than any on the list.
	for(const ScoreRecord& record : *this)
	{
		if(score > record.score)
			return true;
	}

	// Not higher than any on the list.
	// Check if we can add at the bottom of the list...
	return (this->size() < maxitems);
}

bool ScoresTable::Insert(const ScoreRecord& newrecord)
{
	// Go through the list to see if the given score is higher than any on the list,
	// in which case we must insert before that item.
	for(size_t i = 0; i < this->size(); i++)
	{
		ScoreRecord& record = (*this)[i];
		if(newrecord.score > record.score)
		{
			// Insert before this item
			this->insert(this->begin() + i, newrecord);

			// If the list got bigger than allowed, then drop some off the list
			if(this->size() > maxitems)
				this->resize(maxitems);

			return true;
		}
	}

	if(this->size() < maxitems)
	{
		// Append at the end of the list
		this->push_back(newrecord);
		return true;
	}
	else
	{
		// There is no place for losers, now go away.
		return false;
	}
}
