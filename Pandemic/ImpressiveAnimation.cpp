#include <math.h>
#include "ImpressiveAnimation.h"
#include "Main.h"
#include "glm/vec2.hpp"
#include "glm/geometric.hpp"

#define FLASH_INTERVAL			ch::milliseconds(80)
#define TOTAL_DURATION			2500
#define PARTICLES_PER_CHAR		12

ImpressiveAnimation::ImpressiveAnimation(ParticleOverlayRenderer& particlesoverlay) :
	particlesoverlay(particlesoverlay),
	character
	{
		Text("I", Main::GetResources().BoldBitsLarge(), HorizontalAlign::Left, VerticalAlign::Top),
		Text("M", Main::GetResources().BoldBitsLarge(), HorizontalAlign::Left, VerticalAlign::Top),
		Text("P", Main::GetResources().BoldBitsLarge(), HorizontalAlign::Left, VerticalAlign::Top),
		Text("R", Main::GetResources().BoldBitsLarge(), HorizontalAlign::Left, VerticalAlign::Top),
		Text("E", Main::GetResources().BoldBitsLarge(), HorizontalAlign::Left, VerticalAlign::Top),
		Text("S", Main::GetResources().BoldBitsLarge(), HorizontalAlign::Left, VerticalAlign::Top),
		Text("S", Main::GetResources().BoldBitsLarge(), HorizontalAlign::Left, VerticalAlign::Top),
		Text("I", Main::GetResources().BoldBitsLarge(), HorizontalAlign::Left, VerticalAlign::Top),
		Text("V", Main::GetResources().BoldBitsLarge(), HorizontalAlign::Left, VerticalAlign::Top),
		Text("E", Main::GetResources().BoldBitsLarge(), HorizontalAlign::Left, VerticalAlign::Top)
	},
	texture(Main::GetResources().GetImage("yellow12s.dds")),
	flashcount(0)
{
	Text word("IMPRESSIVE", Main::GetResources().BoldBitsLarge(), HorizontalAlign::Center, VerticalAlign::Middle);
	Rect wordrect = word.GetTextRect(Point(65, 15));
	int x = wordrect.x;
	for(int i = 0; i < 10; i++)
	{
		charpos[i] = Point(x, wordrect.y);
		x += character[i].GetTextSize().width;
	}

	particles.SetAdditive(true);
	particles.SetDeceleration(0.96f);
	particles.SetFadeAlpha(-2.0f);
	particles.SetGravity(glm::vec2(0.0f, 0.0f));
	particles.SetSwingStrength(0.0f);
	particles.SetLifetime(3000);
	particlesoverlay.RegisterParticleEffect(&particles);
}

ImpressiveAnimation::~ImpressiveAnimation()
{
	particlesoverlay.UnregisterParticleEffect(&particles);
}

void ImpressiveAnimation::Start()
{
	Main::GetResources().GetSound("impressive.wav").Play();
	starttime = Clock::now();
	laststeptime = Clock::now();
	nextflashtime = Clock::now() + FLASH_INTERVAL;
	offset = tweeny::from(80).to(-160).during(TOTAL_DURATION);
	flashcount = 0;
}

void ImpressiveAnimation::Render(Canvas& canvas)
{
	TimePoint t = Clock::now();
	if(!ch::IsTimeSet(starttime) || (t > (starttime + ch::milliseconds(TOTAL_DURATION))))
	{
		// Animation ended
		starttime = TimePoint();
		return;
	}

	// Advance time
	int dt = static_cast<int>(ch::ToMilliseconds(t - laststeptime));
	laststeptime = t;
	int textoffset = offset.step(dt);
	if((flashcount < 10) && (t > nextflashtime))
	{
		flasher[flashcount].Begin();
		flashcount++;
		nextflashtime += FLASH_INTERVAL;

		for(int i = 0; i < PARTICLES_PER_CHAR; i++)
		{
			glm::vec2 pos = glm::vec2(charpos[flashcount].x + character[flashcount].GetTextSize().width / 2 + textoffset,
				charpos[flashcount].y + character[flashcount].GetTextSize().height / 2);
			glm::vec2 dir = glm::normalize(glm::vec2(Random(-2.0f, 2.0f), Random(-2.0f, 2.0f)));
			float speed = Random(0.5f, 1.0f);
			particles.Spawn(Particle(pos, dir * speed, Color(255, 255, static_cast<byte>(Random(0, 255))), glm::vec2(0.0f, 0.0f)));
		}
	}

	Point to = Point(static_cast<int>(ch::ToMilliseconds(t - starttime)) / 32, static_cast<int>(ch::ToMilliseconds(t - starttime)) / 32);
	for(int i = 0; i < flashcount; i++)
	{
		Point p = charpos[i];
		p.x += textoffset;
		p.y += static_cast<int>(roundf(sin(static_cast<float>(p.x) / 4.0f) * 2.0f));
		character[i].DrawOutlineMask(canvas, p, 2, BLACK);
		character[i].DrawTexturedMask(canvas, p, texture, to);
		flasher[i].Draw(canvas, character[i], p, 2);
	}
}
