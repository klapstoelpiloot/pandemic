#include <math.h>
#include "IncredibleAnimation.h"
#include "Main.h"
#include "glm/vec2.hpp"
#include "glm/geometric.hpp"

#define FLASH_DELAY				ch::milliseconds(300)
#define BOUNCE_DURATION			1600
#define OUT_DURATION			800

IncredibleAnimation::IncredibleAnimation(ParticleOverlayRenderer& particlesoverlay) :
	text("INCREDIBLE", Main::GetResources().BoldBitsLarge(), HorizontalAlign::Center, VerticalAlign::Bottom, -1),
	texture(Main::GetResources().GetImage("yellow12s.dds"))
{
}

IncredibleAnimation::~IncredibleAnimation()
{
}

void IncredibleAnimation::Start()
{
	Main::GetResources().GetSound("incredible.wav").Play();
	starttime = Clock::now();
	laststeptime = Clock::now();
	flashtime = TimePoint();
	shinetime = TimePoint();
	offset = tweeny::from(-5).to(32).during(BOUNCE_DURATION).via(easing::bounceOut).to(60).during(OUT_DURATION).via(easing::backIn);
	offset.step(200);
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

	if((textoffset == 20) && (!ch::IsTimeSet(shinetime) || (t > shinetime)))
	{
		shine.Begin(text.GetTextSize());
		shinetime = t + FLASH_DELAY;
	}

	if((textoffset == 31) && ((t - starttime < ch::milliseconds(BOUNCE_DURATION))) && (!ch::IsTimeSet(flashtime) || (t > flashtime)))
	{
		Main::GetResources().GetSound("bounce.wav").Play();
		flasher.Begin();
		flashtime = t + FLASH_DELAY;
	}

	Point to = Point(static_cast<int>(ch::ToMilliseconds(t - starttime)) / 32, static_cast<int>(ch::ToMilliseconds(t - starttime)) / 32);

	text.DrawOutlineMask(canvas, Point(65, textoffset), 2, BLACK);
	text.DrawTexturedMask(canvas, Point(65, textoffset), texture, to);
	shine.Draw(canvas, text, 2, Point(65, textoffset));
	flasher.Draw(canvas, text, Point(65, textoffset), 2);
}
