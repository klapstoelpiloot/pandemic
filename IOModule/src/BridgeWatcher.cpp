#include "BridgeWatcher.h"
#include "Defines.h"

BridgeWatcher::BridgeWatcher() :
	state(BridgePuckStatus::None),
	starttime(0),
	slidestarted(false),
	speed(0)
{
}

BridgeWatcher::~BridgeWatcher()
{
}

void BridgeWatcher::Init(int sensor1pin, int sensor2pin)
{
	sensor1.Init(sensor1pin, PinStatus::CHANGE);
	sensor2.Init(sensor2pin, PinStatus::CHANGE);
	Reset();
}

void BridgeWatcher::Term()
{
	sensor1.Term();
	sensor2.Term();
}

void BridgeWatcher::Reset()
{
	sensor1.Reset();
	sensor2.Reset();
	starttime = 0;
	speed = 0;
	slidestarted = false;

	if((sensor1.GetState() == PinStatus::LOW) || (sensor2.GetState() == PinStatus::LOW))
		state = BridgePuckStatus::Blocked;
	else
		state = BridgePuckStatus::None;
}

void BridgeWatcher::Update()
{
	if(state == BridgePuckStatus::None)
	{
		// No puck in our area.
		// When the first sensor is triggered, we can assume a puck is going in forward direction
		if(sensor1.IsTriggered() && (sensor1.GetState() == PinStatus::LOW))
		{
			state = BridgePuckStatus::ForwardInRange;
			starttime = sensor1.GetTime();
			sensor1.Reset();
		}
		else if(sensor2.IsTriggered() && (sensor2.GetState() == PinStatus::LOW))
		{
			state = BridgePuckStatus::ReverseInRange;
			sensor2.Reset();
		}
	}
	else if(state == BridgePuckStatus::ForwardInRange)
	{
		// A puck is moving forward...
		if(sensor2.IsTriggered() && (sensor2.GetState() == PinStatus::LOW))
		{
			// Puck reached the second sensor, so now we can calculate the speed and notify the software.
			// We calculate the speed in millimeters per second because that gives us decent accuracy in an integer number.
			unsigned long deltatime = sensor2.GetTime() - starttime;
			speed = static_cast<uint32_t>(round(static_cast<double>(BRIDGE_SENSOR_DISTANCE) / (static_cast<double>(deltatime) / 1000.0)));
			slidestarted = true;
			sensor2.Reset();
		}
		else if(slidestarted && sensor2.IsTriggered() && (sensor2.GetState() == PinStatus::HIGH))
		{
			// The second sensor change from low to high, so the puck has cleared the second sensor
			state = BridgePuckStatus::ForwardComplete;
			sensor2.Reset();
		}
	}
}
