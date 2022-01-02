#include <math.h>
#include "GreatShotAnimation.h"
#include "Main.h"

GreatShotAnimation::GreatShotAnimation(ParticleOverlayRenderer& particlesoverlay) :
	particlesoverlay(particlesoverlay),
	text1("GREAT", Main::GetResources().BoldBitsLarge(), HorizontalAlign::Left, VerticalAlign::Middle),
	text2("SHOT", Main::GetResources().BoldBitsLarge(), HorizontalAlign::Right, VerticalAlign::Middle),
	texture(Main::GetResources().GetImage("gray22.dds"))
{
	particles.SetAdditive(true);
	particles.SetDeceleration(1.0f);
	particles.SetFadeAlpha(0.0f);
	particles.SetLifetime(50);
	particles.Begin();
	particlesoverlay.RegisterParticleEffect(&particles);
}

GreatShotAnimation::~GreatShotAnimation()
{
	particlesoverlay.UnregisterParticleEffect(&particles);
}

void GreatShotAnimation::Start()
{
	Main::GetResources().GetSound("greatshot.wav").Play();
	starttime = Clock::now();
	laststeptime = Clock::now();
	lastparticletime = Clock::now();

	text1pos = tweeny::from(0, -16)
		.to(0, 16).during(500).via(easing::cubicOut)
		.wait(200)
		.to(0, 48).during(500).via(easing::cubicIn);

	text2pos = tweeny::from(128, 48)
		.to(128, 16).during(500).via(easing::cubicOut)
		.wait(200)
		.to(128, -16).during(500).via(easing::cubicIn);
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

	int dt = static_cast<int>(ch::ToMilliseconds(t - laststeptime));
	laststeptime = t;

	text1pos.step(dt);
	text2pos.step(dt);

	Point t1pos = text1pos.peek();
	Point t2pos = text2pos.peek();
	text1.DrawOutlineMask(canvas, t1pos, 2, BLACK);
	text2.DrawOutlineMask(canvas, t2pos, 2, BLACK);
	text1.DrawTexturedMask(canvas, t1pos, texture);
	text2.DrawTexturedMask(canvas, t2pos, texture);

	while(lastparticletime < t)
	{
		// Spawn particles
		int x = Random(0, text1.GetTextSize().width);
		int y = t1pos.y - (text1.GetTextSize().height / 4) + Random(0, text1.GetTextSize().height / 2);
		particles.Spawn(Particle(glm::vec2(x, y), glm::vec2(0, 0), Color(WHITE, 128), glm::vec2(0, 0)));

		x = 128 - Random(0, text2.GetTextSize().width);
		y = t2pos.y - (text2.GetTextSize().height / 4) + Random(0, text2.GetTextSize().height / 2);
		particles.Spawn(Particle(glm::vec2(x, y), glm::vec2(0, 0), Color(WHITE, 128), glm::vec2(0, 0)));
		
		lastparticletime += ch::milliseconds(20);
	}
}
