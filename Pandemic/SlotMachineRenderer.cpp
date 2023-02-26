#include "SlotMachineRenderer.h"
#include "Main.h"

#define WHEEL0_X			2
#define ICON0_X				3
#define WHEEL_X_DISTANCE	34
#define ICON_Y_DISTANCE		20
#define ICON_Y				6

SlotMachineRenderer::SlotMachineRenderer() :
	wheelimage(Main::GetResources().GetImage("slotwheel.dds")),
	wheelposition { 0.0f, 0.0f, 0.0f },
	wheelspeed { 0.0f, 0.0f, 0.0f }
{
	for(int i = 0; i < NUM_ICONS; i++)
		icons[i] = &(Main::GetResources().GetImage("slot" + String::From(i) + ".dds"));
}

void SlotMachineRenderer::Render(Canvas& canvas)
{
	canvas.Clear(BLACK);

	for(int i = 0; i < NUM_WHEELS; i++)
	{
		// Background
		canvas.DrawMonoImage(Point(WHEEL0_X + WHEEL_X_DISTANCE * i, 0), wheelimage, WHITE);

		// Middle icon
		int roundedpos = static_cast<int>(roundf(wheelposition[i]));
		int icon = roundedpos % NUM_ICONS;
		int pos = ICON_Y + static_cast<int>(roundf((wheelposition[i] - static_cast<float>(roundedpos)) * ICON_Y_DISTANCE));
		canvas.DrawColorImageBlend(Point(ICON0_X + WHEEL_X_DISTANCE * i, pos), *icons[icon]);

		// Top icon
		icon = (roundedpos + 1) % NUM_ICONS;
		canvas.DrawColorImageBlend(Point(ICON0_X + WHEEL_X_DISTANCE * i, pos - ICON_Y_DISTANCE), *icons[icon]);

		// Bottom icon
		icon = abs((roundedpos - 1) % NUM_ICONS);
		canvas.DrawColorImageBlend(Point(ICON0_X + WHEEL_X_DISTANCE * i, pos + ICON_Y_DISTANCE), *icons[icon]);
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
