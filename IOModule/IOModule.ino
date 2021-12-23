#include "src/Defines.h"
#include "src/IOMode.h"
#include "src/CRC32.h"

// Statemachine
IOMode mode;

// Initialization
void setup()
{
	CRC32::Initialize();

	// Open serial port
	Serial.begin(SERIAL_BAUD_RATE);

	// Begin in idle state
	mode.ChangeState(mode.GetIdleState());
}

// Main loop
void loop()
{
	mode.Update();
}
