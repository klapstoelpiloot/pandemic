#include "TitleRenderer.h"
#include "ProgressBar.h"
#include "File.h"
#include "Main.h"
#include <cmath>
#include "Voronoi.h"
#include "glm/vec2.hpp"
#include "glm/geometric.hpp"

#define TITLE_START_TIME			500
#define TITLE_FADE_DURATION			4000
#define TITLE_STRETCH_SPEED			20		// Milliseconds per pixel
#define PROGRESSBAR_FADE_TIME		500		// From title start time
#define CREDIT_START_TIME			2000	// From title start time
#define CREDIT_FADE_IN_DURATION		500
#define CREDIT_FADE_SHOW_DURATION	2000
#define CREDIT_FADE_OUT_DURATION	500
#define CREDIT_TEXT_COLOR			Color(120, 120, 120)
#define KEY_FLASH_INTERVAL			400
#define KEY_TEXT_COLOR				Color(200, 200, 200)
#define TITLE_COLOR					Color(255, 25, 0)
#define VORONOI_COLOR				Color(255, 25, 0)
#define SPLASH_FADE_DISTANCE		3

// List of credits to display below the title
vector<String> CREDITS { "BY PASCAL VAN DER HEIDEN", "MUSIC BY JOHN S. WEEKLEY" };

TitleRenderer::TitleRenderer() :
	mulimage(Main::GetResources().GetImage("title_mul.dds")),
	addimage(Main::GetResources().GetImage("title_add.dds")),
	pressbuttontext("PRESS ANY BUTTON TO START", Main::GetResources().Smallest(), HorizontalAlign::Center, VerticalAlign::Bottom),
	creditstext(Main::GetResources().Smallest(), HorizontalAlign::Center, VerticalAlign::Bottom),
	showkeyinfo(false),
	asbackground(false),
	creditindex(0)
{
	voronoi.Clear(BLACK);
}

void TitleRenderer::BeginFirstStart()
{
	titlestarttime = Clock::now() + ch::milliseconds(TITLE_START_TIME);
	creditstarttime = titlestarttime + ch::milliseconds(CREDIT_START_TIME);
	laststeptime = Clock::now() + ch::milliseconds(TITLE_START_TIME);
	showkeyinfo = false;
	creditindex = 0;
	creditstext.SetText(CREDITS[creditindex]);
	SetupSplashes();
}

void TitleRenderer::BeginReturnToTitle()
{
	titlestarttime = Clock::now() + ch::milliseconds(TITLE_START_TIME);
	creditstarttime = titlestarttime + ch::milliseconds(CREDIT_START_TIME);
	laststeptime = Clock::now() + ch::milliseconds(TITLE_START_TIME);
	showkeyinfo = false;
	creditindex = 0;
	creditstext.SetText(CREDITS[creditindex]);
	SetupSplashes();
}

void TitleRenderer::SetupSplashes()
{
	// The curve which the splash must progress along
	splashdistance = tweeny::from(0.0f).to(64.0f).during(TITLE_FADE_DURATION).via(easing::cubicOut);

	// Pick splash points
	Rect sr1 = Rect(11, 6, 36, 21);
	Rect sr2 = Rect(48, 4, 35, 22);
	Rect sr3 = Rect(83, 2, 30, 20);
	splashpoints.clear();
	splashpoints.push_back(Point(Random(sr1.x, sr1.x + sr1.width), Random(sr1.y, sr1.y + sr1.height)));
	splashpoints.push_back(Point(Random(sr2.x, sr2.x + sr2.width), Random(sr2.y, sr2.y + sr2.height)));
	splashpoints.push_back(Point(Random(sr3.x, sr3.x + sr3.width), Random(sr3.y, sr3.y + sr3.height)));
}

