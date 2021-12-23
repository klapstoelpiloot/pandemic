#include "HighscoreManager.h"
#include "File.h"
#include <time.h>

#define FILE_SEPARATOR		"-"
#define HIGHSCORES_PREFIX	"high-"
#define DAYSCORES_PREFIX	"day-"
#define FILE_EXT			".toml"

HighscoreManager::HighscoreManager(const Configuration& config) :
	scorespath(config.GetString("General.HighscoresPath", "")),
	gametype(GameType::Casual),
	highscores(static_cast<size_t>(config.GetInt("General.MaxHighscores", 10))),
	dayscores(static_cast<size_t>(config.GetInt("General.MaxDayscores", 10)))
{
}

void HighscoreManager::Load(GameType _gametype)
{
	gametype = _gametype;

	// Load highest scores
	String highscoresfile = GetHighscoreFilename();
	if(File::FileExists(highscoresfile))
		highscores.Load(highscoresfile);

	// Load day scores
	String dayscoresfile = GetDayscoreFilename();
	if(File::FileExists(dayscoresfile))
		dayscores.Load(dayscoresfile);
}

bool HighscoreManager::Insert(const ScoreRecord& score)
{
	bool result = false;

	if(highscores.Insert(score))
	{
		String highscoresfile = GetHighscoreFilename();
		highscores.Save(highscoresfile);
		result = true;
	}

	if(dayscores.Insert(score))
	{
		String dayscoresfile = GetDayscoreFilename();
		dayscores.Save(dayscoresfile);
		result = true;
	}

	return result;
}

String HighscoreManager::GetHighscoreFilename() const
{
	return File::CombinePath(scorespath, HIGHSCORES_PREFIX + String::From(static_cast<int>(gametype)) + FILE_EXT);;
}

String HighscoreManager::GetDayscoreFilename() const
{
	// For our day scores, we want the first few hours of the night to be part of the same day
	// before that, so that we use the same scores list on a long evening of playing games.
	// So, subtract a few hours from the current time and use that to determine the current date.
	TimePoint tp = Clock::now() - ch::hours(4);
	
	// Convert to a string which we can use in a filename
	time_t tt = TimePoint::clock::to_time_t(tp);
	tm ttparts;
	localtime_r(&tt, &ttparts);
	String date = String::From(ttparts.tm_year + 1900) + FILE_SEPARATOR + String::From(ttparts.tm_mon + 1) + FILE_SEPARATOR + String::From(ttparts.tm_mday);

	return File::CombinePath(scorespath, DAYSCORES_PREFIX + String::From(static_cast<int>(gametype)) + FILE_SEPARATOR + date + FILE_EXT);
}
