#pragma once
#include "Text.h"

class TextFlasher
{
private:
	
	TimePoint starttime;

public:

	// Constructor
	TextFlasher();

	void Begin();
	void Draw(Canvas& canvas, const Text& text, Point pos, int borderwidth);
};
