#include "Canvas.h"
#include "lodepng.h"
#include "File.h"

Canvas::Canvas()
{
}

Canvas::~Canvas()
{
}

void Canvas::Clear(Color color)
{
	for(int i = 0; i < (DISPLAY_WIDTH * DISPLAY_HEIGHT); i++)
		renderbuffer[i] = color;
}

void Canvas::DrawRectangle(Point p1, Point p2, Color linecolor, Color fillcolor)
{
	// Make sure that p1 is at the left-top (lowest coordinates)
	// and p2 at the right-bottom (highest coordinates)
	if(p1.x > p2.x) std::swap(p1.x, p2.x);
	if(p1.y > p2.y) std::swap(p1.y, p2.y);

	// Completely outside view?
	if((p1.x >= DISPLAY_WIDTH) || (p1.y >= DISPLAY_HEIGHT) || (p2.x < 0) || (p2.y < 0))
		return;

	Point cp1 = p1.Clip(DISPLAY_WIDTH, DISPLAY_HEIGHT);
	Point cp2 = p2.Clip(DISPLAY_WIDTH, DISPLAY_HEIGHT);

	// Draw the border lines
	if((p1.x >= 0) && (p1.x < DISPLAY_WIDTH))
	{
		for(int y = cp1.y; y <= cp2.y; y++)
			renderbuffer[y * DISPLAY_WIDTH + p1.x] = linecolor;
	}
	if((p2.x >= 0) && (p2.x < DISPLAY_WIDTH))
	{
		for(int y = cp1.y; y <= cp2.y; y++)
			renderbuffer[y * DISPLAY_WIDTH + p2.x] = linecolor;
	}
	if((p1.y >= 0) && (p1.y < DISPLAY_HEIGHT))
	{
		for(int x = cp1.x; x <= cp2.x; x++)
			renderbuffer[p1.y * DISPLAY_WIDTH + x] = linecolor;
	}
	if((p2.y >= 0) && (p2.y < DISPLAY_HEIGHT))
	{
		for(int x = cp1.x; x <= cp2.x; x++)
			renderbuffer[p2.y * DISPLAY_WIDTH + x] = linecolor;
	}

	// Shrink by 1 pixel on each side and fill this area
	cp1 = p1.Offset(1, 1).Clip(DISPLAY_WIDTH, DISPLAY_HEIGHT);
	cp2 = p2.Offset(-1, -1).Clip(DISPLAY_WIDTH, DISPLAY_HEIGHT);
	for(int y = cp1.y; y <= cp2.y; y++)
	{
		for(int x = cp1.x; x <= cp2.x; x++)
			renderbuffer[y * DISPLAY_WIDTH + x] = fillcolor;
	}
}

void Canvas::DrawRectangleBlend(Point p1, Point p2, Color linecolor, Color fillcolor)
{
	// Make sure that p1 is at the left-top (lowest coordinates)
	// and p2 at the right-bottom (highest coordinates)
	if(p1.x > p2.x) std::swap(p1.x, p2.x);
	if(p1.y > p2.y) std::swap(p1.y, p2.y);

	// Completely outside view?
	if((p1.x >= DISPLAY_WIDTH) || (p1.y >= DISPLAY_HEIGHT) || (p2.x < 0) || (p2.y < 0))
		return;

	Point cp1 = p1.Clip(DISPLAY_WIDTH, DISPLAY_HEIGHT);
	Point cp2 = p2.Clip(DISPLAY_WIDTH, DISPLAY_HEIGHT);

	// Draw the border lines
	if((p1.x >= 0) && (p1.x < DISPLAY_WIDTH))
	{
		for(int y = cp1.y; y <= cp2.y; y++)
			renderbuffer[y * DISPLAY_WIDTH + p1.x].Blend(linecolor);
	}
	if((p2.x >= 0) && (p2.x < DISPLAY_WIDTH))
	{
		for(int y = cp1.y; y <= cp2.y; y++)
			renderbuffer[y * DISPLAY_WIDTH + p2.x].Blend(linecolor);
	}
	if((p1.y >= 0) && (p1.y < DISPLAY_HEIGHT))
	{
		for(int x = cp1.x; x <= cp2.x; x++)
			renderbuffer[p1.y * DISPLAY_WIDTH + x].Blend(linecolor);
	}
	if((p2.y >= 0) && (p2.y < DISPLAY_HEIGHT))
	{
		for(int x = cp1.x; x <= cp2.x; x++)
			renderbuffer[p2.y * DISPLAY_WIDTH + x].Blend(linecolor);
	}

	// Shrink by 1 pixel on each side and fill this area
	cp1 = p1.Offset(1, 1).Clip(DISPLAY_WIDTH, DISPLAY_HEIGHT);
	cp2 = p2.Offset(-1, -1).Clip(DISPLAY_WIDTH, DISPLAY_HEIGHT);
	for(int y = cp1.y; y <= cp2.y; y++)
	{
		for(int x = cp1.x; x <= cp2.x; x++)
			renderbuffer[y * DISPLAY_WIDTH + x].Blend(fillcolor);
	}
}

