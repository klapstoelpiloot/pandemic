#pragma once
#include "Text.h"

class TextShine
{
private:

	TimePoint lasttime;
	TweenInt offset;
	Canvas tempcanvas;

public:

	// Constructor
	TextShine();

	void Begin(Size textsize);
	void Draw(Canvas& canvas, const Text& text, Point pos);
};
