#include "glm/vec4.hpp"
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"
#include "glm/geometric.hpp"
#include "glm/trigonometric.hpp"
#include <math.h>
#include "HotShotAnimation.h"
#include "Main.h"
#include "Fire.h"

HotShotAnimation::HotShotAnimation(ParticleOverlayRenderer& particlesoverlay) :
	particlesoverlay(particlesoverlay),
	text1("HOT", Main::GetResources().BoldBitsLarge(), HorizontalAlign::Left, VerticalAlign::Middle, 1),
	text2("SHOT!", Main::GetResources().BoldBitsLarge(), HorizontalAlign::Right, VerticalAlign::Middle, 1)
{
	particles.SetAdditive(true);
	particles.SetDeceleration(1.0f);
	particles.SetFadeAlpha(-2.0f);
	particles.SetGravity(glm::vec2(0.0f, -0.2f));
	particles.SetSwingStrength(0.005f);
	particles.SetLifetime(3000);
	particlesoverlay.RegisterParticleEffect(&particles);
}

HotShotAnimation::~HotShotAnimation()
{
	particlesoverlay.UnregisterParticleEffect(&particles);
}

void HotShotAnimation::Start()
{
	Main::GetResources().GetSound("hotshot.wav").Play();
	starttime = Clock::now();
	laststeptime = Clock::now();
	lastparticletime = Clock::now();
	particles.Begin();

	text1pos = tweeny::from(-text1.GetTextSize().width, 15)
		.to(7, 15).during(300).via(easing::cubicIn);

	text2pos = tweeny::from(128 + text2.GetTextSize().width, 15)
		.wait(200)
		.to(121, 15).during(300).via(easing::cubicIn)
		.wait(1000);
}

void HotShotAnimation::Render(Canvas& canvas)
{
	TimePoint t = Clock::now();
	if(!ch::IsTimeSet(starttime) || (t > (starttime + ch::milliseconds(text2pos.duration()))))
	{
		// Animation ended
		starttime = TimePoint();
		return;
	}

	int dt = static_cast<int>(ch::ToMilliseconds(t - laststeptime));
	laststeptime = t;

	Point t1pos = text1pos.step(dt);
	Point t2pos = text2pos.step(dt);

	textmask.Clear(Color(0, 0, 0, 0));
	text1.DrawOutlineMask(textmask, t1pos, 2, BLACK);
	text2.DrawOutlineMask(textmask, t2pos, 2, BLACK);
	text1.DrawMask(textmask, t1pos, WHITE);
	text2.DrawMask(textmask, t2pos, WHITE);

	// Turn all white pixels into fire
	float timesec = static_cast<float>(ch::ToMilliseconds(t - starttime)) / 1000.0f;
	float scale_x = 1.0f / static_cast<float>(DISPLAY_WIDTH);
	float scale_y = 1.0f / static_cast<float>(DISPLAY_HEIGHT * 2);
	Size masksize = textmask.GetSize();
	for(int y = 0; y < masksize.height; y++)
	{
		for(int x = 0; x < masksize.width; x++)
		{
			if(textmask.GetPixel(x, y).r > 0)
			{
				float u = static_cast<float>(x) * scale_x;
				float v = static_cast<float>(y) * scale_y;
				textmask.SetPixel(x, y, Fire::PixelShader(u, v, timesec));
			}
		}
	}

	canvas.DrawColorImageMask(Point(0, 0), textmask);

	while(lastparticletime < t)
	{
		Color color(255, static_cast<byte>(Random(0, 255)), 0);

		// Meta is used for Swing effect
		// X = x position to swing about
		// Y = amount of swing factor

		int x = t1pos.x + Random(0, text1.GetTextSize().width);
		int y = Random(6, 22);
		glm::vec2 meta = glm::vec2(static_cast<float>(x) + Random(-2.0f, 2.0f), Random(1.0f, 2.0f));
		particles.Spawn(Particle(glm::vec2(x, y), glm::vec2(Random(-0.2f, 0.2f), -0.1f), color, 1.0f, meta));

		x = t2pos.x - Random(0, text2.GetTextSize().width);
		y = Random(6, 22);
		meta = glm::vec2(static_cast<float>(x) + Random(-2.0f, 2.0f), Random(1.0f, 2.0f));
		particles.Spawn(Particle(glm::vec2(x, y), glm::vec2(Random(-0.2f, 0.2f), -0.1f), color, 1.0f, meta));

		lastparticletime += ch::milliseconds(30);
	}
}
