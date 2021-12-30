#include "FinishedState.h"
#include "GameStateMachine.h"
#include "MenuStateMachine.h"
#include "Main.h"

#define BUTTON_FLASH_INTERVAL	400
#define GAME_OVER_TIME			3000
#define TOTAL_SCORE_TIME		2100
#define HIGHSCORE_TIME			1900

FinishedState::FinishedState(GameStateMachine* _statemachine) :
	statemachine(_statemachine),
	step(FinishedStep::GameOver),
	flashbuttonson(false),
	ishighscore(false)
{
}

void FinishedState::Enter()
{
	GameData& gd = statemachine->GetData();

	// TODO: Temporary fix. Remove when renderers are fixed so that menu can stay open.
	Main::GetMenu().Hide();

	// Setup rendering
	renderer.Clear();
	Main::GetGraphics().ClearRenderers();
	Main::GetGraphics().AddRenderer(&statemachine->GetBackground());
	Main::GetGraphics().AddRenderer(&renderer);
	Main::GetGraphics().AddRenderer(&screendissolve);
	Main::GetGraphics().AddRenderer(&statemachine->GetScreenMelt());

	Main::GetButtons().SetAllGameLEDs(false, false, false, true);
	flashbuttonson = false;

	step = FinishedStep::GameOver;
	renderer.ShowGameOver();
	nextsteptime = Clock::now() + ch::milliseconds(GAME_OVER_TIME);

	// (Re)load the highscores to make sure we have the right data for today
	Main::GetScores().Load(gd.GetType());

	// Complete the game data, making the result available
	gd.Finish();

	// Is this score highscore worthy?
	ScoreRecord result = gd.GetResult();
	ishighscore = (Main::GetScores().IsWorthyHighscore(result.score) ||
	               Main::GetScores().IsWorthyPeriodscore(result.score)) &&
	              !gd.IsCheated();

	// Play music that matches with the result
	if(ishighscore)
		Main::GetResources().GetMusic("inter_long.mp3").Play();
	else
		Main::GetResources().GetMusic("inter_short.mp3").Play();
}

void FinishedState::Leave()
{
	Main::GetButtons().SetAllGameLEDsOff();
}

void FinishedState::Update()
{
	switch(step)
	{
		case FinishedStep::GameOver:
			if(statemachine->GetScreenMelt().IsFinished() && (Clock::now() > nextsteptime))
				BeginShowTotalScore();
			break;

		case FinishedStep::ShowTotalScore:
			if(Clock::now() > nextsteptime)
				BeginHighScoreTextIfWorthy();
			break;

		case FinishedStep::HighscoreText:
			if(Clock::now() > nextsteptime)
				statemachine->ChangeState(statemachine->GetEnterNameState());
			break;

		// The finished state is only reached when there is no top-score
		case FinishedStep::Finished:

			// When the music finishes, continue to the highscores
			if(!Main::GetResources().GetMusic("inter_short.mp3").IsPlaying())
			{
				GameData& gd = statemachine->GetData();
				statemachine->AllowTitleMusicRestart();
				statemachine->GetHighscoreState()->SetGameType(gd.GetType());
				statemachine->ChangeState(statemachine->GetHighscoreState());
			}
			break;
	}
}

bool FinishedState::HandleMessage(const IOModule_IOMessage& msg)
{
	switch(msg.which_Content)
	{
		case IOModule_IOMessage_CancelButtonPressed_tag:
			Main::GetMenu().Show();
			return true;
	}

	return false;
}

void FinishedState::BeginShowTotalScore()
{
	GameData& gd = statemachine->GetData();
	step = FinishedStep::ShowTotalScore;
	renderer.ShowTotalScore(gd.CalculateScore());
	nextsteptime = Clock::now() + ch::milliseconds(TOTAL_SCORE_TIME);
}

void FinishedState::BeginHighScoreTextIfWorthy()
{
	GameData& gd = statemachine->GetData();
	ScoreRecord result = gd.GetResult();
	if(!gd.IsCheated())
	{
		if(Main::GetScores().IsWorthyHighscore(result.score))
		{
			step = FinishedStep::HighscoreText;
			renderer.ShowHighscore(false);
			nextsteptime = Clock::now() + ch::milliseconds(HIGHSCORE_TIME);
		}
		else if(Main::GetScores().IsWorthyPeriodscore(result.score))
		{
			step = FinishedStep::HighscoreText;
			renderer.ShowHighscore(true);
			nextsteptime = Clock::now() + ch::milliseconds(HIGHSCORE_TIME);
		}
		else
		{
			step = FinishedStep::Finished;
		}
	}
	else
	{
		step = FinishedStep::Finished;
	}
}
