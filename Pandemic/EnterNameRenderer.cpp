#include "EnterNameRenderer.h"
#include "Main.h"

#define TOP_TEXT_POS	Point(64, -1)
#define GREEN_TEXT_POS	Point(13, 32)
#define GREEN_IMAGE_POS	Point(3, 24)
#define RED_TEXT_POS	Point(100, 32)
#define RED_IMAGE_POS	Point(90, 24)
#define NAME_TEXT_X		29
#define NAME_TEXT_Y		15
#define TEXTURE_FLASH_INTERVAL	200

EnterNameRenderer::EnterNameRenderer() :
	toptext("ENTER YOUR NAME:", Main::GetResources().Smallest(), HorizontalAlign::Center, VerticalAlign::Top),
	nametext(Main::GetResources().BoldBits(), HorizontalAlign::Left, VerticalAlign::Middle),
	editchar(Main::GetResources().BoldBits(), HorizontalAlign::Left, VerticalAlign::Middle),
	greenbuttontext("= ADD LETTER", Main::GetResources().Smallest(), HorizontalAlign::Left, VerticalAlign::Bottom),
	redbuttontext("= EXIT", Main::GetResources().Smallest(), HorizontalAlign::Left, VerticalAlign::Bottom),
	texture(Main::GetResources().GetImage("yellow12.dds")),
	darktexture(Main::GetResources().GetImage("brown12.dds")),
	greenbutton(Main::GetResources().GetImage("button_green.dds")),
	redbutton(Main::GetResources().GetImage("button_red.dds"))
{
}

void EnterNameRenderer::Render(Canvas& canvas)
{
	int editcharpos = NAME_TEXT_X + nametext.GetTextSize().width;
	bool flashbright = (ch::ToMilliseconds(Clock::now() - starttime) % (TEXTURE_FLASH_INTERVAL << 1) < TEXTURE_FLASH_INTERVAL);
	const Image& tex = flashbright ? texture : darktexture;

	toptext.DrawOutlineMask(canvas, TOP_TEXT_POS, 1, BLACK);
	nametext.DrawOutlineMask(canvas, Point(NAME_TEXT_X, NAME_TEXT_Y), 1, BLACK);
	editchar.DrawOutlineMask(canvas, Point(editcharpos, NAME_TEXT_Y), 1, BLACK);
	greenbuttontext.DrawOutlineMask(canvas, GREEN_TEXT_POS, 1, BLACK);
	redbuttontext.DrawOutlineMask(canvas, RED_TEXT_POS, 1, BLACK);

	canvas.DrawColorImageBlend(GREEN_IMAGE_POS, greenbutton);
	canvas.DrawColorImageBlend(RED_IMAGE_POS, redbutton);

	toptext.DrawMask(canvas, TOP_TEXT_POS, WHITE);
	nametext.DrawTexturedMask(canvas, Point(NAME_TEXT_X, NAME_TEXT_Y), texture);
	editchar.DrawTexturedMask(canvas, Point(editcharpos, NAME_TEXT_Y), tex);
	greenbuttontext.DrawMask(canvas, GREEN_TEXT_POS, WHITE);
	redbuttontext.DrawMask(canvas, RED_TEXT_POS, WHITE);
}

void EnterNameRenderer::SetName(String name)
{
	starttime = Clock::now();
	nametext.SetText(name);
}

void EnterNameRenderer::SetEditChar(String c)
{
	editchar.SetText(c);
}
