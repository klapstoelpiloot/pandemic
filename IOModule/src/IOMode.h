#pragma once
#include "StateMachine.h"
#include "IdleState.h"
#include "NormalState.h"
#include "CalibrationState.h"
#include "SerialLink.h"

class IOMode : virtual public StateMachine
{
private:

	// Serial connection
	SerialLink link;

	// States
	IdleState idlestate;
	NormalState normalstate;
	CalibrationState calibratestate;

	// Board LED
	LED boardled;
	String morsecode;
	String nextmorsecode;
	int codeposition;
	unsigned long lednexttime;

public:

	IOMode();

	// Access
	SerialLink& GetLink() { return link; }
	IdleState* GetIdleState() { return &idlestate; }
	NormalState* GetNormalState() { return &normalstate; }
	CalibrationState* GetCalibrateState() { return &calibratestate; }

	// Methods
	virtual void Update() override;
	void FlashDebugCode(String morse);
};

