#include "IntermissionRenderer.h"
#include "GameDefines.h"
#include "Main.h"

#define ROUND_TEXT_Y		15
#define BIG_TEXT_MIDDLE_Y	15
#define BIG_TEXT_TOP_Y		9
#define BIG_TEXT_BOTTOM_Y	22
#define SMALL_TEXT_Y1		-1
#define SMALL_TEXT_Y2		32
#define SMALL_TEXT_Y3		8
#define CENTER_TEXT_X		64
#define GREEN_BUTTON_X		41
#define GREEN_BUTTON_Y		24
#define TEXT_FLASH_INTERVAL	400
#define ROUND_FLASH_TIME	800
#define ROUND_ANI_DURATION	900

IntermissionRenderer::IntermissionRenderer() :
	roundtext(Main::GetResources().BoldBitsLarge(), HorizontalAlign::Center, VerticalAlign::Middle),
	bigtext1(Main::GetResources().BoldBits(), HorizontalAlign::Center, VerticalAlign::Middle),
	bigtext2(Main::GetResources().BoldBits(), HorizontalAlign::Center, VerticalAlign::Middle),
	smalltext1(Main::GetResources().Smallest(), HorizontalAlign::Center, VerticalAlign::Top),
	smalltext2(Main::GetResources().Smallest(), HorizontalAlign::Center, VerticalAlign::Bottom),
	texture(Main::GetResources().GetImage("yellow12.dds")),
	bigtexture(Main::GetResources().GetImage("yellow22d.dds")),
	bigtext1pos(0),
	bigtext2pos(0),
	smalltext1pos(0),
	smalltext2pos(0),
	greenbutton(Main::GetResources().GetImage("button_green.dds")),
	round(0),
	flashsmalltext1(false),
	flashsmalltext2(false),
	showgreenbutton(false),
	flashgreenbutton(false),
	pucksleft(0)
{
}

void IntermissionRenderer::Render(Canvas& canvas)
{
	int sp = DISPLAY_WIDTH / 2;
	TimePoint t = Clock::now();
	if(ch::IsTimeSet(starttime))
	{
		int dt = static_cast<int>(ch::ToMilliseconds(t - laststeptime));
		laststeptime = t;
		sp = roundprogress.step(dt);
		if(t >= (starttime + ch::milliseconds(roundprogress.duration())))
		{
			sp = DISPLAY_WIDTH / 2;
			roundflasher.Begin(ROUND_FLASH_TIME);
			starttime = TimePoint();
		}
	}

	if(roundtext.GetText().Length() > 0)
	{
		tempcanvas.Clear(Color(0, 0, 0, 0));
		roundtext.DrawOutlineMask(tempcanvas, Point(CENTER_TEXT_X, ROUND_TEXT_Y), 2, BLACK);
		roundtext.DrawTexturedMask(tempcanvas, Point(CENTER_TEXT_X, ROUND_TEXT_Y), bigtexture);
		roundflasher.Draw(tempcanvas, roundtext, Point(CENTER_TEXT_X, ROUND_TEXT_Y), 2);
		canvas.DrawColorImageMask(Point(DISPLAY_WIDTH / 2 - sp, 0), tempcanvas, Rect(DISPLAY_WIDTH / 2 - sp, 0, sp * 2, DISPLAY_HEIGHT));
	}

	bool flash = (((ch::ToMilliseconds(t) / TEXT_FLASH_INTERVAL) % 2) == 0);

	bigtext1.DrawOutlineMask(canvas, Point(CENTER_TEXT_X, bigtext1pos), 1, BLACK);
	bigtext2.DrawOutlineMask(canvas, Point(CENTER_TEXT_X, bigtext2pos), 1, BLACK);

	if(flash || !flashsmalltext1)
		smalltext1.DrawOutlineMask(canvas, Point(CENTER_TEXT_X, smalltext1pos), 1, BLACK);

	if(flash || !flashsmalltext2)
		smalltext2.DrawOutlineMask(canvas, Point(CENTER_TEXT_X, smalltext2pos), 1, BLACK);

	bigtext1.DrawTexturedMask(canvas, Point(CENTER_TEXT_X, bigtext1pos), texture);
	bigtext2.DrawTexturedMask(canvas, Point(CENTER_TEXT_X, bigtext2pos), texture);

	if(flash || !flashsmalltext1)
		smalltext1.DrawMask(canvas, Point(CENTER_TEXT_X, smalltext1pos), WHITE);

	if(flash || !flashsmalltext2)
		smalltext2.DrawMask(canvas, Point(CENTER_TEXT_X, smalltext2pos), WHITE);

	if(showgreenbutton && (!flashgreenbutton || flash))
		canvas.DrawColorImageMask(Point(GREEN_BUTTON_X, GREEN_BUTTON_Y), greenbutton);

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
	starttime = Clock::now();
	laststeptime = Clock::now();
	roundtext.SetText("ROUND " + String::From(index));
	roundprogress = tweeny::from(0).to(DISPLAY_WIDTH / 2).during(ROUND_ANI_DURATION).via(easing::cubicInOut);
}

void IntermissionRenderer::ShowPucksLeft(GameData& gd)
{
	bool isbonusround = false;
	round = gd.CurrentRoundNumber();
	pucksleft = gd.CalculateNextRoundPucks(&isbonusround);

	Clear();
	smalltext1.SetText("CLEAR THE BOARD AND TAKE");
	bigtext1.SetText(String::From(pucksleft) + " PUCKS");
	if(gd.IsLastRound())
		smalltext2.SetText("FOR THE LAST ROUND");
	else
		smalltext2.SetText("FOR THE NEXT ROUND");
	smalltext1pos = SMALL_TEXT_Y1;
	smalltext2pos = SMALL_TEXT_Y2;
	bigtext1pos = BIG_TEXT_MIDDLE_Y;
}

void IntermissionRenderer::FlashGreenForNextRound()
{
	flashsmalltext2 = true;
	showgreenbutton = true;
	flashgreenbutton = true;
	smalltext2.SetText("PRESS    TO START ROUND");
}

void IntermissionRenderer::ShowPucksBlockingBridge()
{
	Clear();
	smalltext1.SetText("CHECK BRIDGE AND GATES");
	showgreenbutton = true;
	smalltext2.SetText("PRESS    TO START ROUND");
	smalltext1pos = SMALL_TEXT_Y3;
	smalltext2pos = SMALL_TEXT_Y2;
}

void IntermissionRenderer::ShowPucksBlockingGates(bool gates[4])
{
	ShowPucksBlockingBridge();
}

void IntermissionRenderer::Clear()
{
	starttime = TimePoint();
	laststeptime = TimePoint();
	flashsmalltext1 = false;
	flashsmalltext2 = false;
	showgreenbutton = false;
	flashgreenbutton = false;
	roundtext.SetText("");
	bigtext1.SetText("");
	bigtext2.SetText("");
	smalltext1.SetText("");
	smalltext2.SetText("");
}
