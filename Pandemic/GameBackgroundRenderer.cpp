#include "GameBackgroundRenderer.h"
#include "Voronoi.h"

vector<Color> COLORS { Color(120, 0, 0), Color(110, 50, 0), Color(0, 100, 0), Color(0, 110, 110), Color(0, 30, 160), Color(110, 50, 100) };

GameBackgroundRenderer::GameBackgroundRenderer() :
	starttime(Clock::now()),
	timesec(0.0f),
	temporalditheroffset(0),
	colorindex(0)
{
	// Because we don't draw all the pixels every frame, we need to
	// draw on a separate buffer which we copy to the canvas every frame.
	backbuffer.Clear(BLACK);
}

void GameBackgroundRenderer::Begin()
{
	starttime = Clock::now();
	colorindex = 0;
}

void GameBackgroundRenderer::SetNextColor()
{
	colorindex++;
	if(colorindex == static_cast<int>(COLORS.size()))
		colorindex = 0;
}

void GameBackgroundRenderer::SetPrevColor()
{
	colorindex--;
	if(colorindex < 0)
		colorindex = static_cast<int>(COLORS.size()) - 1;
}

void GameBackgroundRenderer::Render(Canvas& canvas)
{
	timesec = static_cast<float>(ch::ToMilliseconds(Clock::now() - starttime)) / 1000.0f;
	temporalditheroffset++;

	for(int y = 0; y < DISPLAY_HEIGHT; y++)
	{
		for(int x = 0; x < DISPLAY_WIDTH; x++)
		{
			// Don't redraw every pixel every frame, that is too expensive.
			// I'm also happy with the grainy effect that this dither gives!
			if(((x ^ y ^ temporalditheroffset) & 0x03) == 0)
			{
				byte v = Voronoi::PixelShader(x, DISPLAY_HEIGHT - y, timesec);
				Color c = COLORS[colorindex];
				c.ModulateRGB(v);
				backbuffer.SetPixel(x, y, c);
			}
		}
	}

	// Copy image to canvas
	backbuffer.CopyTo(canvas);
}
