#include "PlayingState.h"
#include "GameStateMachine.h"
#include "Main.h"
#include "MenuStateMachine.h"

#define NUM_SHOT_SOUNDS				3
#define NUM_COMPLIMENT_SOUNDS		6
#define COMPLIMENT_RND_MIN			2
#define COMPLIMENT_RND_MAX			20
#define COMPLIMENT_SHORT_DELAY		300
#define COMPLIMENT_LONG_DELAY		900

PlayingState::PlayingState(GameStateMachine* _statemachine) :
	statemachine(_statemachine),
	scoringenabled(false),
	complimentchance(0),
	slidetimeout(0),
	roundtimeout(0)
{
}

void PlayingState::Enter()
{
	// TODO: Temporary fix. Remove when renderers are fixed so that menu can stay open.
	Main::GetMenu().Hide();

	scoringenabled = true;
	complimentchance = 0;
	complimenttime = TimePoint();
	throwfinishtime = TimePoint();
	roundfinishtime = TimePoint();
	slidetimeout = Main::GetConfig().GetInt("Game.SlideTimeoutMs", 2000);
	roundtimeout = Main::GetConfig().GetInt("Game.RoundTimeoutMs", 2000);

	// Setup rendering
	Main::GetGraphics().ClearRenderers();
	Main::GetGraphics().AddRenderer(&statemachine->GetBackground());
	Main::GetGraphics().AddRenderer(&gamerenderer);
	Main::GetGraphics().AddRenderer(&statemachine->GetScreenMelt());
	UpdateDisplay();

	Main::GetButtons().SetAllGameLEDs(false, false, false, true);
}

void PlayingState::Leave()
{
}

void PlayingState::Update()
{
	TimePoint now = Clock::now();
	GameData& gd = statemachine->GetData();

	// Time to compliment the player?
	if(ch::IsTimeSet(complimenttime) && (now >= complimenttime))
	{
		// Reset the timer
		complimenttime = TimePoint();
		complimentchance = 0;

		// Play a complimentary sound
		int rndindex = Random(1, NUM_COMPLIMENT_SOUNDS);
		Main::GetResources().GetSound("good" + String::From(rndindex) + ".wav").Play();
	}

	// Finishing a throw?
	if(ch::IsTimeSet(throwfinishtime) && (now >= throwfinishtime))
	{
		RoundData& rd = gd.CurrentRound();

		// Reset the timer
		throwfinishtime = TimePoint();

		// Increase compliment chance
		complimentchance++;

		// When this was our last puck for this round, don't count any more scores. This is to
		// prevent accedential scoring when the player is quick to pick up the pucks from the board.
		if(rd.pucksthrown == rd.startpucks)
			scoringenabled = false;
	}

	// Finishing a round?
	if(ch::IsTimeSet(roundfinishtime) && (now >= roundfinishtime))
	{
		// Round is finished
		GameData& gd = statemachine->GetData();
		if(gd.CalculateNextRoundPucks(nullptr) > 0)
		{
			// Next round
			statemachine->GetScreenMelt().Begin();
			statemachine->ChangeState(statemachine->GetIntermissionState());
		}
		else
		{
			// Fin
			statemachine->GetScreenMelt().Begin();
			statemachine->ChangeState(statemachine->GetFinishedState());
		}
		throwfinishtime = TimePoint();
		roundfinishtime = TimePoint();
	}
}

