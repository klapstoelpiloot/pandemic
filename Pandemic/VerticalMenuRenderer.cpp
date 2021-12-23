#include "VerticalMenuRenderer.h"
#include <cmath>

#define ITEM_SPACING		14
#define VERTICAL_MIDDLE		16
#define MULTIPLE_OFFSET		22
#define HORIZONTAL_MIDDLE	64
#define PIXELS_PER_MS		0.2f

VerticalMenuRenderer::VerticalMenuRenderer() :
	firstposition(DetermineTargetPos()),
	isanimating(false),
	movingup(false),
	movestartpos(0)
{
}

void VerticalMenuRenderer::Render(Canvas& canvas)
{
	// Darken the background
	canvas.DrawRectangleBlend(Point(0, 0), Point(128, 32), Color(0, 0, 0, opacity), Color(0, 0, 0, opacity));

	int targetpos = DetermineTargetPos();
	if(firstposition != targetpos)
	{
		// Not animating or direction changed?
		bool targetisup = (targetpos < firstposition);
		if(!isanimating || (movingup != targetisup))
		{
			// Begin animating
			movestartpos = firstposition;
			movingup = targetisup;
			beginmovetime = Clock::now();
			isanimating = true;
		}
		
		// Calculate position in animation
		float duration = static_cast<float>(ToMilliseconds(Clock::now() - beginmovetime));
		int direction = movingup ? -1 : 1;
		firstposition = movestartpos + static_cast<int>(std::round(duration * PIXELS_PER_MS)) * direction;

		// Past our target position? Then stick to target position.
		if((movingup && (firstposition < targetpos)) || (!movingup && (firstposition > targetpos)))
			firstposition = targetpos;
	}

	// Render all items
	int ypos = firstposition;
	for(IPositionalRenderer* r : items)
	{
		r->SetPosition(Point(HORIZONTAL_MIDDLE, ypos));
		r->Render(canvas);
		ypos += ITEM_SPACING;
	}
}

void VerticalMenuRenderer::AddItem(IPositionalRenderer* renderer)
{
	// Add only when the renderer was not added already
	auto it = std::find(items.begin(), items.end(), renderer);
	if(it == items.end())
		items.push_back(renderer);
}

void VerticalMenuRenderer::RemoveItem(IPositionalRenderer* renderer)
{
	REQUIRE(items.size() > 0);
	REQUIRE(items.back() == renderer);
	items.pop_back();
}

int VerticalMenuRenderer::DetermineTargetPos()
{
	if(items.size() < 2)
		return VERTICAL_MIDDLE;
	else
		return MULTIPLE_OFFSET - ((items.size() - 1) * ITEM_SPACING);
}
