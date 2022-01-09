#include <math.h>
#include "MegaShotAnimation.h"
#include "Main.h"
#include "glm/vec2.hpp"
#include "glm/geometric.hpp"

#define FLASH_START_TIME		ch::milliseconds(400)
#define FLASH_INTERVAL			ch::milliseconds(100)
#define FLASH_DURATION			100
#define FLASH_COUNT				6
#define PARTICLE_SPAWN_COUNT	40

MegaShotAnimation::MegaShotAnimation(ParticleOverlayRenderer& particlesoverlay) :
	particlesoverlay(particlesoverlay),
	text("MEGA SHOT", Main::GetResources().BoldBitsLarge(), HorizontalAlign::Center, VerticalAlign::Middle),
	texture(Main::GetResources().GetImage("yellow22d.dds"))
{
	particles.SetAdditive(false);
	particles.SetDeceleration(0.98f);
	particles.SetFadeAlpha(-0.5f);
	particles.SetGravity(glm::vec2(0.0f, 2.0f));
	particles.SetSwingStrength(0.0f);
	particles.SetLifetime(3000);
	particlesoverlay.RegisterParticleEffect(&particles);
}

MegaShotAnimation::~MegaShotAnimation()
{
	particlesoverlay.UnregisterParticleEffect(&particles);
}

void MegaShotAnimation::Start()
{
	Main::GetResources().GetSound("megashot.wav").Play();
	starttime = Clock::now();
	laststeptime = Clock::now();
	nextflashtime = Clock::now() + FLASH_START_TIME;
	textsize = tweeny::from(0).to(128).during(400).via(easing::circularIn).wait(1200);
	flashcount = 0;
}

void MegaShotAnimation::Render(Canvas& canvas)
{
	TimePoint t = Clock::now();
	if(!ch::IsTimeSet(starttime) || (t > (starttime + ch::milliseconds(textsize.duration()))))
	{
		// Animation ended
		starttime = TimePoint();
		return;
	}

	// Advance time
	int dt = static_cast<int>(ch::ToMilliseconds(t - laststeptime));
	laststeptime = t;
	int textwidth = textsize.step(dt);

	// Flash the text
	if(ch::IsTimeSet(nextflashtime) && (t > nextflashtime) && (flashcount < FLASH_COUNT))
	{
		if(flashcount == 0)
		{
			// Spawn particles
			glm::vec center = glm::vec2(64.0f, 16.0f);
			for(int i = 0; i < PARTICLE_SPAWN_COUNT; i++)
			{
				byte c = static_cast<byte>(Random(50, 128));
				glm::vec pos = glm::vec2(Random(2.0f, 126.0f), Random(6.0f, 26.0f));
				glm::vec delta = pos - center;
				float deltalen = glm::length(delta);
				glm::vec dir = glm::normalize(pos - center);
				dir.y *= 2.0f;
				dir *= (80.0f - deltalen) * 0.01f;
				particles.Spawn(Particle(pos, dir, Color(c, c, c), glm::vec2(0.0f, 0.0f)));
			}
		}

		flashcount++;
		nextflashtime += FLASH_INTERVAL;
		flasher.Begin(FLASH_DURATION);
	}

	// Draw the text on a separate canvas
	tempcanvas.Clear(Color(0, 0, 0, 0));
	text.DrawOutlineMask(tempcanvas, Point(65, 15), 2, BLACK);
	text.DrawTexturedMask(tempcanvas, Point(65, 15), texture);
	flasher.Draw(tempcanvas, text, Point(65, 15), 2);

	// Calculate size and position
	Size tempcanvassize = tempcanvas.GetSize();
	int textheight = static_cast<int>(std::roundf((static_cast<float>(textwidth) / static_cast<float>(tempcanvassize.width)) * static_cast<float>(tempcanvassize.height)));
	Point offset = Point((canvas.Width() - textwidth) / 2, (canvas.Height() - textheight) / 2);
	if((textwidth > 0) && (textheight > 0))
	{
		// Draw to the final canvas with resizing
		int x_ratio = ((tempcanvassize.width << 16) / textwidth) + 1;
		int y_ratio = ((tempcanvassize.height << 16) / textheight) + 1;
		for(int i = 0; i < textheight; i++)
		{
			for(int j = 0; j < textwidth; j++)
			{
				int x2 = (j * x_ratio) >> 16;
				int y2 = (i * y_ratio) >> 16;
				canvas.MaskPixel(offset.x + j, offset.y + i, tempcanvas.GetPixel(x2, y2));
			}
		}
	}
}
