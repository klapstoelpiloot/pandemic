#include "SlotMachineRenderer.h"
#include "Main.h"
#include <algorithm>
#include <iterator>
#include <algorithm>

#define WHEEL0_X				0
#define ICON0_X					1
#define WHEEL_X_DISTANCE		34
#define ICON_Y_DISTANCE			20
#define ICON_Y					6
#define SELECTION_BAR_Y			25
#define SELECTION_BAR_COLOR		Color(BLACK, 200)
#define SELECTION_TEXT_Y		33
#define HOLD0_TEXT_X			(WHEEL0_X + 18)
#define HOLD_DISABLED_COLOR		Color(100, 100, 0)
#define HOLD_ENABLED_COLOR		YELLOW
#define SPIN_COLOR				YELLOW
#define SELECTION_FLASH_RATE	200
#define SPIN_TEXT_POS			Point(HOLD0_TEXT_X + WHEEL_X_DISTANCE * 3 - 2, SELECTION_TEXT_Y)
#define SELECTION_ARROW_Y		25
#define SELECTION_OFFSET_X		1
#define BLUR_START_SPEED		0.2
#define BLUR_FULL_SPEED			0.5

SlotMachineRenderer::SlotMachineRenderer() :
	wheelimage(Main::GetResources().GetImage("slotwheel.dds")),
	arrowright(Main::GetResources().GetImage("arrowright_small.dds")),
	wheelposition { 0.0f, 0.0f, 0.0f },
	wheelspeed { 0.0f, 0.0f, 0.0f },
	wheelhold { false, false, false },
	holdtext {
		Text(Main::GetResources().Smallest(), HorizontalAlign::Center, VerticalAlign::Bottom),
		Text(Main::GetResources().Smallest(), HorizontalAlign::Center, VerticalAlign::Bottom),
		Text(Main::GetResources().Smallest(), HorizontalAlign::Center, VerticalAlign::Bottom)
	},
	spintext(Main::GetResources().Smallest(), HorizontalAlign::Center, VerticalAlign::Bottom)
{
	holdtext[0].SetText("HOLD");
	holdtext[1].SetText("HOLD");
	holdtext[2].SetText("HOLD");
	spintext.SetText("SPIN");

	for(int i = 0; i < NUM_ICONS; i++)
	{
		icons[i] = &(Main::GetResources().GetImage("slot" + String::From(i) + ".dds"));
		bluricons[i] = &(Main::GetResources().GetImage("slotblur" + String::From(i) + ".dds"));
	}

	blurscanvas.Clear(BLACK);
}

