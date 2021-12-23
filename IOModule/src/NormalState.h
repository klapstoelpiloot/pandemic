#pragma once
#include "Defines.h"
#include "IState.h"
#include "GateWatcher.h"
#include "BridgeWatcher.h"
#include "LEDButton.h"

class IOMode;

class NormalState : virtual public IState
{
private:

	IOMode* mode;

	// Gates
	GateWatcher gate1;
	GateWatcher gate2;
	GateWatcher gate3;
	GateWatcher gate4;

	// Bridge
	BridgeWatcher bridge;

	// Buttons
	LEDButton buttonleft;
	LEDButton buttonright;
	LEDButton buttonaccept;
	LEDButton buttoncancel;

	// Set to True to use official gate score rules
	bool hardmode;

	// This is set to True when a puck is being thrown (we know its direction and speed) but has not cleared the sensors yet.
	// After it has cleared the sensors this returns to False to allow for another throw.
	bool throwing;

	// This is set to True after a puck is thrown (see above) and returns to False when the slide timeout is reached.
	// The difference with (starttime > 0) is that the starttime is already set at the beginning of the throw
	// (when the first sensor is hit).
	bool slideinprogress;

	// The time at which a puck throw begins (hits first sensor).
	unsigned long starttime;

	// Timeout until we stop watching for gate scores and reverse pucks after the throw begins.
	unsigned long slidetimeout;

	// This is to re-check a blocked sensor and to determine when a puck is to be removed from the game.
	unsigned long pucklostchecktime;
	unsigned long pucklosttimeout;

	// Methods
	void HandleButtonPresses();
	void HandleGateLogic(GateWatcher& gate, int index);
	void ResetBridge();

public:

	NormalState(IOMode* mode);

	// Methods
	virtual void Enter() override;
	virtual void Leave() override;
	virtual void Update() override;
	virtual void HandleMessage(const IOModule_PiMessage& msg) override;
	void SetSlideTimeout(uint32_t milliseconds) { slidetimeout = milliseconds; };
	void SetPuckLostTimeout(uint32_t milliseconds) { pucklosttimeout = milliseconds; };
	void SetHardMode(bool sethardmode) { hardmode = sethardmode; }
};
