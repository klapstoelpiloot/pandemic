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
	offset = tweeny::from(-SHINE_WIDTH).to(textsize.width + textsize.height).during(300).via(easing::cubicInOut);
	lasttime = Clock::now();
}

void TextShine::DrawShine(Canvas& canvas, Rect tr)
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

	// Draw the shine only where text pixels are.
	Size canvassize = tempcanvas.GetSize();
	for(int sy = 0; sy < tr.height; sy++)
	{
		for(int sx = 0; sx < SHINE_WIDTH; sx++)
		{
			int x = tr.x + offset.peek() + sx - sy;
			int y = tr.y + sy;
			if((x >= 0) && (x < canvassize.width) && (y >= 0) && (y < canvassize.height))
			{
				if(tempcanvas.GetPixel(x, y).a > 0)
					canvas.SetPixel(x, y, WHITE);
			}
		}
	}
}

void TextShine::Draw(Canvas& canvas, const Text& text, int outline, Point pos)
{
	// Draw the text on a separate canvas.
	tempcanvas.Clear(Color(0, 0, 0, 0));
	text.DrawOutlineMask(tempcanvas, pos, outline, WHITE);
	text.DrawMask(tempcanvas, pos, WHITE);
	DrawShine(canvas, text.GetTextRect(pos));
}

void TextShine::Draw(Canvas& canvas, const IImage& image, Point pos)
{
	// Draw the image on a separate canvas.
	tempcanvas.Clear(Color(0, 0, 0, 0));
	if(image.HasColors())
		tempcanvas.DrawColorImageMask(pos, image);
	else
		tempcanvas.DrawMonoImageMask(pos, image, WHITE);
	DrawShine(canvas, Rect(pos, image.GetSize()));
}
