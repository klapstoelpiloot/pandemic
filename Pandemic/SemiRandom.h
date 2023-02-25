#pragma once
#include "Tools.h"

// This class is NOT thread-safe
class SemiRandom
{
private:

	byte* nextbyte;

public:

	SemiRandom(int seed = 0);

	void Reset(int seed);
	byte GetByte();
};
