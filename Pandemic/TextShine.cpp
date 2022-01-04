#include "TextShine.h"

//
//  0                                                           Text width
//  |                                                                    |
//   ____________________________________________________________________
//  |     /                           /                                  |
//  |    /                           /                                   |
//  |   /                           /                                    |
//  |  /                           /                                     |
//  | /                           /                                      |
//  |/___________________________/_______________________________________|
// 
//  |                           |
//  0                      SHINE_WIDTH
//

#define SHINE_WIDTH		32

TextShine::TextShine()
{
}

void TextShine::Begin(Size textsize)
{
	offset = tweeny::from(-SHINE_WIDTH).to(textsize.width + textsize.height / 2).during(300).via(easing::sinusoidalInOut);
	lasttime = Clock::now();
}

void TextShine::Draw(Canvas& canvas, const Text& text, Point pos)
{
	if(!ch::IsTimeSet(lasttime) || (offset.progress() >= 1.0f))
	{
		lasttime = TimePoint();
		return;
	}

	// Advance time
	TimePoint t = Clock::now();
	int dt = static_cast<int>(ch::ToMilliseconds(t - lasttime));
	lasttime = t;
	offset.step(dt);

	// Draw the text on a separate canvas.
	tempcanvas.Clear(BLACK);
	text.DrawMask(tempcanvas, pos, WHITE);

	// Draw the shine only where text pixels are.
	Rect tr = text.GetTextRect(pos);
	Size canvassize = tempcanvas.GetSize();
	for(int sy = 0; sy < tr.height; sy++)
	{
		for(int sx = 0; sx < SHINE_WIDTH; sx++)
		{
			int x = tr.x + offset.peek() + sx - sy;
			int y = tr.y + sy;
			if((x >= 0) && (x < canvassize.width) && (y >= 0) && (y < canvassize.height))
			{
				if(tempcanvas.GetPixel(x, y).r > 0)
					canvas.SetPixel(x, y, WHITE);
			}
		}
	}
}
