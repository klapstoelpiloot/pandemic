#include "IntEditorRenderer.h"
#include "Main.h"

#define VALUE_FLASH_RATE	200

IntEditorRenderer::IntEditorRenderer(String name, String postfix, int spacing) :
	nametext(name, Main::GetResources().BoldBits(), HorizontalAlign::Center, VerticalAlign::Middle),
	valuetext(Main::GetResources().BoldBits(), HorizontalAlign::Right, VerticalAlign::Middle),
	texture(Main::GetResources().GetImage("yellow12.dds")),
	darktexture(Main::GetResources().GetImage("brown12.dds")),
	valuepostfix(postfix),
	valuespacing(spacing),
	flashvalue(false)
{
	starttime = Clock::now();
}

void IntEditorRenderer::Render(Canvas& canvas)
{
	// Draw text
	Point namepos = Point(position.x - valuespacing, position.y);
	nametext.DrawOutlineMask(canvas, namepos, 2, BLACK);
	nametext.DrawTexturedMask(canvas, namepos, texture);

	// Draw value
	bool bright = !flashvalue || (ch::ToMilliseconds(Clock::now() - starttime) % (VALUE_FLASH_RATE << 1) < VALUE_FLASH_RATE);
	const Image& tex = bright ? texture : darktexture;
	Point valuepos = Point(position.x + (nametext.GetTextSize().width / 2) + valuespacing, position.y);
	valuetext.DrawOutlineMask(canvas, valuepos, 2, BLACK);
	valuetext.DrawTexturedMask(canvas, valuepos, tex);
}

void IntEditorRenderer::SetValue(int v)
{
	valuetext.SetText(String::From(v) + valuepostfix);
}
