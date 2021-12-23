#include "SystemTestRenderer.h"
#include "File.h"
#include "Main.h"

#define OVERLAY_ALPHA	150

SystemTestRenderer::SystemTestRenderer() :
	backgroundimage(Main::GetResources().GetImage("test_back.dds")),
	circleimage(Main::GetResources().GetImage("circle4.dds")),
	darkcircleimage(Main::GetResources().GetImage("circle4dark.dds")),
	inputcount(0),
	inputcounttext("0", Main::GetResources().Smallest(), HorizontalAlign::Left, VerticalAlign::Bottom),
	gate1front(false),
	gate2front(false),
	gate3front(false),
	gate4front(false),
	gate1back(false),
	gate2back(false),
	gate3back(false),
	gate4back(false),
	bridge1sensor(false),
	bridge2sensor(false),
	leftbutton(false),
	rightbutton(false),
	acceptbutton(false),
	cancelbutton(false)
{
}

void SystemTestRenderer::Begin()
{
	inputcount = 0;
	inputcounttext.SetText("0");
}

void SystemTestRenderer::Render(Canvas& canvas)
{
	canvas.DrawColorImage(Point(0, 0), backgroundimage);

	canvas.DrawMonoImageBlend(Point(4, 3), darkcircleimage, BLACK);
	canvas.DrawMonoImageBlend(Point(4, 8), darkcircleimage, BLACK);
	canvas.DrawMonoImageBlend(Point(4, 14), darkcircleimage, BLACK);
	canvas.DrawMonoImageBlend(Point(4, 19), darkcircleimage, BLACK);
	canvas.DrawMonoImageBlend(Point(24, 3), darkcircleimage, BLACK);
	canvas.DrawMonoImageBlend(Point(30, 3), darkcircleimage, BLACK);
	canvas.DrawMonoImageBlend(Point(92, 2), darkcircleimage, BLACK);
	canvas.DrawMonoImageBlend(Point(92, 8), darkcircleimage, BLACK);
	canvas.DrawMonoImageBlend(Point(92, 14), darkcircleimage, BLACK);
	canvas.DrawMonoImageBlend(Point(92, 20), darkcircleimage, BLACK);
	canvas.DrawMonoImageBlend(Point(101, 2), darkcircleimage, BLACK);
	canvas.DrawMonoImageBlend(Point(101, 8), darkcircleimage, BLACK);
	canvas.DrawMonoImageBlend(Point(101, 14), darkcircleimage, BLACK);
	canvas.DrawMonoImageBlend(Point(101, 20), darkcircleimage, BLACK);

	if(leftbutton) canvas.DrawMonoImageBlend(Point(4, 3), circleimage, YELLOW);
	if(rightbutton) canvas.DrawMonoImageBlend(Point(4, 8), circleimage, YELLOW);
	if(acceptbutton) canvas.DrawMonoImageBlend(Point(4, 14), circleimage, GREEN);
	if(cancelbutton) canvas.DrawMonoImageBlend(Point(4, 19), circleimage, RED);
	if(bridge1sensor) canvas.DrawMonoImageBlend(Point(24, 3), circleimage, WHITE);
	if(bridge2sensor) canvas.DrawMonoImageBlend(Point(30, 3), circleimage, WHITE);
	if(gate1front) canvas.DrawMonoImageBlend(Point(92, 2), circleimage, WHITE);
	if(gate2front) canvas.DrawMonoImageBlend(Point(92, 8), circleimage, WHITE);
	if(gate3front) canvas.DrawMonoImageBlend(Point(92, 14), circleimage, WHITE);
	if(gate4front) canvas.DrawMonoImageBlend(Point(92, 20), circleimage, WHITE);
	if(gate1back) canvas.DrawMonoImageBlend(Point(101, 2), circleimage, WHITE);
	if(gate2back) canvas.DrawMonoImageBlend(Point(101, 8), circleimage, WHITE);
	if(gate3back) canvas.DrawMonoImageBlend(Point(101, 14), circleimage, WHITE);
	if(gate4back) canvas.DrawMonoImageBlend(Point(101, 20), circleimage, WHITE);

	inputcounttext.DrawMask(canvas, Point(30, 25), WHITE);
}

void SystemTestRenderer::CountInput()
{
	inputcount++;
	inputcounttext.SetText(String::From(inputcount));
}