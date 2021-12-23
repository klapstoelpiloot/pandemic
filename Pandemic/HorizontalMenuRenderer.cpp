#include <cmath>
#include "HorizontalMenuRenderer.h"
#include "Main.h"

#define ITEM_SPACING		128
#define VERTICAL_MIDDLE		16
#define HORIZONTAL_MIDDLE	64
#define PIXELS_PER_MS		0.8f
#define ARROW_FLASH_RATE	300
#define ARROW_Y_OFFSET		-4
#define ARROW_LEFT_POS		-1
#define ARROW_RIGHT_POS		121

HorizontalMenuRenderer::HorizontalMenuRenderer() :
	currentindex(0),
	verticalposition(VERTICAL_MIDDLE),
	currentposition(HORIZONTAL_MIDDLE),
	isanimating(false),
	nextindex(0),
	movingleft(false),
	showarrows(false),
	arrowleft(Main::GetResources().GetImage("arrowleft.dds")),
	arrowright(Main::GetResources().GetImage("arrowright.dds"))
{
}

void HorizontalMenuRenderer::Render(Canvas& canvas)
{
	if(isanimating)
	{
		// Calculate position in animation
		int direction = movingleft ? -1 : 1;
		float duration = static_cast<float>(ToMilliseconds(Clock::now() - beginmovetime));
		currentposition = HORIZONTAL_MIDDLE + static_cast<int>(std::round(duration * PIXELS_PER_MS)) * direction;

		// Past our target position? Then finish animation.
		if((movingleft && (currentposition < (HORIZONTAL_MIDDLE - ITEM_SPACING))) ||
		   (!movingleft && (currentposition > (HORIZONTAL_MIDDLE + ITEM_SPACING))))
		{
			currentposition = HORIZONTAL_MIDDLE;
			currentindex = nextindex;
			isanimating = false;
		}
	}

	// Draw direction arrows
	int64_t time = ToMilliseconds(Clock::now());
	if(showarrows && ((time / ARROW_FLASH_RATE) % 2 == 0))
	{
		canvas.DrawColorImageBlend(Point(ARROW_LEFT_POS, verticalposition + ARROW_Y_OFFSET), arrowleft);
		canvas.DrawColorImageBlend(Point(ARROW_RIGHT_POS, verticalposition + ARROW_Y_OFFSET), arrowright);
	}

	// Render current item
	items[currentindex]->SetPosition(Point(currentposition, verticalposition));
	items[currentindex]->Render(canvas);

	// Render next item
	if(isanimating)
	{
		int nextitemposition = movingleft ? (currentposition + ITEM_SPACING) : (currentposition - ITEM_SPACING);
		items[nextindex]->SetPosition(Point(nextitemposition, verticalposition));
		items[nextindex]->Render(canvas);
	}
}

void HorizontalMenuRenderer::MoveToIndex(int index, bool scrollleft)
{
	currentindex = index;
	nextindex = index;
	currentposition = HORIZONTAL_MIDDLE;
	beginmovetime = Clock::now();
	movingleft = scrollleft;
	isanimating = true;
}

void HorizontalMenuRenderer::SetAtIndex(int index)
{
	currentindex = index;
	currentposition = HORIZONTAL_MIDDLE;
	isanimating = false;
}

void HorizontalMenuRenderer::AddItem(IPositionalRenderer* renderer)
{
	items.push_back(renderer);
}

void HorizontalMenuRenderer::RemoveItem()
{
	items.pop_back();
}

void HorizontalMenuRenderer::Clear()
{
	items.clear();
}
