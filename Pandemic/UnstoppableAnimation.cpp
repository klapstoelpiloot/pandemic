#include <math.h>
#include "UnstoppableAnimation.h"
#include "Main.h"
#include "glm/vec2.hpp"
#include "Fire.h"

#define FADE_ALPHA		255
#define FADE_FADE		2
#define BOLT_TIME1		400
#define BOLT_TIME2		820
#define BOLT_DURATION	ch::milliseconds(120)

UnstoppableAnimation::UnstoppableAnimation(ParticleOverlayRenderer& particlesoverlay) :
	text("UNSTOPPABLE", Main::GetResources().BoldBitsLarge(), HorizontalAlign::Left, VerticalAlign::Middle, 1),
	texture(Main::GetResources().GetImage("gray22d.dds")),
	bolt1(Main::GetResources().GetImage("bolt1.dds")),
	bolt2(Main::GetResources().GetImage("bolt2.dds")),
	distort1(Main::GetResources().GetImage("distort1.dds")),
	electcolors(Main::GetResources().GetImage("electcolors.dds"))
{
}

UnstoppableAnimation::~UnstoppableAnimation()
{
}

void UnstoppableAnimation::Start()
{
	Main::GetResources().GetSound("unstoppable.wav").Play();
	starttime = Clock::now();
	laststeptime = starttime;
	textprogress = tweeny::from(100).wait(BOLT_TIME1).to(-text.GetTextSize().width - 5).during(2000);
	fadeprogress = tweeny::from(0.0f).to(1.0f).during(400).via(easing::quadraticOut);
	flashprogress = tweeny::from(0).wait(400)
		.to(255).during(1).to(0).during(200).via(easing::exponentialOut)
		.wait(100)
		.to(255).during(1).to(100).during(120)
		.to(255).during(1).to(100).during(120)
		.to(255).during(1).to(0).during(200).via(easing::exponentialOut);
	bolttime1 = starttime + ch::milliseconds(BOLT_TIME1);
	bolttime2 = starttime + ch::milliseconds(BOLT_TIME2);
	bolttimeoff = TimePoint();
}

void UnstoppableAnimation::Render(Canvas& canvas)
{
	TimePoint t = Clock::now();
	if(!ch::IsTimeSet(starttime) || (t > (starttime + ch::milliseconds(textprogress.duration()))))
	{
		// Animation ended
		starttime = TimePoint();
		return;
	}

	// Advance time
	int dt = static_cast<int>(ch::ToMilliseconds(t - laststeptime));
	laststeptime = t;
	int textpos = textprogress.step(dt);
	float backfade = fadeprogress.step(dt);
	byte flashalpha = static_cast<byte>(flashprogress.step(dt));

	tempcanvas.Clear(Color(0, 0, 0, 0));
	if(t > bolttime1)
	{
		// Draw text on temp canvas
		text.DrawOutlineMask(tempcanvas, Point(textpos, 15), 2, BLACK);
		text.DrawTexturedMask(tempcanvas, Point(textpos, 15), texture);

		// Process pixels for electric effect
		int zt = static_cast<int>(ch::ToMilliseconds(t - starttime - ch::milliseconds(BOLT_TIME1)) % 512);
		if(zt > 255) zt = 255 - (zt - 255);
		float colorswidthf = static_cast<float>(electcolors.Width() - 1);
		for(int y = 0; y < DISPLAY_HEIGHT; y++)
		{
			for(int x = 0; x < DISPLAY_WIDTH; x++)
			{
				int zoffset = distort1.GetByte((-textpos + x) % distort1.GetSize().width, y);
				float epos = std::clamp(static_cast<float>(zoffset - zt), -20.0f, 20.0f) / 40.0f + 0.5f;
				Color ecolor = electcolors.GetColor(static_cast<int>(std::clamp(roundf(epos * colorswidthf), 0.0f, colorswidthf)), 0);
				Color cc = tempcanvas.GetPixel(x, y);
				ecolor.a = cc.a;
				ecolor.Add(cc);
				tempcanvas.SetPixel(x, y, ecolor);
			}
		}
	}

	// Draw the fade and the flashes over the background
	for(int y = 0; y < DISPLAY_HEIGHT; y++)
	{
		for(int x = 0; x < DISPLAY_WIDTH; x++)
		{
			byte fa = static_cast<byte>(std::clamp(static_cast<int>(FADE_ALPHA * backfade) - FADE_FADE * y, 0, 255));
			canvas.BlendPixel(x, y, Color(BLACK, fa));
			canvas.BlendPixel(x, y, Color(WHITE, flashalpha));
		}
	}

	// Draw temp canvas to final canvas
	canvas.DrawColorImageBlend(Point(0, 0), tempcanvas);

	// Draw the lightning bolts
	if(ch::IsTimeSet(bolttime1) && (t > bolttime1) && (!ch::IsTimeSet(bolttimeoff) || (t < bolttimeoff)))
	{
		bolttimeoff = bolttime1 + BOLT_DURATION;
		canvas.DrawColorImageAdd(Point(60, 0), bolt2);
	}
	else if(ch::IsTimeSet(bolttime1) && ch::IsTimeSet(bolttimeoff) && (t > bolttimeoff))
	{
		bolttime1 = TimePoint();
		bolttimeoff = TimePoint();
	}
	else if(ch::IsTimeSet(bolttime2) && (t > bolttime2) && (!ch::IsTimeSet(bolttimeoff) || (t < bolttimeoff)))
	{
		bolttimeoff = bolttime2 + BOLT_DURATION;
		canvas.DrawColorImageAdd(Point(8, 0), bolt1);
	}
}
