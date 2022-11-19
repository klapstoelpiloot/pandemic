#include "HighscoreManager.h"
#include "File.h"
#include <time.h>

#define FILE_SEPARATOR		"-"
#define HIGHSCORES_PREFIX	"high-"
#define FILE_EXT			".toml"

HighscoreManager::HighscoreManager(const Configuration& config) :
	scorespath(config.GetString("General.HighscoresPath", "")),
	casual_highscores(static_cast<size_t>(config.GetInt("General.MaxHighScores", 10))),
	pro_highscores(static_cast<size_t>(config.GetInt("General.MaxHighScores", 10)))
{
}

void HighscoreManager::Load()
{
	String highscoresfile = GetFilename(GameType::Casual);
	if(File::FileExists(highscoresfile))
		casual_highscores.Load(highscoresfile);

	highscoresfile = GetFilename(GameType::Pro);
	if(File::FileExists(highscoresfile))
		pro_highscores.Load(highscoresfile);
}

bool HighscoreManager::Insert(GameType gametype, const ScoreRecord& score)
{
	bool result = false;

	ScoresTable* table = GetTable(gametype);
	if(table->Insert(score))
	{
		String highscoresfile = GetFilename(gametype);
		table->Save(highscoresfile);
		result = true;
	}

	return result;
}

ScoresTable* HighscoreManager::GetTable(GameType gametype)
{
	switch(gametype)
	{
		case GameType::Casual: return &casual_highscores;
		case GameType::Pro: return &pro_highscores;
		default:
			NOT_IMPLEMENTED;
	}
}

String HighscoreManager::GetFilename(GameType gametype) const
{
	return File::CombinePath(scorespath, HIGHSCORES_PREFIX + String::From(static_cast<int>(gametype)) + FILE_EXT);;
}

bool HighscoreManager::IsWorthy(GameType gametype, int score)
{
	ScoresTable* table = GetTable(gametype);
	return table->IsWorthy(score);
}