bool Canvas::PrepareImageDraw(Point pos, const IImage& img, Rect& imgrect, Rect& drawrect)
{
	if((imgrect.width <= 0) || (imgrect.height <= 0))
		return false;

	// Image rectangle valid?
	REQUIRE(imgrect.x >= 0);
	REQUIRE(imgrect.y >= 0);
	REQUIRE(imgrect.x < img.Width());
	REQUIRE(imgrect.y < img.Height());
	REQUIRE(imgrect.Right() < img.Width());
	REQUIRE(imgrect.Bottom() < img.Height());

	// Determine right-bottom pixel on target
	Point lastpos = pos.Offset(imgrect.GetSize()).Offset(-1, -1);

	// Completely outside view?
	if((pos.x >= DISPLAY_WIDTH) || (pos.y >= DISPLAY_HEIGHT) || (lastpos.x < 0) || (lastpos.y < 0))
		return false;

	// Determine clipped target area and image area
	Point drawlefttop = pos.Clip(DISPLAY_WIDTH, DISPLAY_HEIGHT);
	imgrect.x += drawlefttop.x - pos.x;
	imgrect.y += drawlefttop.y - pos.y;
	Point drawrightbottom = lastpos.Clip(DISPLAY_WIDTH, DISPLAY_HEIGHT);
	drawrect = Rect(drawlefttop, Size(drawrightbottom.x - drawlefttop.x, drawrightbottom.y - drawlefttop.y));

	// OK to draw this image
	return true;
}

void Canvas::DrawColorImage(Point pos, const IImage& img, Rect imgrect)
{
	REQUIRE(img.HasColors());

	Rect drawrect;
	if(!PrepareImageDraw(pos, img, imgrect, drawrect))
		return;

	// Draw image
	ColorSampler sampler = img.GetColorSampler();
	for(int y = 0; y <= drawrect.height; y++)
	{
		for(int x = 0; x <= drawrect.width; x++)
			SetPixel(drawrect.x + x, drawrect.y + y, sampler(imgrect.x + x, imgrect.y + y));
	}
}

void Canvas::DrawColorImageBlend(Point pos, const IImage& img, Rect imgrect)
{
	REQUIRE(img.HasColors());

	Rect drawrect;
	if(!PrepareImageDraw(pos, img, imgrect, drawrect))
		return;

	// Draw image
	ColorSampler sampler = img.GetColorSampler();
	for(int y = 0; y <= drawrect.height; y++)
	{
		for(int x = 0; x <= drawrect.width; x++)
			BlendPixel(drawrect.x + x, drawrect.y + y, sampler(imgrect.x + x, imgrect.y + y));
	}
}

void Canvas::DrawColorImageAdd(Point pos, const IImage& img, Rect imgrect)
{
	REQUIRE(img.HasColors());

	Rect drawrect;
	if(!PrepareImageDraw(pos, img, imgrect, drawrect))
		return;

	// Draw image
	ColorSampler sampler = img.GetColorSampler();
	for(int y = 0; y <= drawrect.height; y++)
	{
		for(int x = 0; x <= drawrect.width; x++)
			AddPixel(drawrect.x + x, drawrect.y + y, sampler(imgrect.x + x, imgrect.y + y));
	}
}

void Canvas::DrawColorImageMask(Point pos, const IImage& img, Rect imgrect)
{
	REQUIRE(img.HasColors());

	Rect drawrect;
	if(!PrepareImageDraw(pos, img, imgrect, drawrect))
		return;

	// Draw image
	ColorSampler sampler = img.GetColorSampler();
	for(int y = 0; y <= drawrect.height; y++)
	{
		for(int x = 0; x <= drawrect.width; x++)
			MaskPixel(drawrect.x + x, drawrect.y + y, sampler(imgrect.x + x, imgrect.y + y));
	}
}

