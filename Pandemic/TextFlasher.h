#pragma once
#include "Text.h"

class TextFlasher
{
private:
	
	TimePoint starttime;
	TimePoint laststeptime;
	TweenFloat progress;

public:

	// Constructor
	TextFlasher();

	void Begin(int duration = 400, easing::enumerated easing = easing::enumerated::exponentialOut);
	void Draw(Canvas& canvas, const Text& text, Point pos, int borderwidth);
};
