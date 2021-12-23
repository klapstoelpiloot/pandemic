#pragma once

// Pins
#define PIN_FRONTGATE1	2
#define PIN_FRONTGATE2	20
#define PIN_FRONTGATE3	4
#define PIN_FRONTGATE4	18
#define PIN_BACKGATE1	16
#define PIN_BACKGATE2	7
#define PIN_BACKGATE3	8
#define PIN_BACKGATE4	14
#define PIN_BRIDGE1		12
#define PIN_BRIDGE2		11
#define PIN_UNUSED1		10
#define PIN_BUTTONLEFT	21
#define PIN_BUTTONRIGHT	19
#define PIN_BUTTONACCEPT	17
#define PIN_BUTTONCANCEL	15
#define PIN_LEDLEFT		3
#define PIN_LEDRIGHT	5
#define PIN_LEDACCEPT	6
#define PIN_LEDCANCEL	9

//Number of sensors to monitor in calibration mode
#define NUM_SENSORS		14

// Communication speed over serial line
// This must match on both sides
#define SERIAL_BAUD_RATE	9600

// Receive buffer size
#define SERIAL_BUFFER_SIZE	256

// Keep-alive timeout in milliseconds
#define KEEP_ALIVE_TIMEOUT	(1000 * 3)

// Bridge sensor distance in millimeters
#define BRIDGE_SENSOR_DISTANCE	103
