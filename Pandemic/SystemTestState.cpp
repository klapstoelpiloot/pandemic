#include "SystemTestState.h"
#include "MenuStateMachine.h"
#include "GameStateMachine.h"
#include "Main.h"

SystemTestState::SystemTestState(GameStateMachine* _statemachine) :
	statemachine(_statemachine),
	changesound(Main::GetResources().GetSound("select.wav")),
	acceptpressed(false),
	cancelpressed(false)
{
}

void SystemTestState::Enter()
{
	acceptpressed = false;
	cancelpressed = false;

	// TODO: Temporary fix. Remove when renderers are fixed so that menu can stay open.
	Main::GetMenu().Hide();

	// Setup renderer
	Main::GetGraphics().ClearRenderers();
	Main::GetGraphics().AddRenderer(&renderer);

	// Turn all button LEDs on
	Main::GetButtons().SetAllGameLEDsOn();

	// Set IO module in test mode
	Main::GetIO().SetTestMode();
}

void SystemTestState::Leave()
{
	// Set IO module in normal operation mode
	Main::GetIO().SetNormalMode();

	// Turn all button LEDs off
	Main::GetButtons().SetAllGameLEDsOff();
}

void SystemTestState::Update()
{
}

bool SystemTestState::HandleMessage(const IOModule_IOMessage& msg)
{
	if(msg.which_Content == IOModule_IOMessage_SensorState_tag)
	{
		bool high = msg.Content.SensorState.High;
		switch(msg.Content.SensorState.Sensor)
		{
			case IOModule_Sensor_Gate1Front: renderer.SetGate1Front(high); break;
			case IOModule_Sensor_Gate2Front: renderer.SetGate2Front(high); break;
			case IOModule_Sensor_Gate3Front: renderer.SetGate3Front(high); break;
			case IOModule_Sensor_Gate4Front: renderer.SetGate4Front(high); break;
			case IOModule_Sensor_Gate1Back: renderer.SetGate1Back(high); break;
			case IOModule_Sensor_Gate2Back: renderer.SetGate2Back(high); break;
			case IOModule_Sensor_Gate3Back: renderer.SetGate3Back(high); break;
			case IOModule_Sensor_Gate4Back: renderer.SetGate4Back(high); break;
			case IOModule_Sensor_LeftButton: renderer.SetLeftButton(!high); break;
			case IOModule_Sensor_RightButton: renderer.SetRightButton(!high); break;
			case IOModule_Sensor_AcceptButton: renderer.SetAcceptButton(!high); acceptpressed = !high; break;
			case IOModule_Sensor_CancelButton: renderer.SetCancelButton(!high); cancelpressed = !high; break;
			case IOModule_Sensor_Bridge1Sensor: renderer.SetBridge1Sensor(high); break;
			case IOModule_Sensor_Bridge2Sensor: renderer.SetBridge2Sensor(high); break;
		}

		// Count the number of sensor changes we receive
		renderer.CountInput();

		// Leave this mode when both accept and cancel buttons are pressed
		if(acceptpressed && cancelpressed)
			statemachine->ChangeState(statemachine->GetTitleState());

		return true;
	}
	else
	{
		return false;
	}
}
