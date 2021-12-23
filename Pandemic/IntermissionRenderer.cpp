#include "IntermissionRenderer.h"
#include "Main.h"

#define ROUND_TEXT_Y		15
#define BIG_TEXT_MIDDLE_Y	15
#define BIG_TEXT_TOP_Y		9
#define BIG_TEXT_BOTTOM_Y	22
#define TOP_TEXT_Y			-1
#define BOTTOM_TEXT_Y		32
#define CENTER_TEXT_X		64

IntermissionRenderer::IntermissionRenderer() :
	roundtext(Main::GetResources().BoldBitsLarge(), HorizontalAlign::Center, VerticalAlign::Middle),
	bigtext1(Main::GetResources().BoldBits(), HorizontalAlign::Center, VerticalAlign::Middle),
	bigtext2(Main::GetResources().BoldBits(), HorizontalAlign::Center, VerticalAlign::Middle),
	toptext(Main::GetResources().Smallest(), HorizontalAlign::Center, VerticalAlign::Top),
	bottomtext(Main::GetResources().Smallest(), HorizontalAlign::Center, VerticalAlign::Bottom),
	texture(Main::GetResources().GetImage("yellow12.dds")),
	bigtexture(Main::GetResources().GetImage("yellow22d.dds")),
	bigtext1pos(0),
	bigtext2pos(0)
{
}

void IntermissionRenderer::Render(Canvas& canvas)
{
	roundtext.DrawOutlineMask(canvas, Point(CENTER_TEXT_X, ROUND_TEXT_Y), 2, BLACK);
	bigtext1.DrawOutlineMask(canvas, Point(CENTER_TEXT_X, bigtext1pos), 1, BLACK);
	bigtext2.DrawOutlineMask(canvas, Point(CENTER_TEXT_X, bigtext2pos), 1, BLACK);
	toptext.DrawOutlineMask(canvas, Point(CENTER_TEXT_X, TOP_TEXT_Y), 1, BLACK);
	bottomtext.DrawOutlineMask(canvas, Point(CENTER_TEXT_X, BOTTOM_TEXT_Y), 1, BLACK);

	roundtext.DrawTexturedMask(canvas, Point(CENTER_TEXT_X, ROUND_TEXT_Y), bigtexture);
	bigtext1.DrawTexturedMask(canvas, Point(CENTER_TEXT_X, bigtext1pos), texture);
	bigtext2.DrawTexturedMask(canvas, Point(CENTER_TEXT_X, bigtext2pos), texture);
	toptext.DrawMask(canvas, Point(CENTER_TEXT_X, TOP_TEXT_Y), WHITE);
	bottomtext.DrawMask(canvas, Point(CENTER_TEXT_X, BOTTOM_TEXT_Y), WHITE);

	roundflasher.Draw(canvas, roundtext, Point(CENTER_TEXT_X, ROUND_TEXT_Y), 2);
}

void IntermissionRenderer::ShowRoundComplete(int index)
{
	Clear();
	bigtext1.SetText("ROUND " + String::From(index));
	bigtext2.SetText("COMPLETED");
	bigtext1pos = BIG_TEXT_TOP_Y;
	bigtext2pos = BIG_TEXT_BOTTOM_Y;
}

void IntermissionRenderer::ShowRound(int index)
{
	Clear();
	roundtext.SetText("ROUND " + String::From(index));
	roundflasher.Begin();
}

void IntermissionRenderer::ShowPucksLeft(int pucks, bool lastround)
{
	Clear();
	toptext.SetText("CLEAR THE BOARD AND TAKE");
	bigtext1.SetText(String::From(pucks) + " PUCKS");
	if(lastround)
		bottomtext.SetText("FOR THE LAST ROUND");
	else
		bottomtext.SetText("FOR THE NEXT ROUND");
	bigtext1pos = BIG_TEXT_MIDDLE_Y;
}

void IntermissionRenderer::Clear()
{
	roundtext.SetText("");
	bigtext1.SetText("");
	bigtext2.SetText("");
	toptext.SetText("");
	bottomtext.SetText("");
}