bool PlayingState::HandleMessage(const IOModule_IOMessage& msg)
{
	GameData& gd = statemachine->GetData();
	switch(msg.which_Content)
	{
		case IOModule_IOMessage_RemovePuck_tag:
		{
			// Remove the puck from the game
			RoundData& rd = gd.CurrentRound();
			rd.pucksremoved++;
			UpdateDisplay();

			Main::GetResources().GetSound("error.wav").Play();

			if(!ch::IsTimeSet(roundfinishtime))
			{
				statemachine->GetRemovePuckState()->SetSensorBlocked(false);
				statemachine->ChangeState(statemachine->GetRemovePuckState());
			}

			return true;
		}

		case IOModule_IOMessage_SensorBlocked_tag:
		{
			Main::GetResources().GetSound("error.wav").Play();

			if(!ch::IsTimeSet(roundfinishtime))
			{
				statemachine->GetRemovePuckState()->SetSensorBlocked(true);
				statemachine->ChangeState(statemachine->GetRemovePuckState());
			}

			return true;
		}

		case IOModule_IOMessage_StartSlide_tag:
		{
			// Here comes a puck!
			RoundData& rd = gd.CurrentRound();
			if(rd.pucksthrown < rd.startpucks)
			{
				// Output the speed in the console for fun
				uint32_t speed = msg.Content.StartSlide.Speed;
				double kmh = static_cast<double>(speed) * 0.0036;
				//std::cout << "Speed: " << kmh << " km/h" << std::endl;

				// Add the throw to the game data
				rd.speeds[rd.pucksthrown] = speed;
				rd.pucksthrown++;

				// Start our timing
				throwfinishtime = Clock::now() + ch::milliseconds(slidetimeout);
				if(rd.pucksthrown == rd.startpucks)
					roundfinishtime = Clock::now() + ch::milliseconds(roundtimeout);

				PlayShotSound();
				UpdateDisplay();
			}
			else
			{
				// Can't throw a puck when you have none left
				scoringenabled = false;
			}
			return true;
		}

		case IOModule_IOMessage_GateScore_tag:
		{
			if(scoringenabled)
			{
				int gateindex = msg.Content.GateScore.Gate - 1;

				// Status before the score
				bool gatesrequired[GAME_GATES];
				gd.GetGatesNeededForSet(gatesrequired, true);
				int prevsets = gd.CalculateSets();
				bool prevreq = gatesrequired[gateindex];

				// Apply the score
				RoundData& rd = gd.CurrentRound();
				rd.gatepucks[gateindex]++;

				// Check if cheating
				if(rd.PucksOnTable() < 0)
					gd.SetIsCheated();

				// Check result
				int newsets = gd.CalculateSets();
				KillShotSounds();
				Main::GetResources().GetSound("score.wav").Play();
				if(newsets != prevsets)
				{
					// A new set is completed!
					Main::GetResources().GetSound("set.wav").Play();
					gamerenderer.ShowSetAnimation(newsets);
					if(Random(COMPLIMENT_RND_MIN, COMPLIMENT_RND_MAX) < complimentchance)
						complimenttime = Clock::now() + ch::milliseconds(COMPLIMENT_LONG_DELAY);
				}
				else if(prevreq)
				{
					// Scored in a gate we needed
					gamerenderer.ScoreRequiredGate(gateindex);
					if(!ch::IsTimeSet(complimenttime) && (Random(COMPLIMENT_RND_MIN, COMPLIMENT_RND_MAX) < complimentchance))
						complimenttime = Clock::now() + ch::milliseconds(COMPLIMENT_SHORT_DELAY);
				}
				else
				{
					// Scored in a gate we don't need
					gamerenderer.ScoreGate(gateindex);
				}

				UpdateDisplay();
			}
			return true;
		}

		case IOModule_IOMessage_CancelButtonPressed_tag:
			Main::GetMenu().Show();
			return true;

		default:
			return false;
	}
}

void PlayingState::UpdateDisplay()
{
	GameData& gd = statemachine->GetData();
	RoundData& rd = gd.CurrentRound();
	bool gatesrequired[GAME_GATES];

	gamerenderer.SetRound(rd.index + 1);
	gamerenderer.SetPucks(rd.startpucks - rd.pucksthrown);
	gamerenderer.SetScore(gd.CalculateScore());
	gd.GetGatesNeededForSet(gatesrequired, true);
	gamerenderer.SetRequiredGates(gatesrequired);
}

void PlayingState::PlayShotSound()
{
	// Play random shot sound
	int rndindex = Random(1, NUM_SHOT_SOUNDS);
	Main::GetResources().GetSound("shot" + String::From(rndindex) + ".wav").Play();
}

void PlayingState::KillShotSounds()
{
	for(int i = 1; i <= NUM_SHOT_SOUNDS; i++)
		Main::GetResources().GetSound("shot" + String::From(i) + ".wav").Stop();
}
