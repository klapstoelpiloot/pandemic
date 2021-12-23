#pragma once
#include "Defines.h"
#include "IState.h"
#include "PollMonitor.h"
#include "LEDButton.h"

class IOMode;

class CalibrationState : virtual public IState
{
private:

	IOMode* mode;

	// Polling monitors for calibration
	PollMonitor sensormonitor[NUM_SENSORS];
	bool isdirty[NUM_SENSORS];

	// Buttons
	LEDButton buttonleft;
	LEDButton buttonright;
	LEDButton buttonaccept;
	LEDButton buttoncancel;

	// Methods
	void TransmitDirtySensorStates();

public:

	CalibrationState(IOMode* mode);

	// Methods
	virtual void Enter() override;
	virtual void Leave() override;
	virtual void Update() override;
	virtual void HandleMessage(const IOModule_PiMessage& msg) override;
};
