#include "CalibrationState.h"
#include "IOMode.h"

CalibrationState::CalibrationState(IOMode* mode) :
	mode(mode)
{
}

void CalibrationState::Enter()
{
	sensormonitor[0].Init(PIN_FRONTGATE1);
	sensormonitor[1].Init(PIN_FRONTGATE2);
	sensormonitor[2].Init(PIN_FRONTGATE3);
	sensormonitor[3].Init(PIN_FRONTGATE4);
	sensormonitor[4].Init(PIN_BACKGATE1);
	sensormonitor[5].Init(PIN_BACKGATE2);
	sensormonitor[6].Init(PIN_BACKGATE3);
	sensormonitor[7].Init(PIN_BACKGATE4);
	sensormonitor[8].Init(PIN_BUTTONLEFT);
	sensormonitor[9].Init(PIN_BUTTONRIGHT);
	sensormonitor[10].Init(PIN_BUTTONACCEPT);
	sensormonitor[11].Init(PIN_BUTTONCANCEL);
	sensormonitor[12].Init(PIN_BRIDGE1);
	sensormonitor[13].Init(PIN_BRIDGE2);
	buttonleft.Init(PIN_BUTTONLEFT, PIN_LEDLEFT);
	buttonright.Init(PIN_BUTTONRIGHT, PIN_LEDRIGHT);
	buttonaccept.Init(PIN_BUTTONACCEPT, PIN_LEDACCEPT);
	buttoncancel.Init(PIN_BUTTONCANCEL, PIN_LEDCANCEL);

	// Send all sensor states when entering this state
	for(int i = 0; i < NUM_SENSORS; i++)
		isdirty[i] = true;
}

void CalibrationState::Leave()
{
	buttonleft.SetLED(LOW);
	buttonright.SetLED(LOW);
	buttonaccept.SetLED(LOW);
	buttoncancel.SetLED(LOW);
}

void CalibrationState::Update()
{
	// If a sensor changed, mark it as dirty (for sending)
	for(int i = 0; i < NUM_SENSORS; i++)
	{
		if(sensormonitor[i].HasChanged())
			isdirty[i] = true;
	}

	TransmitDirtySensorStates();
}

void CalibrationState::HandleMessage(const IOModule_PiMessage& msg)
{
}

void CalibrationState::TransmitDirtySensorStates()
{
	for(int i = 0; i < NUM_SENSORS; i++)
	{
		if(isdirty[i])
		{
			PinStatus state = sensormonitor[i].GetState();
			isdirty[i] = !mode->GetLink().SendSensorState(i, (state == PinStatus::HIGH));
		}
	}
}
