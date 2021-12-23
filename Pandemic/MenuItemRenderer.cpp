#include "MenuItemRenderer.h"
#include "Main.h"

MenuItemRenderer::MenuItemRenderer(String name) :
	text(name, Main::GetResources().BoldBits(), HorizontalAlign::Center, VerticalAlign::Middle),
	texture(Main::GetResources().GetImage("yellow12.dds"))
{
}

void MenuItemRenderer::Render(Canvas& canvas)
{
	text.DrawOutlineMask(canvas, position, 2, BLACK);
	text.DrawTexturedMask(canvas, position, texture);
}
