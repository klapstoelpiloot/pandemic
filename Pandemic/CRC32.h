#pragma once
#include <cstdint>

class CRC32
{
private:

	// The lookup table
	static uint32_t table[256];

	// The cumulative CRC
	uint32_t crc;

public:

	// This initializes the lookup table.
	// Needs only to be called once.
	static void Initialize();

	// Constructor, initializes a new CRC.
	CRC32();

	// Calculates the CRC for the given data.
	// This method works cumulatively with previous calls.
	uint32_t CCalculate(const unsigned char* data, unsigned long length);

	// Calculates the CRC for the given data.
	static inline uint32_t Calculate(const unsigned char* data, unsigned long length) { CRC32 c; return c.CCalculate(data, length); }
};
