#pragma once
#include "Defines.h"
#include "IState.h"
#include "LEDButton.h"

class IOMode;

class IdleState : virtual public IState
{
private:

	IOMode* mode;
	unsigned long flashtime = 0;
	bool ledstate;
	bool flashledstate;
	LEDButton buttonleft;
	LEDButton buttoncancel;

public:

	IdleState(IOMode* mode);

	// Methods
	virtual void Enter() override;
	virtual void Leave() override;
	virtual void Update() override;
	virtual void HandleMessage(const IOModule_PiMessage& msg) override;
};