void SlotMachineRenderer::Render(Canvas& canvas)
{
	TimePoint t = Clock::now();

	canvas.Clear(BLACK);

	for(int i = 0; i < NUM_WHEELS; i++)
	{
		Point wheelpos = Point(WHEEL0_X + WHEEL_X_DISTANCE * i, 0);
		Rect wheelrect = Rect(wheelpos.x, wheelpos.y, WHEEL_X_DISTANCE, 32);
		float blur = std::clamp(static_cast<float>((wheelspeed[i] - BLUR_START_SPEED) / (BLUR_FULL_SPEED - BLUR_START_SPEED)), 0.0f, 1.0f);

		// Background
		canvas.DrawMonoImage(wheelpos, wheelimage, WHITE);
		blurscanvas.DrawMonoImage(wheelpos, wheelimage, WHITE);

		// Middle icon
		int roundedpos = static_cast<int>(roundf(wheelposition[i]));
		int icon = roundedpos % NUM_ICONS;
		int xpos = ICON0_X + WHEEL_X_DISTANCE * i;
		int ypos = ICON_Y + static_cast<int>(roundf((wheelposition[i] - static_cast<float>(roundedpos)) * ICON_Y_DISTANCE));
		if(blur < 1.0f) canvas.DrawColorImageBlend(Point(xpos, ypos), *icons[icon]);
		if(blur > 0.0f) blurscanvas.DrawColorImageBlend(Point(xpos, ypos), *bluricons[icon]);

		// Top icon
		icon = (roundedpos + 1) % NUM_ICONS;
		if(blur < 1.0f) canvas.DrawColorImageBlend(Point(xpos, ypos - ICON_Y_DISTANCE), *icons[icon]);
		if(blur > 0.0f) blurscanvas.DrawColorImageBlend(Point(xpos, ypos - ICON_Y_DISTANCE), *bluricons[icon]);

		// Bottom icon
		icon = abs((roundedpos - 1) % NUM_ICONS);
		if(blur < 1.0f) canvas.DrawColorImageBlend(Point(xpos, ypos + ICON_Y_DISTANCE), *icons[icon]);
		if(blur > 0.0f) blurscanvas.DrawColorImageBlend(Point(xpos, ypos + ICON_Y_DISTANCE), *bluricons[icon]);

		// Crossfade between normal wheel and blurred wheel
		if(blur > 0.0f)
		{
			// Set the alpha values on the pixels for this blur
			byte blurbyte = static_cast<byte>(std::clamp(static_cast<int>(blur * 255.0f), 0, 255));
			for(int y = wheelrect.Top(); y <= wheelrect.Bottom(); y++)
			{
				for(int x = wheelrect.Left(); x <= wheelrect.Right(); x++)
					blurscanvas.SetPixel(x, y, Color(blurscanvas.GetPixel(x, y), blurbyte));
			}

			// Blend image
			canvas.DrawColorImageBlend(wheelpos, blurscanvas, wheelrect);
		}
	}

	if(selection != SlotMachineSelection::None)
	{
		// Draw selection bar
		canvas.DrawRectangleBlend(Point(0, SELECTION_BAR_Y), Point(DISPLAY_WIDTH, DISPLAY_HEIGHT), SELECTION_BAR_COLOR, SELECTION_BAR_COLOR);

		// Hold texts
		for(int i = 0; i < NUM_WHEELS; i++)
		{
			Point holdpos = Point(HOLD0_TEXT_X + WHEEL_X_DISTANCE * i, SELECTION_TEXT_Y);
			holdtext[i].DrawOutlineMask(canvas, holdpos, 1, BLACK);
			if(wheelhold[i])
			{
				holdtext[i].DrawMask(canvas, holdpos, HOLD_ENABLED_COLOR);
			}
			else
			{
				holdtext[i].DrawBlend(canvas, holdpos, HOLD_DISABLED_COLOR);
			}
		}

		// Spin text
		spintext.DrawOutlineMask(canvas, SPIN_TEXT_POS, 1, BLACK);
		spintext.DrawMask(canvas, SPIN_TEXT_POS, SPIN_COLOR);

		// Selection arrow
		if((ToMilliseconds(t - selectionchangetime) / SELECTION_FLASH_RATE) % 2 == 0)
		{
			Point pos = Point(SELECTION_OFFSET_X + WHEEL_X_DISTANCE * (selection - SlotMachineSelection::Hold0), SELECTION_ARROW_Y);
			canvas.DrawColorImageMask(pos, arrowright);
		}
	}
}

void SlotMachineRenderer::Start()
{
	for(int i = 0; i < NUM_WHEELS; i++)
	{
		wheelposition[i] = 0.0f;
		wheelspeed[i] = 0.0f;
	}
}

void SlotMachineRenderer::SetWheelPosition(int wheel, float pos, bool rolling)
{
	wheelspeed[wheel] = rolling ? (pos - wheelposition[wheel]) : 0.0f;
	wheelposition[wheel] = pos;
}

void SlotMachineRenderer::SetSelection(SlotMachineSelection s)
{
	selection = s;
	selectionchangetime = Clock::now();
}

void SlotMachineRenderer::SetHolds(bool hold[NUM_WHEELS])
{
	std::copy(hold, hold + NUM_WHEELS, wheelhold);
}