void Canvas::DrawMonoImage(Point pos, const IImage& img, Color color, Rect imgrect)
{
	REQUIRE(img.HasColors() == false);

	Rect drawrect;
	if(!PrepareImageDraw(pos, img, imgrect, drawrect))
		return;

	// Draw image
	MonoSampler sampler = img.GetMonoSampler();
	for(int y = 0; y <= drawrect.height; y++)
	{
		for(int x = 0; x <= drawrect.width; x++)
		{
			Color c = color;
			c.ModulateRGBA(sampler(imgrect.x + x, imgrect.y + y));
			SetPixel(drawrect.x + x, drawrect.y + y, c);
		}
	}
}

void Canvas::DrawMonoImageBlend(Point pos, const IImage& img, Color color, Rect imgrect)
{
	REQUIRE(img.HasColors() == false);

	Rect drawrect;
	if(!PrepareImageDraw(pos, img, imgrect, drawrect))
		return;

	// Draw image
	Color c = color;
	MonoSampler sampler = img.GetMonoSampler();
	for(int y = 0; y <= drawrect.height; y++)
	{
		for(int x = 0; x <= drawrect.width; x++)
		{
			uint imagealpha = static_cast<uint>(sampler(imgrect.x + x, imgrect.y + y));
			c.a = static_cast<byte>((static_cast<uint>(color.a) * imagealpha) / 255u);
			BlendPixel(drawrect.x + x, drawrect.y + y, c);
		}
	}
}

void Canvas::DrawMonoImageAdd(Point pos, const IImage& img, Color color, Rect imgrect)
{
	REQUIRE(img.HasColors() == false);

	Rect drawrect;
	if(!PrepareImageDraw(pos, img, imgrect, drawrect))
		return;

	// Draw image
	Color c = color;
	MonoSampler sampler = img.GetMonoSampler();
	for(int y = 0; y <= drawrect.height; y++)
	{
		for(int x = 0; x <= drawrect.width; x++)
		{
			uint imagealpha = static_cast<uint>(sampler(imgrect.x + x, imgrect.y + y));
			c.a = static_cast<byte>((static_cast<uint>(color.a) * imagealpha) / 255u);
			AddPixel(drawrect.x + x, drawrect.y + y, c);
		}
	}
}

void Canvas::DrawMonoImageMask(Point pos, const IImage& img, Color color, Rect imgrect)
{
	REQUIRE(img.HasColors() == false);

	Rect drawrect;
	if(!PrepareImageDraw(pos, img, imgrect, drawrect))
		return;

	// Draw image
	Color c = color;
	MonoSampler sampler = img.GetMonoSampler();
	for(int y = 0; y <= drawrect.height; y++)
	{
		for(int x = 0; x <= drawrect.width; x++)
		{
			c.a = sampler(imgrect.x + x, imgrect.y + y);
			MaskPixel(drawrect.x + x, drawrect.y + y, c);
		}
	}
}

void Canvas::DrawMonoTextured(Point pos, const IImage& img, const IImage& tex, Point texoffset, Rect imgrect)
{
	REQUIRE(img.HasColors() == false);
	REQUIRE(tex.HasColors() == true);

	Rect origimgrect = imgrect;
	Rect drawrect;
	if(!PrepareImageDraw(pos, img, imgrect, drawrect))
		return;

	// Adjust textoffset by the same amount as imgrect was adjusted
	texoffset.x += imgrect.x - origimgrect.x;
	texoffset.y += imgrect.y - origimgrect.y;

	// Draw image
	MonoSampler imgsampler = img.GetMonoSampler();
	ColorSampler texsampler = tex.GetColorSampler();
	for(int y = 0; y <= drawrect.height; y++)
	{
		for(int x = 0; x <= drawrect.width; x++)
		{
			int tx = (texoffset.x + x) % tex.Width();
			int ty = (texoffset.y + y) % tex.Height();
			Color c = Color(texsampler(tx, ty), imgsampler(imgrect.x + x, imgrect.y + y));
			SetPixel(drawrect.x + x, drawrect.y + y, c);
		}
	}
}

