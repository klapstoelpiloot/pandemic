#include <math.h>
#include "IncredibleAnimation.h"
#include "Main.h"
#include "glm/vec2.hpp"
#include "glm/geometric.hpp"

#define FLASH_DELAY				ch::milliseconds(1000)
#define MOVE_DURATION			2000
#define WAIT_DURATION			400

IncredibleAnimation::IncredibleAnimation(ParticleOverlayRenderer& particlesoverlay) :
	character
	{
		Text("I", Main::GetResources().BoldBitsLarge(), HorizontalAlign::Left, VerticalAlign::Bottom),
		Text("N", Main::GetResources().BoldBitsLarge(), HorizontalAlign::Left, VerticalAlign::Bottom),
		Text("C", Main::GetResources().BoldBitsLarge(), HorizontalAlign::Left, VerticalAlign::Bottom),
		Text("R", Main::GetResources().BoldBitsLarge(), HorizontalAlign::Left, VerticalAlign::Bottom),
		Text("E", Main::GetResources().BoldBitsLarge(), HorizontalAlign::Left, VerticalAlign::Bottom),
		Text("D", Main::GetResources().BoldBitsLarge(), HorizontalAlign::Left, VerticalAlign::Bottom),
		Text("I", Main::GetResources().BoldBitsLarge(), HorizontalAlign::Left, VerticalAlign::Bottom),
		Text("B", Main::GetResources().BoldBitsLarge(), HorizontalAlign::Left, VerticalAlign::Bottom),
		Text("L", Main::GetResources().BoldBitsLarge(), HorizontalAlign::Left, VerticalAlign::Bottom),
		Text("E", Main::GetResources().BoldBitsLarge(), HorizontalAlign::Left, VerticalAlign::Bottom)
	},
	texture(Main::GetResources().GetImage("yellow12s.dds"))
{
	Text word("INCREDIBLE", Main::GetResources().BoldBitsLarge(), HorizontalAlign::Center, VerticalAlign::Middle);
	Rect wordrect = word.GetTextRect(Point(68, 15));
	int x = wordrect.x;
	for(int i = 0; i < 10; i++)
	{
		charpos[i] = Point(x, wordrect.y);
		x += character[i].GetTextSize().width - 1;
	}
}

IncredibleAnimation::~IncredibleAnimation()
{
}

void IncredibleAnimation::Start()
{
	Main::GetResources().GetSound("incredible.wav").Play();
	starttime = Clock::now();
	laststeptime = Clock::now();
	flashtime = Clock::now() + FLASH_DELAY;
	offset = tweeny::from(-6).to(38).during(MOVE_DURATION).via(easing::bounceOut).wait(WAIT_DURATION);
}

void IncredibleAnimation::Render(Canvas& canvas)
{
	TimePoint t = Clock::now();
	if(!ch::IsTimeSet(starttime) || (t > (starttime + ch::milliseconds(offset.duration()))))
	{
		// Animation ended
		starttime = TimePoint();
		return;
	}

	// Advance time
	int dt = static_cast<int>(ch::ToMilliseconds(t - laststeptime));
	laststeptime = t;
	int textoffset = offset.step(dt);
	if(ch::IsTimeSet(flashtime) && (t >= flashtime))
	{
		flasher.Begin(500);
		flashtime = TimePoint();
	}

	Point to = Point(static_cast<int>(ch::ToMilliseconds(t - starttime)) / 32, static_cast<int>(ch::ToMilliseconds(t - starttime)) / 32);

	for(int i = 0; i < 10; i++)
		character[i].DrawOutlineMask(canvas, Point(charpos[i].x, textoffset), 2, BLACK);
	for(int i = 0; i < 10; i++)
		character[i].DrawTexturedMask(canvas, Point(charpos[i].x, textoffset), texture, to);
	for(int i = 0; i < 10; i++)
		flasher.Draw(canvas, character[i], Point(charpos[i].x, textoffset), 2);
}
