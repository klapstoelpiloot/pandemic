#include "TextFlasher.h"

#define FADE_DURATION	200

TextFlasher::TextFlasher()
{
}

void TextFlasher::Begin()
{
	starttime = Clock::now();
}

void TextFlasher::Draw(Canvas& canvas, const Text& text, Point pos, int borderwidth)
{
	if(!ch::IsTimeSet(starttime) || ((Clock::now() - starttime) > ch::milliseconds(FADE_DURATION)))
		return;

	int64 dt = ch::ToMilliseconds(Clock::now() - starttime);
	float a = 1.0f - (static_cast<float>(dt) / FADE_DURATION);
	Color c1 = Color(255, 255, 255, Color::ToByte(a));
	Color c2 = Color(255, 255, 255, Color::ToByte(a * 0.5f));
	text.DrawOutlineBlend(canvas, pos, borderwidth, c2);
	text.DrawAdd(canvas, pos, c1);
}
