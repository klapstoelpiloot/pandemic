#include "ProgressBar.h"
#include <cmath>

#define BAR_LEFT		8
#define BAR_RIGHT		119
#define BAR_TOP			15
#define BAR_LINES		2
#define BAR_BACK_COLOR	Color(50, 0, 0)
#define BAR_FILL_COLOR	Color(255, 0, 0)

ProgressBar::ProgressBar() :
	progress(0.0f),
	alpha(255)
{
}

void ProgressBar::Render(Canvas& canvas)
{
	// Calculate total length of the bar.
	// The left and right are inclusive.
	int maxlen = (BAR_RIGHT - BAR_LEFT) + 2;

	// Calculate actual length.
	int barlen = std::clamp(static_cast<int>(std::ceil(static_cast<float>(maxlen) * progress)), 1, maxlen);

	// Draw the pixels
	Color fillcolor = BAR_FILL_COLOR;
	fillcolor.a = alpha;
	for(int x = 0; x < barlen; x++)
	{
		for(int y = 0; y < BAR_LINES; y++)
			canvas.BlendPixel(BAR_LEFT + x, BAR_TOP + y, fillcolor);
	}

	// Draw remaining pixels dark
	Color backcolor = BAR_BACK_COLOR;
	backcolor.a = alpha;
	for(int x = 0; x < (maxlen - barlen); x++)
	{
		for(int y = 0; y < BAR_LINES; y++)
			canvas.BlendPixel(BAR_LEFT + barlen + x, BAR_TOP + y, backcolor);
	}
}
