#include "PlayingState.h"
#include "GameStateMachine.h"
#include "Main.h"
#include "MenuStateMachine.h"

#define NUM_SHOT_SOUNDS				3
#define ANI_START_DELAY				ch::milliseconds(300)

PlayingState::PlayingState(GameStateMachine* _statemachine) :
	statemachine(_statemachine),
	hud(particlesoverlay),
	setanimation(particlesoverlay),
	greatshotanimation(particlesoverlay),
	hotshotanimation(particlesoverlay),
	megashotanimation(particlesoverlay),
	impressiveanimation(particlesoverlay),
	excellentanimation(particlesoverlay),
	incredibleanimation(particlesoverlay),
	unstoppableanimation(particlesoverlay),
	showinganimations(false),
	scoringenabled(false),
	throwcomboscored(false),
	easycombos(false),
	combocount(0),
	slidetimeout(0),
	roundtimeout(0)
{
	// Make the list of combo animations in order of higher achievements
	comboanimations.push_back(&greatshotanimation);
	comboanimations.push_back(&hotshotanimation);
	comboanimations.push_back(&megashotanimation);
	comboanimations.push_back(&impressiveanimation);
	comboanimations.push_back(&excellentanimation);
	comboanimations.push_back(&incredibleanimation);
	comboanimations.push_back(&unstoppableanimation);
}

void PlayingState::Enter()
{
	// TODO: Temporary fix. Remove when renderers are fixed so that menu can stay open.
	Main::GetMenu().Hide();

	showinganimations = false;
	scoringenabled = true;
	throwcomboscored = false;
	combocount = 0;
	while(!aniqueue.empty())
		aniqueue.pop();
	anistarttime = TimePoint();
	throwfinishtime = TimePoint();
	roundfinishtime = TimePoint();
	slidetimeout = Main::GetConfig().GetInt("Game.SlideTimeoutMs", 2000);
	roundtimeout = Main::GetConfig().GetInt("Game.RoundTimeoutMs", 2000);
	easycombos = Main::GetConfig().GetBool("Game.EasyCombos", false);
	hud.Show();

	// Setup rendering
	// Animations before the game renderer, so that particles are renderer on top
	Main::GetGraphics().ClearRenderers();
	Main::GetGraphics().AddRenderer(&statemachine->GetBackground());
	Main::GetGraphics().AddRenderer(&hud);
	for(auto ca : comboanimations)
		Main::GetGraphics().AddRenderer(ca);
	Main::GetGraphics().AddRenderer(&setanimation);
	Main::GetGraphics().AddRenderer(&particlesoverlay);
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

	// Time to show the animiation(s)?
	if(ch::IsTimeSet(anistarttime) && (now >= anistarttime))
	{
		anistarttime = TimePoint();
		if(!aniqueue.empty())
		{
			showinganimations = true;
			hud.Hide();
			aniqueue.front()->Start();
		}
	}

	// Animation done?
	if(showinganimations && aniqueue.front()->HasFinished())
	{
		aniqueue.pop();
		if(!aniqueue.empty())
		{
			// Play the next animation
			aniqueue.front()->Start();
		}
		else
		{
			// Done with the animations
			showinganimations = false;
			hud.Show();
		}
	}

	// Finishing a throw?
	if(ch::IsTimeSet(throwfinishtime) && (now >= throwfinishtime))
	{
		RoundData& rd = gd.CurrentRound();

		// Reset the timer
		throwfinishtime = TimePoint();

		// When this was our last puck for this round, don't count any more scores. This is to
		// prevent accedential scoring when the player is quick to pick up the pucks from the board.
		if(rd.pucksthrown == rd.startpucks)
			scoringenabled = false;

		// When no puck was scored during this throw, then reset the combo
		if(!throwcomboscored)
			combocount = 0;

		throwcomboscored = false;
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
				//double kmh = static_cast<double>(speed) * 0.0036;
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
					hud.ScoreRequiredGate(gateindex);
					CheckComboAchievement();

					// A new set is completed!
					hud.ScoreSet();
					setanimation.SetIndex(newsets);
					aniqueue.push(&setanimation);

					if(!showinganimations)
						anistarttime = Clock::now() + ANI_START_DELAY;
				}
				else if(prevreq)
				{
					// Scored in a gate we needed
					hud.ScoreRequiredGate(gateindex);
					CheckComboAchievement();
				}
				else
				{
					// Scored in a gate we don't need
					hud.ScoreGate(gateindex);

					if(easycombos)
						CheckComboAchievement();
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

	hud.SetRound(rd.index + 1);
	hud.SetPucks(rd.startpucks - rd.pucksthrown);
	hud.SetScore(gd.CalculateScore());
	gd.GetGatesNeededForSet(gatesrequired, true);
	hud.SetRequiredGates(gatesrequired);
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

void PlayingState::CheckComboAchievement()
{
	throwcomboscored = true;
	combocount++;
	if(combocount > 1)
	{
		int comboindex = combocount - 2;
		if(comboindex < static_cast<int>(comboanimations.size()))
			aniqueue.push(comboanimations[comboindex]);
		else
			aniqueue.push(comboanimations.back());

		if(!showinganimations)
			anistarttime = Clock::now() + ANI_START_DELAY;
	}
}
