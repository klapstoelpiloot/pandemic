#include "NormalState.h"
#include "IOMode.h"

NormalState::NormalState(IOMode* mode) :
	mode(mode),
	hardmode(false),
	throwing(false),
	slideinprogress(false),
	starttime(0),
	slidetimeout(2000),
	pucklostchecktime(0),
	pucklosttimeout(500)
{
}

void NormalState::Enter()
{
	gate1.Init(PIN_FRONTGATE1, PIN_BACKGATE1, hardmode);
	gate2.Init(PIN_FRONTGATE2, PIN_BACKGATE2, hardmode);
	gate3.Init(PIN_FRONTGATE3, PIN_BACKGATE3, hardmode);
	gate4.Init(PIN_FRONTGATE4, PIN_BACKGATE4, hardmode);
	bridge.Init(PIN_BRIDGE1, PIN_BRIDGE2);
	buttonleft.Init(PIN_BUTTONLEFT, PIN_LEDLEFT);
	buttonright.Init(PIN_BUTTONRIGHT, PIN_LEDRIGHT);
	buttonaccept.Init(PIN_BUTTONACCEPT, PIN_LEDACCEPT);
	buttoncancel.Init(PIN_BUTTONCANCEL, PIN_LEDCANCEL);
	hardmode = false;
	throwing = false;
	slideinprogress = false;
	starttime = 0;
	pucklostchecktime = 0;
}

void NormalState::Leave()
{
	gate1.Term();
	gate2.Term();
	gate3.Term();
	gate4.Term();
	bridge.Term();
	buttonleft.SetLED(LOW);
	buttonright.SetLED(LOW);
	buttonaccept.SetLED(LOW);
	buttoncancel.SetLED(LOW);
}

void NormalState::Update()
{
	// Update bridge logic
	BridgePuckStatus prevstate = bridge.GetStatus();
	bridge.Update();

	// Detecting a puck going forward?
	if((prevstate == BridgePuckStatus::None) && (bridge.GetStatus() == BridgePuckStatus::ForwardInRange))
	{
		// (Re)start our timers
		starttime = millis();
		pucklostchecktime = starttime + pucklosttimeout;
	}
	else if((prevstate == BridgePuckStatus::None) && (bridge.GetStatus() == BridgePuckStatus::ReverseInRange))
	{
		pucklostchecktime = starttime + pucklosttimeout;
	}

	// Begin of a slide?
	if(!throwing && bridge.HasSlideStarted())
	{
		// Start watching the gates
		gate1.Enable();
		gate2.Enable();
		gate3.Enable();
		gate4.Enable();
		throwing = true;
		slideinprogress = true;

		// Notify the game that a puck is thrown
		mode->GetLink().SendStartSlide(bridge.GetSpeed());
	}

	// Timeout to check for a "lost puck"
	if((pucklostchecktime > 0) && (millis() > pucklostchecktime))
	{
		// Only when a puck was actually thrown then we remove one from the game
		if(slideinprogress)
		{
			if((bridge.GetStatus() == BridgePuckStatus::ForwardInRange) ||
			   (bridge.GetStatus() == BridgePuckStatus::ReverseInRange))
				mode->GetLink().SendRemovePuck();
		}

		ResetBridge();
	}

	// Forward going puck has cleared the sensors?
	if(bridge.GetStatus() == BridgePuckStatus::ForwardComplete)
	{
		// Puck sliding forward has completed.
		// This is a good moment to reset the bridge so that we can detect pucks coming back
		ResetBridge();
		throwing = false;
	}

	// Detect pucks at the gates until timeout
	if(starttime > 0)
	{
		HandleGateLogic(gate1, 1);
		HandleGateLogic(gate2, 2);
		HandleGateLogic(gate3, 3);
		HandleGateLogic(gate4, 4);
	}

	// Timeout for activity reached?
	if((starttime > 0) && (millis() > (starttime + slidetimeout)))
	{
		// If a puck has come back, then it needs to be removed
		if((bridge.GetStatus() == BridgePuckStatus::ForwardInRange) ||
			(bridge.GetStatus() == BridgePuckStatus::ReverseInRange))
			mode->GetLink().SendRemovePuck();

		// Everything should be stopped by now
		gate1.Disable();
		gate2.Disable();
		gate3.Disable();
		gate4.Disable();
		throwing = false;
		slideinprogress = false;
		starttime = 0;
		ResetBridge();
	}

	HandleButtonPresses();
}

void NormalState::HandleGateLogic(GateWatcher& gate, int index)
{
	if(gate.HasPuckPassed())
		mode->GetLink().SendGateScore(index);
}

void NormalState::ResetBridge()
{
	bridge.Reset();

	if(bridge.GetStatus() == BridgePuckStatus::Blocked)
	{
		mode->GetLink().SendSensorBlocked();
		pucklostchecktime = millis() + pucklosttimeout;
	}
	else
	{
		mode->GetLink().SendSensorCleared();
		pucklostchecktime = 0;
	}
}

void NormalState::HandleButtonPresses()
{
	if(buttonleft.IsPressed())
		mode->GetLink().SendLeftButtonPress();

	if(buttonright.IsPressed())
		mode->GetLink().SendRightButtonPress();

	if(buttonaccept.IsPressed())
		mode->GetLink().SendAcceptButtonPress();

	if(buttoncancel.IsPressed())
		mode->GetLink().SendCancelButtonPress();
}

void NormalState::HandleMessage(const IOModule_PiMessage& msg)
{
	switch(msg.which_Content)
	{
		// Reset states for a new round
		case IOModule_PiMessage_RoundReset_tag:
		{
			gate1.Reset();
			gate2.Reset();
			gate3.Reset();
			gate4.Reset();
			ResetBridge();
			slideinprogress = false;
			throwing = false;
			starttime = 0;
			break;
		}

		// Set the button LEDs
		case IOModule_PiMessage_SetButtonLights_tag:
		{
			const IOModule_SetButtonLightsMsg& lights = msg.Content.SetButtonLights;
			buttonleft.SetLED(lights.LeftButton ? HIGH : LOW);
			buttonright.SetLED(lights.RightButton ? HIGH : LOW);
			buttonaccept.SetLED(lights.AcceptButton ? HIGH : LOW);
			buttoncancel.SetLED(lights.CancelButton ? HIGH : LOW);
		}
	}
}
