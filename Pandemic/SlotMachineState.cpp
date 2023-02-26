#include "SlotMachineState.h"
#include "GameStateMachine.h"
#include "Main.h"
#include "MenuStateMachine.h"

#define ROLL_START_DISTANCE		5
#define ROLL_DISTANCE_TIME		100

SlotMachineState::SlotMachineState(GameStateMachine* _statemachine) :
	statemachine(_statemachine),
	renderer(),
	isrolling(false),
	random(),
	wheelposition({ 0.0f, 0.0f, 0.0f })
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

	// Start with a random setup. Choose from 5 seeds.
	int seed = Random(0, 4);
	random.Reset(seed);
	for(int i = 0; i < NUM_WHEELS; i++)
	{
		wheelposition[i] = random.GetByte();
		renderer.SetWheelPosition(i, wheelposition[i], false);
	}

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
			if(wheelroll[i].progress() < 1.0f)
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
			isrolling = false;
	}
}

bool SlotMachineState::HandleMessage(const IOModule_IOMessage& msg)
{
	GameData& gd = statemachine->GetData();
	switch(msg.which_Content)
	{
		case IOModule_IOMessage_AcceptButtonPressed_tag:
			if(!isrolling)
				StartRoll();
			return true;

		case IOModule_IOMessage_LeftButtonPressed_tag:
			return true;

		case IOModule_IOMessage_RightButtonPressed_tag:
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
	float distance = ROLL_START_DISTANCE + static_cast<float>((500 + random.GetByte()) / 100);

	for(int i = 0; i < NUM_WHEELS; i++)
	{
		// For every wheel, cumulatively add some distance. We want them to stop from
		// left to right in sequence, so the right wheel makes the longest distance.
		distance += static_cast<float>((500 + random.GetByte()) / 100);

		// Setup the wheel movement
		wheelroll[i] = tweeny::from(wheelposition[i]).to(wheelposition[i] + ROLL_START_DISTANCE).during(ROLL_START_DISTANCE * ROLL_DISTANCE_TIME).via(easing::sinusoidalIn)
			.to(roundf(wheelposition[i] + distance)).during(static_cast<int>((distance - ROLL_START_DISTANCE) * ROLL_DISTANCE_TIME)).via(easing::sinusoidalOut);
	}

	isrolling = true;
}
