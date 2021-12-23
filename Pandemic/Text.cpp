#include "Text.h"

Text::Text() :
	font(nullptr)
{
}

Text::Text(const Font& font, HorizontalAlign horizontalalign, VerticalAlign verticalalign) :
	font(&font),
	alignh(horizontalalign),
	alignv(verticalalign)
{
}

Text::Text(const String& text, const Font& font, HorizontalAlign horizontalalign, VerticalAlign verticalalign) :
	text(text),
	font(&font),
	alignh(horizontalalign),
	alignv(verticalalign)
{
	Update();
}

Text::~Text()
{
}

void Text::Update()
{
	chars.clear();

	if(font == nullptr)
	{
		textsize = Size(0, 0);
		return;
	}

	if(text.IsEmpty())
	{
		textsize = Size(0, font->LineHeight());
		return;
	}

	// Make all characters
	int xpos = 0;
	int maxh = 0;
	for(byte c : text.stl())
	{
		FontChar fc = font->GetCharacter(c);
		TextChar tc;
		tc.imgrect = fc.imgrect;
		tc.position = Point(xpos + fc.offset.x, fc.offset.y);
		xpos += fc.advance;
		maxh = std::max(maxh, fc.imgrect.height);
		chars.push_back(tc);
	}

	// Keep the size of our text
	textsize = Size(xpos, font->LineHeight());

	// Adjust offsets for alignment
	Size offsetadjust;
	switch(alignh)
	{
		case HorizontalAlign::Left: offsetadjust.width = 0; break;
		case HorizontalAlign::Right: offsetadjust.width = -textsize.width; break;
		case HorizontalAlign::Center: offsetadjust.width = -(textsize.width / 2); break;
		default: NOT_IMPLEMENTED;
	}
	switch(alignv)
	{
		case VerticalAlign::Top: offsetadjust.height = 0; break;
		case VerticalAlign::Bottom: offsetadjust.height = -textsize.height; break;
		case VerticalAlign::Middle: offsetadjust.height = -(textsize.height / 2); break;
		default: NOT_IMPLEMENTED;
	}
	for(TextChar& tc : chars)
		tc.position = tc.position.Offset(offsetadjust);
}

void Text::DrawOpaque(Canvas& canvas, Point pos, Color c) const
{
	if(text.IsEmpty() || (font == nullptr))
		return;

	const Image& img = font->GetImage();
	if(img.HasColors())
	{
		for(const TextChar& tc : chars)
			canvas.DrawColorImage(tc.position.Offset(pos.x, pos.y), img, tc.imgrect);
	}
	else
	{
		for(const TextChar& tc : chars)
			canvas.DrawMonoImage(tc.position.Offset(pos.x, pos.y), img, c, tc.imgrect);
	}
}

void Text::DrawBlend(Canvas& canvas, Point pos, Color c) const
{
	if(text.IsEmpty() || (font == nullptr))
		return;

	const Image& img = font->GetImage();
	if(img.HasColors())
	{
		for(const TextChar& tc : chars)
			canvas.DrawColorImageBlend(tc.position.Offset(pos.x, pos.y), img, tc.imgrect);
	}
	else
	{
		for(const TextChar& tc : chars)
			canvas.DrawMonoImageBlend(tc.position.Offset(pos.x, pos.y), img, c, tc.imgrect);
	}
}

void Text::DrawAdd(Canvas& canvas, Point pos, Color c) const
{
	if(text.IsEmpty() || (font == nullptr))
		return;

	const Image& img = font->GetImage();
	if(img.HasColors())
	{
		for(const TextChar& tc : chars)
			canvas.DrawColorImageAdd(tc.position.Offset(pos.x, pos.y), img, tc.imgrect);
	}
	else
	{
		for(const TextChar& tc : chars)
			canvas.DrawMonoImageAdd(tc.position.Offset(pos.x, pos.y), img, c, tc.imgrect);
	}
}

void Text::DrawMask(Canvas& canvas, Point pos, Color c) const
{
	if(text.IsEmpty() || (font == nullptr))
		return;

	const Image& img = font->GetImage();
	if(img.HasColors())
	{
		for(const TextChar& tc : chars)
			canvas.DrawColorImageMask(tc.position.Offset(pos.x, pos.y), img, tc.imgrect);
	}
	else
	{
		for(const TextChar& tc : chars)
			canvas.DrawMonoImageMask(tc.position.Offset(pos.x, pos.y), img, c, tc.imgrect);
	}
}

void Text::DrawTexturedMask(Canvas& canvas, Point pos, const Image& tex, Point texoffset) const
{
	if(text.IsEmpty() || (font == nullptr))
		return;

	const Image& img = font->GetImage();
	REQUIRE(!img.HasColors());

	for(const TextChar& tc : chars)
		canvas.DrawMonoTexturedMask(tc.position.Offset(pos.x, pos.y), img, tex, texoffset, tc.imgrect);
}

void Text::DrawTexturedAdd(Canvas& canvas, Point pos, const Image& tex, Point texoffset) const
{
	if(text.IsEmpty() || (font == nullptr))
		return;

	const Image& img = font->GetImage();
	REQUIRE(!img.HasColors());

	for(const TextChar& tc : chars)
		canvas.DrawMonoTexturedAdd(tc.position.Offset(pos.x, pos.y), img, tex, texoffset, tc.imgrect);
}

void Text::DrawShadowMask(Canvas& canvas, Point pos, int distance, Color c) const
{
	DrawMask(canvas, Point(pos.x + distance, pos.y), c);
	DrawMask(canvas, Point(pos.x + distance, pos.y + distance), c);
	DrawMask(canvas, Point(pos.x, pos.y + distance), c);
}

void Text::DrawOutlineMask(Canvas& canvas, Point pos, int distance, Color c) const
{
	int diagdist = (distance > 1) ? distance - 1 : distance;
	DrawMask(canvas, Point(pos.x + distance, pos.y), c);
	DrawMask(canvas, Point(pos.x + diagdist, pos.y + diagdist), c);
	DrawMask(canvas, Point(pos.x, pos.y + distance), c);
	DrawMask(canvas, Point(pos.x - diagdist, pos.y + diagdist), c);
	DrawMask(canvas, Point(pos.x - distance, pos.y), c);
	DrawMask(canvas, Point(pos.x - diagdist, pos.y - diagdist), c);
	DrawMask(canvas, Point(pos.x, pos.y - distance), c);
	DrawMask(canvas, Point(pos.x + diagdist, pos.y - diagdist), c);
}

void Text::DrawOutlineBlend(Canvas& canvas, Point pos, int distance, Color c) const
{
	int diagdist = (distance > 1) ? distance - 1 : distance;
	DrawBlend(canvas, Point(pos.x + distance, pos.y), c);
	DrawBlend(canvas, Point(pos.x + diagdist, pos.y + diagdist), c);
	DrawBlend(canvas, Point(pos.x, pos.y + distance), c);
	DrawBlend(canvas, Point(pos.x - diagdist, pos.y + diagdist), c);
	DrawBlend(canvas, Point(pos.x - distance, pos.y), c);
	DrawBlend(canvas, Point(pos.x - diagdist, pos.y - diagdist), c);
	DrawBlend(canvas, Point(pos.x, pos.y - distance), c);
	DrawBlend(canvas, Point(pos.x + diagdist, pos.y - diagdist), c);
}