void TitleRenderer::Render(Canvas& canvas)
{
	TimePoint now = Clock::now();
	if(now < titlestarttime)
		return;

	// Render the voronoi
	float timesec = static_cast<float>(ch::ToMilliseconds(now - titlestarttime)) / 1000.0f;
	temporalditheroffset++;
	for(int y = 0; y < DISPLAY_HEIGHT; y++)
	{
		for(int x = 0; x < DISPLAY_WIDTH; x++)
		{
			// Don't redraw every pixel every frame, that is too expensive.
			if(((x ^ y ^ temporalditheroffset) & 0x03) == 0)
			{
				byte v = Voronoi::PixelShader(x, DISPLAY_HEIGHT - y, timesec);
				Color c = VORONOI_COLOR;
				c.ModulateRGB(v);
				voronoi.SetPixel(x, y, c);
			}
		}
	}

	// Play/repeat the title music
	if(playtitlemusic)
		playtitlemusic();

	// Time in milliseconds since the start of animation
	uint64 t = ch::ToMilliseconds(now - titlestarttime);
	int dt = static_cast<int>(ch::ToMilliseconds(now - laststeptime));
	laststeptime = now;
	splashdistance.step(dt);

	// Draw the title screen
	float splashdistancevalue = splashdistance.peek();
	MonoSampler mulsampler = mulimage.GetMonoSampler();
	MonoSampler addsampler = addimage.GetMonoSampler();
	for(int y = 0; y < DISPLAY_HEIGHT; y++)
	{
		for(int x = 0; x < DISPLAY_WIDTH; x++)
		{
			Color v = voronoi.GetPixel(x, y);
			byte m = mulsampler(x, y);
			byte a = addsampler(x, y);

			float f;
			if(((m > 0) || (a > 0)) && (splashdistance.progress() < 1.0f))
			{
				// Determine the alpha depending on distance from splashpoints
				f = 0.0f;
				glm::vec2 pv = glm::vec2(static_cast<float>(x), static_cast<float>(y));
				for(Point sp : splashpoints)
				{
					glm::vec2 dv = glm::vec2(static_cast<float>(sp.x), static_cast<float>(sp.y)) - pv;
					float d = glm::length(dv);

					// Darker points are filled later
					d += static_cast<float>((256 - a) / 32);

					if(d > splashdistancevalue)
						continue;
					else
						f += std::clamp((splashdistancevalue - d) / SPLASH_FADE_DISTANCE, 0.0f, 1.0f);
				}
			}
			else
			{
				f = 1.0f;
			}

			v.ModulateRGB(m);
			v.Blend(Color(TITLE_COLOR, a));
			v.Blend(Color(BLACK, static_cast<byte>(255.0f - std::clamp(f * 255.0f, 0.0f, 255.0f))));
			canvas.SetPixel(x, y, v);
		}
	}

	if(asbackground)
	{
		// Reset the credits time just in case we go back to the title screen
		creditstarttime = Clock::now();
	}
	else if(Clock::now() >= creditstarttime)
	{
		if(creditindex < CREDITS.size())
		{
			t = ch::ToMilliseconds(Clock::now() - creditstarttime);

			// Calculate translucency at this time
			float credits_alpha;
			if(t < CREDIT_FADE_IN_DURATION)
				credits_alpha = (static_cast<float>(t) / CREDIT_FADE_IN_DURATION) * 255.0f;
			else if(t < (CREDIT_FADE_IN_DURATION + CREDIT_FADE_SHOW_DURATION))
				credits_alpha = 255.0f;
			else
				credits_alpha = 255.0f - (static_cast<float>(t - CREDIT_FADE_IN_DURATION - CREDIT_FADE_SHOW_DURATION) / CREDIT_FADE_OUT_DURATION) * 255.0f;

			// Show credits
			Color c = CREDIT_TEXT_COLOR;
			Color co = BLACK;
			c.a = static_cast<byte>(std::clamp(credits_alpha, 0.0f, 255.0f));
			co.a = c.a;
			creditstext.DrawOutlineBlend(canvas, Point(64, 34), 1, co);
			creditstext.DrawBlend(canvas, Point(64, 34), c);

			// Next credits?
			if(t >= (CREDIT_FADE_IN_DURATION + CREDIT_FADE_SHOW_DURATION + CREDIT_FADE_OUT_DURATION))
			{
				creditindex++;
				if(creditindex < CREDITS.size())
					creditstext.SetText(CREDITS[creditindex]);
				creditstarttime = Clock::now();
			}
		}
		else
		{
			// Flash info
			if(((t / KEY_FLASH_INTERVAL) % 2) == 0)
			{
				pressbuttontext.DrawOutlineMask(canvas, Point(64, 34), 1, BLACK);
				pressbuttontext.DrawMask(canvas, Point(64, 34), KEY_TEXT_COLOR);
			}
		}
	}
}
