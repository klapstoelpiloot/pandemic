#pragma once
#include "Rect.h"

struct FontChar
{
	Rect imgrect;		// Character position in source image
	Point offset;		// Offset for the character output
	int advance;		// How much to advance the X position

	// Constructor
	FontChar() { advance = 0; }
};
