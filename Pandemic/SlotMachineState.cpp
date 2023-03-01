#include "SlotMachineState.h"
#include "GameStateMachine.h"
#include "Main.h"
#include "MenuStateMachine.h"

#define ROLL_START_DISTANCE			5
#define ROLL_DISTANCE_TIME			100
#define ROLL_MIN_DISTANCE			6
#define ROLL_STEP_DISTANCE			5
#define SELECTION_SHOW_DELAY		ch::milliseconds(500)

SlotMachineState::SlotMachineState(GameStateMachine* _statemachine) :
	statemachine(_statemachine),
	renderer(),
	isrolling(false),
	random(),
	wheelposition { 0.0f, 0.0f, 0.0f },
	wheelhold { false, false, false },
	selection(SlotMachineSelection::Spin)
{
}

void SlotMachineState::Enter()
{
	// TODO: Temporary fix. Remove when renderers are fixed so that menu can stay open.
	Main::GetMenu().Hide();

	// Setup rendering
	// Animations before the game renderer, so that particles are renderer on top
	Main::GetGraphics().ClearRenderers();
	Main::GetGraphics().AddRenderer(&statemachine->GetBackground());
	Main::GetGraphics().AddRenderer(&renderer);
	Main::GetGraphics().AddRenderer(&statemachine->GetScreenMelt());
	renderer.Start();

	// Start with a random setup. Choose from 3 seeds.
	int seed = Random(0, 2);
	random.Reset(seed);
	for(int i = 0; i < NUM_WHEELS; i++)
	{
		wheelposition[i] = random.GetByte();
		renderer.SetWheelPosition(i, wheelposition[i], false);
	}
	renderer.SetSelection(selection);

	Main::GetButtons().SetAllGameLEDs(false, false, true, true);
}

void SlotMachineState::Leave()
{
}

void SlotMachineState::Update()
{
	TimePoint t = Clock::now();
	GameData& gd = statemachine->GetData();
	int dt = static_cast<int>(ch::ToMilliseconds(t - laststeptime));
	laststeptime = t;

	if(isrolling)
	{
		bool allstopped = true;
		for(int i = 0; i < NUM_WHEELS; i++)
		{
			if(!wheelhold[i] && (wheelroll[i].progress() < 1.0f))
			{
				allstopped = false;
				float pos = wheelroll[i].step(dt);
				if(wheelroll[i].progress() >= 1.0f)
				{
					pos = roundf(wheelroll[i].seek(1.0f));
					wheelposition[i] = pos;

					// Stop wheel on renderer
					renderer.SetWheelPosition(i, pos, false);
				}
				else
				{
					// Update renderer
					renderer.SetWheelPosition(i, pos, true);
				}
			}
		}

		if(allstopped)
		{
			isrolling = false;
			selectionstarttime = t + SELECTION_SHOW_DELAY;
		}
	}

	// Time to make a selection?
	if(ch::IsTimeSet(selectionstarttime) && (selectionstarttime < t) && (selection == SlotMachineSelection::None))
	{
		selection = SlotMachineSelection::Spin;
		renderer.SetSelection(selection);
	}
}

bool SlotMachineState::HandleMessage(const IOModule_IOMessage& msg)
{
	GameData& gd = statemachine->GetData();
	switch(msg.which_Content)
	{
		case IOModule_IOMessage_AcceptButtonPressed_tag:
			if((selection >= SlotMachineSelection::Hold0) && (selection <= SlotMachineSelection::Hold2))
			{
				int index = selection - SlotMachineSelection::Hold0;
				wheelhold[index] = !wheelhold[index];
				renderer.SetHolds(wheelhold);
			}
			else if(selection == SlotMachineSelection::Spin)
			{
				StartRoll();
			}
			return true;

		case IOModule_IOMessage_LeftButtonPressed_tag:
			if(selection > SlotMachineSelection::Hold0)
			{
				selection = static_cast<SlotMachineSelection>(selection - 1);
				renderer.SetSelection(selection);
			}
			return true;

		case IOModule_IOMessage_RightButtonPressed_tag:
			if(selection < SlotMachineSelection::Spin)
			{
				selection = static_cast<SlotMachineSelection>(selection + 1);
				renderer.SetSelection(selection);
			}
			return true;

		case IOModule_IOMessage_CancelButtonPressed_tag:
			Main::GetMenu().Show();
			return true;

		default:
			return false;
	}
}

void SlotMachineState::StartRoll()
{
	// Minimum distance each wheel rolls
	float distance = ROLL_START_DISTANCE + static_cast<float>((ROLL_MIN_DISTANCE * 100 + random.GetByte()) / 100);

	for(int i = 0; i < NUM_WHEELS; i++)
	{
		// For every wheel, cumulatively add some distance. We want them to stop from
		// left to right in sequence, so the right wheel makes the longest distance.
		distance += static_cast<float>((ROLL_STEP_DISTANCE * 100 + random.GetByte() * 4) / 100);

		if(!wheelhold[i])
		{
			// Setup the wheel movement
			float endpoint = roundf(wheelposition[i] + distance);
			wheelroll[i] = tweeny::from(wheelposition[i])
				.to(wheelposition[i] + ROLL_START_DISTANCE).during(ROLL_START_DISTANCE * ROLL_DISTANCE_TIME).via(easing::sinusoidalIn)
				.to(endpoint + 0.05f).during(static_cast<int>((distance - ROLL_START_DISTANCE) * ROLL_DISTANCE_TIME)).via(easing::sinusoidalOut)
				.to(endpoint).during(100).via(easing::linear);
		}
	}

	selectionstarttime = TimePoint();
	selection = SlotMachineSelection::None;
	renderer.SetSelection(selection);
	isrolling = true;
}
