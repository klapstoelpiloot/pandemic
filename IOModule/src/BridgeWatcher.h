#pragma once
#include "Arduino.h"
#include "DigitalTrigger.h"

enum class BridgePuckStatus
{
	// No puck has been detected
	None = 0,

	// A puck going forward (from sensor 1 to sensor 2) is in range of the sensors.
	// This state begins when a puck is first detected at sensor 1 and continues until
	// the puck clears the second sensor. From this moment it would be wise to start a
	// timer and only call Reset when this status remains for at least a few hundered
	// milliseconds.
	ForwardInRange,

	// A puck was detected going forward, but has already cleared the sensors.
	// This would be a good moment to call Reset and wait for another status change.
	ForwardComplete,

	// A puck is going backwards (from sensor 2 to sensor 1) in range of the sensors.
	// This state begins when a puck is first detected at sensor 2 and continues
	// until Reset is called. Do not call Reset immediately, as it may trigger sensor 1
	// which would cause the state to go to ForwardInRange. Call Reset after the throw
	// timeout to make sure all pucks have come to a rest.
	ReverseInRange,

	// At least one of the sensors was blocked when Reset was called.
	Blocked
};

class BridgeWatcher
{
private:

	// Sensors
	DigitalTrigger sensor1;
	DigitalTrigger sensor2;

	// Puck tracking
	BridgePuckStatus state;
	unsigned long starttime;
	bool slidestarted;
	uint32_t speed;

public:

	BridgeWatcher();
	virtual ~BridgeWatcher();

	// Initialization/termination
	void Init(int sensor1pin, int sensor2pin);
	void Term();

	// Access
	BridgePuckStatus GetStatus() const { return state; }

	// This returns True when the status is ForwardInRange and the puck reached the second sensor.
	// At this moment the speed is known and we know the puck is moving towards the gates.
	// However, if the state doesn't change to ForwardComplete soon, the puck may have stopped
	// at the second sensor. Speed is in millimeters per second.
	bool HasSlideStarted() const { return slidestarted; }
	uint32_t GetSpeed() const { return speed; }

	// Regular updates
	void Update();

	// Resets all counters
	void Reset();
};