void Canvas::DrawMonoTexturedMask(Point pos, const IImage& img, const IImage& tex, Point texoffset, Rect imgrect)
{
	REQUIRE(img.HasColors() == false);
	REQUIRE(tex.HasColors() == true);

	Rect origimgrect = imgrect;
	Rect drawrect;
	if(!PrepareImageDraw(pos, img, imgrect, drawrect))
		return;

	// Adjust textoffset by the same amount as imgrect was adjusted
	texoffset.x += imgrect.x - origimgrect.x;
	texoffset.y += imgrect.y - origimgrect.y;

	// Draw image
	MonoSampler imgsampler = img.GetMonoSampler();
	ColorSampler texsampler = tex.GetColorSampler();
	for(int y = 0; y <= drawrect.height; y++)
	{
		for(int x = 0; x <= drawrect.width; x++)
		{
			int tx = (texoffset.x + x) % tex.Width();
			int ty = (texoffset.y + y) % tex.Height();
			Color c = Color(texsampler(tx, ty), imgsampler(imgrect.x + x, imgrect.y + y));
			MaskPixel(drawrect.x + x, drawrect.y + y, c);
		}
	}
}

void Canvas::DrawMonoTexturedBlend(Point pos, const IImage& img, const IImage& tex, Point texoffset, Rect imgrect)
{
	REQUIRE(img.HasColors() == false);
	REQUIRE(tex.HasColors() == true);

	Rect origimgrect = imgrect;
	Rect drawrect;
	if(!PrepareImageDraw(pos, img, imgrect, drawrect))
		return;

	// Adjust textoffset by the same amount as imgrect was adjusted
	texoffset.x += imgrect.x - origimgrect.x;
	texoffset.y += imgrect.y - origimgrect.y;

	// Draw image
	MonoSampler imgsampler = img.GetMonoSampler();
	ColorSampler texsampler = tex.GetColorSampler();
	for(int y = 0; y <= drawrect.height; y++)
	{
		for(int x = 0; x <= drawrect.width; x++)
		{
			int tx = (texoffset.x + x) % tex.Width();
			int ty = (texoffset.y + y) % tex.Height();
			Color c = Color(texsampler(tx, ty), imgsampler(imgrect.x + x, imgrect.y + y));
			BlendPixel(drawrect.x + x, drawrect.y + y, c);
		}
	}
}

void Canvas::DrawMonoTexturedAdd(Point pos, const IImage& img, const IImage& tex, Point texoffset, Rect imgrect)
{
	REQUIRE(img.HasColors() == false);
	REQUIRE(tex.HasColors() == true);

	Rect origimgrect = imgrect;
	Rect drawrect;
	if(!PrepareImageDraw(pos, img, imgrect, drawrect))
		return;

	// Adjust textoffset by the same amount as imgrect was adjusted
	texoffset.x += imgrect.x - origimgrect.x;
	texoffset.y += imgrect.y - origimgrect.y;

	// Draw image
	MonoSampler imgsampler = img.GetMonoSampler();
	ColorSampler texsampler = tex.GetColorSampler();
	for(int y = 0; y <= drawrect.height; y++)
	{
		for(int x = 0; x <= drawrect.width; x++)
		{
			int tx = (texoffset.x + x) % tex.Width();
			int ty = (texoffset.y + y) % tex.Height();
			Color c = Color(texsampler(tx, ty), imgsampler(imgrect.x + x, imgrect.y + y));
			AddPixel(drawrect.x + x, drawrect.y + y, c);
		}
	}
}

void Canvas::DrawMonoTexturedMod(Point pos, const IImage& img, const IImage& tex, Color mod, Point texoffset, Rect imgrect)
{
	REQUIRE(img.HasColors() == false);
	REQUIRE(tex.HasColors() == true);

	Rect origimgrect = imgrect;
	Rect drawrect;
	if(!PrepareImageDraw(pos, img, imgrect, drawrect))
		return;

	// Adjust textoffset by the same amount as imgrect was adjusted
	texoffset.x += imgrect.x - origimgrect.x;
	texoffset.y += imgrect.y - origimgrect.y;

	// Draw image
	MonoSampler imgsampler = img.GetMonoSampler();
	ColorSampler texsampler = tex.GetColorSampler();
	for(int y = 0; y <= drawrect.height; y++)
	{
		for(int x = 0; x <= drawrect.width; x++)
		{
			int tx = (texoffset.x + x) % tex.Width();
			int ty = (texoffset.y + y) % tex.Height();
			Color c = Color(texsampler(tx, ty), imgsampler(imgrect.x + x, imgrect.y + y));
			c.ModulateRGBA(mod);
			SetPixel(drawrect.x + x, drawrect.y + y, c);
		}
	}
}

