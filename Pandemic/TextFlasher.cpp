#include "TextFlasher.h"

#define FADE_DURATION	200

TextFlasher::TextFlasher()
{
}

void TextFlasher::Begin(int duration, easing::enumerated easing)
{
	starttime = Clock::now();
	laststeptime = Clock::now();
	progress = tweeny::from(255).to(0).during(duration).via(easing);
}

void TextFlasher::Draw(Canvas& canvas, const Text& text, Point pos, int borderwidth)
{
	TimePoint t = Clock::now();
	if(!ch::IsTimeSet(starttime) || ((t - starttime) > ch::milliseconds(progress.duration())))
		return;

	int dt = static_cast<int>(ch::ToMilliseconds(t - laststeptime));
	laststeptime = t;
	int a = progress.step(dt);
	Color c1 = Color(255, 255, 255, static_cast<byte>(a));
	Color c2 = Color(255, 255, 255, static_cast<byte>(a / 2));
	text.DrawOutlineBlend(canvas, pos, borderwidth, c2);
	text.DrawAdd(canvas, pos, c1);
}
