#include "glm/vec4.hpp"
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"
#include "glm/geometric.hpp"
#include "glm/trigonometric.hpp"
#include <math.h>
#include "HotShotAnimation.h"
#include "Main.h"

HotShotAnimation::HotShotAnimation(ParticleOverlayRenderer& particlesoverlay) :
	particlesoverlay(particlesoverlay),
	text1("HOT", Main::GetResources().BoldBitsLarge(), HorizontalAlign::Left, VerticalAlign::Middle),
	text2("SHOT", Main::GetResources().BoldBitsLarge(), HorizontalAlign::Right, VerticalAlign::Middle)
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
		.to(16, 15).during(300).via(easing::cubicIn);

	text2pos = tweeny::from(128 + text2.GetTextSize().width, 15)
		.wait(200)
		.to(116, 15).during(300).via(easing::cubicIn)
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
				textmask.SetPixel(x, y, PixelShader(u, v));
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

glm::vec2 hash(glm::vec2 p)
{
	p = glm::vec2(glm::dot(p, glm::vec2(127.1f, 311.7f)),
		glm::dot(p, glm::vec2(269.5f, 183.3f)));
	return -1.0f + 2.0f * glm::fract(glm::sin(p) * 43758.5453123f);
}

float noise(glm::vec2 p)
{
	const float K1 = 0.366025404f;
	const float K2 = 0.211324865f;
	glm::vec2 i = glm::floor(p + (p.x + p.y) * K1);
	glm::vec2 a = p - i + (i.x + i.y) * K2;
	glm::vec2 o = glm::step(glm::vec2(a.y, a.x), a);
	glm::vec2 b = a - o + K2;
	glm::vec2 c = a - 1.0f + 2.0f * K2;
	glm::vec3 h = glm::max(0.5f - glm::vec3(glm::dot(a, a), glm::dot(b, b), glm::dot(c, c)), 0.0f);
	glm::vec3 n = h * h * h * h * glm::vec3(glm::dot(a, hash(i + 0.0f)), glm::dot(b, hash(i + o)), glm::dot(c, hash(i + 1.0f)));
	return glm::dot(n, glm::vec3(70.0f));
}

float fbm(glm::vec2 st)
{
	constexpr int OCTAVES = 6;
	float value = 0.0f;
	float amplitude = .5f;
	for(int i = 0; i < OCTAVES; i++)
	{
		value += amplitude * noise(st);
		st *= 2.0f;
		amplitude *= 0.5f;
	}
	return value * 0.5f + 0.5f;
}

Color HotShotAnimation::PixelShader(float u, float v)
{
	float iTime = static_cast<float>(ch::ToMilliseconds(Clock::now() - starttime)) / 1000.0f;
	constexpr glm::vec2 fireMovement = glm::vec2(-0.1f, 0.5f);
	constexpr glm::vec2 distortionMovement = glm::vec2(-0.01f, 0.2f);
	constexpr float distortionStrength = 0.3f;

	float color = 0.0f;
	glm::vec2 uv = glm::vec2(u, v);

	//These variables are used to modify the speed at which the texture moves
	float strength = glm::floor(uv.x + 1.0f);
	float t = glm::max(2.0f, 1.25f * strength) * iTime * 0.75f;

	color += fbm(uv * glm::vec2(1.0f, 0.3f) + distortionMovement * t);
	glm::vec2 displacement = glm::clamp((glm::vec2(color) - .5f) * distortionStrength, -1.0f, 1.0f);
	uv += displacement;

	glm::vec2 uvT = (uv * glm::vec2(1.0f, 0.5f)) + fireMovement * t * 0.55f;
	float col = glm::pow(fbm(8.0f * uvT), 1.0f);

	float gradient = 5.0f;
	float finalNoise = col * gradient;

	glm::vec3 colorFinal = finalNoise * glm::vec3(col, col * col * col, col * col * col * col * col * col);

	return Color(static_cast<byte>(std::clamp(colorFinal.r * 255.0f, 0.0f, 255.0f)),
	             static_cast<byte>(std::clamp(colorFinal.g * 255.0f, 0.0f, 255.0f)),
	             static_cast<byte>(std::clamp(colorFinal.b * 255.0f, 0.0f, 255.0f)));
}