void Canvas::DrawMonoTexturedModMask(Point pos, const IImage& img, const IImage& tex, Color mod, Point texoffset, Rect imgrect)
{
	REQUIRE(img.HasColors() == false);
	REQUIRE(tex.HasColors() == true);

	Rect origimgrect = imgrect;
	Rect drawrect;
	if(!PrepareImageDraw(pos, img, imgrect, drawrect))
		return;

	// Adjust textoffset by the same amount as imgrect was adjusted
	texoffset.x += imgrect.x - origimgrect.x;
	texoffset.y += imgrect.y - origimgrect.y;

	// Draw image
	MonoSampler imgsampler = img.GetMonoSampler();
	ColorSampler texsampler = tex.GetColorSampler();
	for(int y = 0; y <= drawrect.height; y++)
	{
		for(int x = 0; x <= drawrect.width; x++)
		{
			int tx = (texoffset.x + x) % tex.Width();
			int ty = (texoffset.y + y) % tex.Height();
			Color c = Color(texsampler(tx, ty), imgsampler(imgrect.x + x, imgrect.y + y));
			c.ModulateRGBA(mod);
			MaskPixel(drawrect.x + x, drawrect.y + y, c);
		}
	}
}

void Canvas::DrawMonoTexturedModBlend(Point pos, const IImage& img, const IImage& tex, Color mod, Point texoffset, Rect imgrect)
{
	REQUIRE(img.HasColors() == false);
	REQUIRE(tex.HasColors() == true);

	Rect origimgrect = imgrect;
	Rect drawrect;
	if(!PrepareImageDraw(pos, img, imgrect, drawrect))
		return;

	// Adjust textoffset by the same amount as imgrect was adjusted
	texoffset.x += imgrect.x - origimgrect.x;
	texoffset.y += imgrect.y - origimgrect.y;

	// Draw image
	MonoSampler imgsampler = img.GetMonoSampler();
	ColorSampler texsampler = tex.GetColorSampler();
	for(int y = 0; y <= drawrect.height; y++)
	{
		for(int x = 0; x <= drawrect.width; x++)
		{
			int tx = (texoffset.x + x) % tex.Width();
			int ty = (texoffset.y + y) % tex.Height();
			Color c = Color(texsampler(tx, ty), imgsampler(imgrect.x + x, imgrect.y + y));
			c.ModulateRGBA(mod);
			BlendPixel(drawrect.x + x, drawrect.y + y, c);
		}
	}
}

void Canvas::DrawMonoTexturedModAdd(Point pos, const IImage& img, const IImage& tex, Color mod, Point texoffset, Rect imgrect)
{
	REQUIRE(img.HasColors() == false);
	REQUIRE(tex.HasColors() == true);

	Rect origimgrect = imgrect;
	Rect drawrect;
	if(!PrepareImageDraw(pos, img, imgrect, drawrect))
		return;

	// Adjust textoffset by the same amount as imgrect was adjusted
	texoffset.x += imgrect.x - origimgrect.x;
	texoffset.y += imgrect.y - origimgrect.y;

	// Draw image
	MonoSampler imgsampler = img.GetMonoSampler();
	ColorSampler texsampler = tex.GetColorSampler();
	for(int y = 0; y <= drawrect.height; y++)
	{
		for(int x = 0; x <= drawrect.width; x++)
		{
			int tx = (texoffset.x + x) % tex.Width();
			int ty = (texoffset.y + y) % tex.Height();
			Color c = Color(texsampler(tx, ty), imgsampler(imgrect.x + x, imgrect.y + y));
			c.ModulateRGBA(mod);
			AddPixel(drawrect.x + x, drawrect.y + y, c);
		}
	}
}

void Canvas::WriteToFile(String filename) const
{
	vector<byte> recordbuffer;
	lodepng::encode(recordbuffer, reinterpret_cast<const unsigned char*>(renderbuffer), DISPLAY_WIDTH, DISPLAY_HEIGHT);
	lodepng::save_file(recordbuffer, filename.stl());
}
