#include <math.h>
#include "ScreenDissolveRenderer.h"
#include "Main.h"

#define DISSOLVE_DURATION	600

ScreenDissolveRenderer::ScreenDissolveRenderer() :
	noise(Main::GetResources().GetImage("noise.dds"))
{
}

void ScreenDissolveRenderer::Begin()
{
	// Make a copy of the last rendered screen
	Main::GetGraphics().GetCanvas().CopyTo(oldcanvas);

	// Go
	starttime = Clock::now();
	endtime = starttime + ch::milliseconds(DISSOLVE_DURATION);
}

void ScreenDissolveRenderer::Render(Canvas& canvas)
{
	TimePoint t = Clock::now();
	if(t <= endtime)
	{
		int dt = checked_cast<int>(ch::ToMilliseconds(t - starttime));
		double dtp = static_cast<double>(dt) / static_cast<double>(DISSOLVE_DURATION);
		for(int y = 0; y < DISPLAY_HEIGHT; y++)
		{
			for(int x = 0; x < DISPLAY_WIDTH; x++)
			{
				Color nc = noise.GetColor(x, y);
				int n = static_cast<int>(nc.r) | (static_cast<int>(nc.g) << 8) | (static_cast<int>(nc.b) << 16);
				double pixeltimeoffset = static_cast<double>(n) / 16777215.0;
				if(dtp < pixeltimeoffset)
					canvas.SetPixel(x, y, oldcanvas.GetPixel(x, y));
			}
		}
	}
}
