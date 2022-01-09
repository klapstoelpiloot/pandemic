#pragma once
#include "Text.h"

class TextShine
{
private:

	TimePoint lasttime;
	TweenInt offset;
	Canvas tempcanvas;

	// Methods
	void DrawShine(Canvas& canvas, Rect tr);

public:

	// Constructor
	TextShine();

	void Begin(Size textsize);
	void Draw(Canvas& canvas, const Text& text, int outline, Point pos);
	void Draw(Canvas& canvas, const IImage& image, Point pos);
};
