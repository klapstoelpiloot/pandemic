#include <math.h>
#include "ScreenMeltRenderer.h"
#include "Main.h"

#define MAX_OFFSET_MS			400
#define MAX_DELTA_OFFSET_MS		100
#define MELT_SPEED				28		// Milliseconds per pixel

ScreenMeltRenderer::ScreenMeltRenderer()
{
}

void ScreenMeltRenderer::Begin()
{
	// Make a copy of the last rendered screen
	Main::GetGraphics().GetCanvas().CopyTo(meltcanvas);

	// Make a random offset for each column
	int off = Random(0, MAX_OFFSET_MS);
	for(int i = 0; i < DISPLAY_WIDTH; i++)
	{
		int d = Random(0, MAX_DELTA_OFFSET_MS * 2) - MAX_DELTA_OFFSET_MS;
		off = std::clamp(off + d, 0, MAX_OFFSET_MS);
		offsets[i] = off;
	}

	// Go
	starttime = Clock::now();
	endtime = starttime + ch::milliseconds((MELT_SPEED * DISPLAY_HEIGHT) + MAX_OFFSET_MS);
}

void ScreenMeltRenderer::Render(Canvas& canvas)
{
	TimePoint t = Clock::now();
	if(t <= endtime)
	{
		int dt = checked_cast<int>(ch::ToMilliseconds(t - starttime));
		for(int x = 0; x < DISPLAY_WIDTH; x++)
		{
			int offset = std::max(dt - offsets[x], 0) / MELT_SPEED;
			for(int y = 0; y < (DISPLAY_HEIGHT - offset); y++)
				canvas.SetPixel(x, y + offset, meltcanvas.GetPixel(x, y));
		}
	}
}
