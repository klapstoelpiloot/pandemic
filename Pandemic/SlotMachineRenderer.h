#pragma once
#include "Tools.h"
#include "IRenderer.h"
#include "Text.h"
#include "TextShine.h"

#define NUM_WHEELS			3
#define NUM_ICONS			10

class SlotMachineRenderer final : public virtual IRenderer
{
private:

	// Members
	const Image& wheelimage;
	const Image* icons[NUM_ICONS];
	float wheelposition[NUM_WHEELS];
	float wheelspeed[NUM_WHEELS];

public:

	SlotMachineRenderer();

	// Methods
	virtual void Render(Canvas& canvas) override final;
	void Start();
	void SetWheelPosition(int wheel, float pos, bool rolling);
};
