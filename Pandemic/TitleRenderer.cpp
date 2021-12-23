#include "TitleRenderer.h"
#include "ProgressBar.h"
#include "File.h"
#include "Main.h"
#include <cmath>

#define TITLE_START_TIME			500
#define TITLE_FADE_DURATION			100
#define TITLE_STRETCH_SPEED			20		// Milliseconds per pixel
#define PROGRESSBAR_FADE_TIME		500		// From title start time
#define CREDIT_START_TIME			2000	// From title start time
#define CREDIT_FADE_IN_DURATION		500
#define CREDIT_FADE_SHOW_DURATION	2000
#define CREDIT_FADE_OUT_DURATION	500
#define CREDIT_TEXT_COLOR			Color(120, 120, 120)
#define KEY_FLASH_INTERVAL			400
#define KEY_TEXT_COLOR				Color(200, 200, 200)

// Time offset in milliseconds for each column
uint64 COLUMN_OFFSET[] = {		0, 0, 0, 0, 0, 1500, 1200, 900, 600, 450,
								300, 150, 0, 150, 300, 450, 600, 900, 1200, 1350,
								1500, 1200, 900, 600, 300, 150, 0, 0, 150, 300,
								150, 300, 750, 1500, 1500, 900, 300, 0, 0, 0,
								300, 375, 75, 0, 0, 450, 900, 1500, 1800, 1200,
								750, 300, 0, 75, 0, 0, 300, 450, 750, 1200,
								1350, 1500, 1450, 1400, 1350, 1350, 900, 600, 450,
								300, 150, 0, 0, 75, 0, 75, 150, 75, 0,
								150, 300, 450, 600, 150, 75, 0, 0, 75, 150,
								75, 300, 150, 0, 75, 450, 750, 1500, 1200, 1050,
								750, 750, 300, 0, 0, 0, 300, 900, 450, 150, 75,
								0, 0, 75, 0, 150, 75, 150, 0, 150, 225,
								300, 900, 600, 150, 150, 0, 0, 0, 0, 0 };

// List of credits to display below the title
vector<String> CREDITS { "BY PASCAL VAN DER HEIDEN", "MUSIC BY JOHN S. WEEKLEY" };

TitleRenderer::TitleRenderer() :
	titleimage(Main::GetResources().GetImage("title.dds")),
	pressbuttontext("PRESS ANY BUTTON TO START", Main::GetResources().Smallest(), HorizontalAlign::Center, VerticalAlign::Bottom),
	creditstext(Main::GetResources().Smallest(), HorizontalAlign::Center, VerticalAlign::Bottom),
	showkeyinfo(false),
	asbackground(false),
	creditindex(0)
{
}

void TitleRenderer::BeginFirstStart()
{
	titlestarttime = Clock::now() + ch::milliseconds(TITLE_START_TIME);
	creditstarttime = titlestarttime + ch::milliseconds(CREDIT_START_TIME);
	showkeyinfo = false;
	creditindex = 0;
	creditstext.SetText(CREDITS[creditindex]);
}

void TitleRenderer::BeginReturnToTitle()
{
	titlestarttime = Clock::now() + ch::milliseconds(TITLE_START_TIME);
	creditstarttime = titlestarttime + ch::milliseconds(CREDIT_START_TIME);
	showkeyinfo = false;
	creditindex = 0;
	creditstext.SetText(CREDITS[creditindex]);
}

void TitleRenderer::RenderTitleColumn(Canvas& canvas, int x, int img_y, int img_len, int canvas_y, int canvas_len, byte mod)
{
	// Draw all pixels for this column from top to bottom
	int lasty = img_y - 1;
	for(int y = 0; y < canvas_len; y++)
	{
		// Where on the original image would this point be?
		int yu = img_y + static_cast<int>(((static_cast<float>(y) + 0.5f) / static_cast<float>(canvas_len)) * static_cast<float>(img_len));
		if(yu >= (img_y + img_len))
			yu = (img_y + img_len) - 1;

		// All the pixels from (lasty + 1) until and including yu are compressed in this pixel.
		// Calculate the average color for this pixel.
		int tr = 0, tg = 0, tb = 0;
		for(int iy = lasty + 1; iy <= yu; iy++)
		{
			Color c = titleimage.GetColor(x, iy);
			tr += c.r;
			tg += c.g;
			tb += c.b;
		}
		Color cc = BLACK;
		cc.r = static_cast<byte>(std::clamp(tr / (yu - lasty), 0, 255));
		cc.g = static_cast<byte>(std::clamp(tg / (yu - lasty), 0, 255));
		cc.b = static_cast<byte>(std::clamp(tb / (yu - lasty), 0, 255));
		cc.Modulate(mod);

		// Draw
		canvas.SetPixel(x, canvas_y + y, cc);
		lasty = yu;
	}
}

void TitleRenderer::Render(Canvas& canvas)
{
	ProgressBar* bar = Main::GetResources().GetProgressBar();

	// Don't draw anything until the start time
	if(Clock::now() < titlestarttime)
	{
		bar->SetTranslucency(255);
		bar->Render(canvas);
		return;
	}

	// Play/repeat the title music
	if(playtitlemusic)
		playtitlemusic();

	// Time in milliseconds since the start of animation
	uint64 t = ch::ToMilliseconds(Clock::now() - titlestarttime);

	// Calculate fade
	float fade_alpha;
	if(t < TITLE_FADE_DURATION)
		fade_alpha = (static_cast<float>(t) / TITLE_FADE_DURATION) * 255.0f;
	else 
		fade_alpha = 255.0f;
	byte modulation = static_cast<byte>(std::clamp(fade_alpha, 0.0f, 255.0f));

	// Render title image per column
	int partheight = titleimage.Height() / 2;
	for(int x = 0; x < titleimage.Width(); x++)
	{
		// Render top half of the image
		int toplength = 1;
		if(t > (TITLE_FADE_DURATION + COLUMN_OFFSET[titleimage.Width() - x]))
		{
			uint64 st = t - TITLE_FADE_DURATION;
			toplength = static_cast<int>(std::ceil(static_cast<float>(st - COLUMN_OFFSET[titleimage.Width() - x]) / TITLE_STRETCH_SPEED));
			if(toplength > partheight)
				toplength = partheight;
		}
		RenderTitleColumn(canvas, x, 0, 16, 16 - toplength, toplength, modulation);

		// Render bottom half of the image
		int bottomlength = 1;
		if(t > (TITLE_FADE_DURATION + COLUMN_OFFSET[x]))
		{
			uint64 st = t - TITLE_FADE_DURATION;
			bottomlength = static_cast<int>(std::ceil(static_cast<float>(st - COLUMN_OFFSET[x]) / TITLE_STRETCH_SPEED));
			if(bottomlength > partheight)
				bottomlength = partheight;
		}
		RenderTitleColumn(canvas, x, 16, 16, 16, bottomlength, modulation);
	}

	// Blend the progressbar over the image until it has faded out
	if(t < PROGRESSBAR_FADE_TIME)
	{
		bar->SetTranslucency(static_cast<byte>(255.0f - std::clamp((static_cast<float>(t) / PROGRESSBAR_FADE_TIME) * 255.0f, 0.0f, 255.0f)));
		bar->Render(canvas);
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
