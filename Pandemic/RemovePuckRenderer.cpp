#include "RemovePuckRenderer.h"
#include "Main.h"

#define BIG_TEXT_Y			12
#define BOTTOM_TEXT_Y		28
#define CENTER_TEXT_X		64

RemovePuckRenderer::RemovePuckRenderer() :
	bigcross(Main::GetResources().GetImage("bigcross.dds")),
	bigtext("REMOVE PUCKS", Main::GetResources().BoldBits(), HorizontalAlign::Center, VerticalAlign::Middle),
	bottomtext("ON AND BEFORE THE LINE", Main::GetResources().Smallest(), HorizontalAlign::Center, VerticalAlign::Bottom),
	texture(Main::GetResources().GetImage("yellow12.dds"))
{
}

void RemovePuckRenderer::Render(Canvas& canvas)
{
	canvas.DrawColorImageMask(Point(0, 0), bigcross);
	bigtext.DrawOutlineMask(canvas, Point(CENTER_TEXT_X, BIG_TEXT_Y), 1, BLACK);
	bottomtext.DrawOutlineMask(canvas, Point(CENTER_TEXT_X, BOTTOM_TEXT_Y), 1, BLACK);
	bigtext.DrawTexturedMask(canvas, Point(CENTER_TEXT_X, BIG_TEXT_Y), texture);
	bottomtext.DrawMask(canvas, Point(CENTER_TEXT_X, BOTTOM_TEXT_Y), WHITE);
}
