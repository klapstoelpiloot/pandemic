#include <math.h>
#include "ExcellentAnimation.h"
#include "Main.h"
#include "glm/vec2.hpp"
#include "Fire.h"

#define BURN_LENGTH				0.1f
#define NUM_SMOKE_IMAGES		6
#define PARTICLES_PER_MS		1

ExcellentAnimation::ExcellentAnimation(ParticleOverlayRenderer& particlesoverlay) :
	text("EXCELLENT", Main::GetResources().BoldBitsLarge(), HorizontalAlign::Center, VerticalAlign::Middle),
	burnmap(Main::GetResources().GetImage("burnmap.dds")),
	burntex(Main::GetResources().GetImage("burntex.dds")),
	burncolors(Main::GetResources().GetImage("burncolors.dds"))
{
}

ExcellentAnimation::~ExcellentAnimation()
{
}

void ExcellentAnimation::Start()
{
	Main::GetResources().GetSound("excellent.wav").Play();
	starttime = Clock::now();
	laststeptime = Clock::now();
	burnprogress = tweeny::from(1.0f + BURN_LENGTH).to(-BURN_LENGTH * 2.0f).during(2000).via(easing::quarticOut).wait(400);
}

void ExcellentAnimation::Render(Canvas& canvas)
{
	TimePoint t = Clock::now();
	if(!ch::IsTimeSet(starttime) || (t > (starttime + ch::milliseconds(burnprogress.duration()))))
	{
		// Animation ended
		starttime = TimePoint();
		return;
	}

	// Advance time
	int dt = static_cast<int>(ch::ToMilliseconds(t - laststeptime));
	laststeptime = t;
	float burndist = burnprogress.step(dt);

	tempcanvas.Clear(Color(0, 0, 0, 0));
	text.DrawOutlineMask(tempcanvas, Point(65, 15), 2, BLACK);
	text.DrawMask(tempcanvas, Point(65, 15), WHITE);

	// Turn all white pixels into glowy fire
	float timesec = static_cast<float>(ch::ToMilliseconds(t - starttime)) / 3000.0f;
	float scale_x = 1.0f / static_cast<float>(DISPLAY_WIDTH * 0.5f);
	float scale_y = 1.0f / static_cast<float>(DISPLAY_HEIGHT);
	Size canvassize = tempcanvas.GetSize();
	float burncolorswidthf = static_cast<float>(burncolors.Width() - 1);
	for(int y = 0; y < canvassize.height; y++)
	{
		for(int x = 0; x < canvassize.width; x++)
		{
			Color tc = tempcanvas.GetPixel(x, y);
			if(tc.a > 0)
			{
				float bm = 1.0f - static_cast<float>(burnmap.GetByte(x, y)) / 255.0f;
				byte bt = burntex.GetByte(x, y);
				float edgepos = std::clamp((burndist + bm) - 1.0f, -BURN_LENGTH, 0.0f) + BURN_LENGTH;
				float edgeglow = edgepos / BURN_LENGTH;

				Color c;
				if(edgeglow < 0.999f)
				{
					Color edgecolor = burncolors.GetColor(static_cast<int>(std::clamp(roundf(edgeglow * burncolorswidthf), 0.0f, burncolorswidthf)), 0);
					float u = static_cast<float>(x) * scale_x;
					float v = static_cast<float>(y) * scale_y;
					c = Fire::PixelShader(u, v, timesec);
					c.ModulateRGB(bt);
					c.ModulateRGBA(tc);
					c.Add(edgecolor);
				}
				else
				{
					c = Color(0, 0, 0, 0);
				}

				tempcanvas.SetPixel(x, y, c);
			}
		}
	}

	canvas.DrawColorImageMask(Point(0, 0), tempcanvas);
}
