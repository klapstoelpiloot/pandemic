#include "HighscoreManager.h"
#include "File.h"
#include <time.h>

#define FILE_SEPARATOR		"-"
#define HIGHSCORES_PREFIX	"high-"
#define DAYSCORES_PREFIX	"day-"
#define MONTHSCORES_PREFIX	"month-"
#define YEARSCORES_PREFIX	"year-"
#define FILE_EXT			".toml"

HighscoreManager::HighscoreManager(const Configuration& config) :
	scorespath(config.GetString("General.HighscoresPath", "")),
	gametype(GameType::Casual),
	interval(static_cast<ScoresPeriod>(config.GetInt("General.PeriodicScoresInterval", 0))),
	highscores(static_cast<size_t>(config.GetInt("General.MaxHighScores", 10))),
	periodscores(static_cast<size_t>(config.GetInt("General.MaxPeriodicScores", 10)))
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
	String periodscoresfile = GetPeriodscoreFilename();
	if(File::FileExists(periodscoresfile))
		periodscores.Load(periodscoresfile);
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

	if(periodscores.Insert(score))
	{
		String periodscoresfile = GetPeriodscoreFilename();
		periodscores.Save(periodscoresfile);
		result = true;
	}

	return result;
}

String HighscoreManager::GetHighscoreFilename() const
{
	return File::CombinePath(scorespath, HIGHSCORES_PREFIX + String::From(static_cast<int>(gametype)) + FILE_EXT);;
}

String HighscoreManager::GetPeriodscoreFilename() const
{
	// For our periodic scores, we want the first few hours of the night to be part of the same day
	// before that, so that we use the same scores list on a long evening of playing games.
	// So, subtract a few hours from the current time and use that to determine the current date.
	TimePoint tp = Clock::now() - ch::hours(4);
	
	// Convert to a string which we can use in a filename
	time_t tt = TimePoint::clock::to_time_t(tp);
	tm ttparts;
	localtime_r(&tt, &ttparts);

	String strgt = String::From(static_cast<int>(gametype));
	String stryear = String::From(ttparts.tm_year + 1900);
	String strmonth = String::From(ttparts.tm_mon + 1);
	String strday = String::From(ttparts.tm_mday);

	String filename;
	switch(interval)
	{
		case ScoresPeriod::Daily:
			filename = DAYSCORES_PREFIX + strgt + FILE_SEPARATOR + stryear + FILE_SEPARATOR + strmonth + FILE_SEPARATOR + strday;
			break;

		case ScoresPeriod::Monthly:
			filename = MONTHSCORES_PREFIX + strgt + FILE_SEPARATOR + stryear + FILE_SEPARATOR + strmonth;
			break;

		case ScoresPeriod::Annually:
			filename = YEARSCORES_PREFIX + strgt + FILE_SEPARATOR + stryear;
			break;

		default:
			NOT_IMPLEMENTED;
	}

	return File::CombinePath(scorespath, filename + FILE_EXT);
}
