#include <math.h>
#include "GreatShotAnimation.h"
#include "Main.h"

GreatShotAnimation::GreatShotAnimation(ParticleOverlayRenderer& particlesoverlay) :
	text1("GREAT", Main::GetResources().BoldBitsLarge(), HorizontalAlign::Left, VerticalAlign::Middle, -1),
	text2("SHOT", Main::GetResources().BoldBitsLarge(), HorizontalAlign::Right, VerticalAlign::Middle, -1),
	texture(Main::GetResources().GetImage("gray22d.dds")),
	shinedone(false)
{
}

GreatShotAnimation::~GreatShotAnimation()
{
}

void GreatShotAnimation::Start()
{
	Main::GetResources().GetSound("greatshot.wav").Play();
	starttime = Clock::now();
	laststeptime = Clock::now();
	lastparticletime = Clock::now();
	shinedone = false;

	text1pos = tweeny::from(2, -16)
		.to(2, 15).during(500).via(easing::cubicOut)
		.wait(200)
		.to(2, 48).during(500).via(easing::cubicIn)
		.wait(100);

	text2pos = tweeny::from(127, 48)
		.to(127, 15).during(500).via(easing::cubicOut)
		.wait(200)
		.to(127, -16).during(500).via(easing::cubicIn)
		.wait(100);
}

void GreatShotAnimation::Render(Canvas& canvas)
{
	TimePoint t = Clock::now();
	if(!ch::IsTimeSet(starttime) || (t > (starttime + ch::milliseconds(text1pos.duration()))))
	{
		// Animation ended
		starttime = TimePoint();
		return;
	}

	if(!shinedone && (t > (starttime + ch::milliseconds(400))))
	{
		shine1.Begin(text1.GetTextSize());
		shine2.Begin(text2.GetTextSize());
		shinedone = true;
	}

	int dt = static_cast<int>(ch::ToMilliseconds(t - laststeptime));
	laststeptime = t;

	Point t1pos = text1pos.step(dt);
	Point t2pos = text2pos.step(dt);

	text1.DrawOutlineMask(canvas, t1pos, 2, BLACK);
	text2.DrawOutlineMask(canvas, t2pos, 2, BLACK);
	text1.DrawTexturedMask(canvas, t1pos, texture);
	text2.DrawTexturedMask(canvas, t2pos, texture);
	shine1.Draw(canvas, text1, 2, t1pos);
	shine2.Draw(canvas, text2, 2, t2pos);
}
