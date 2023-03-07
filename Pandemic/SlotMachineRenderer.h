#pragma once
#include "Tools.h"
#include "IRenderer.h"
#include "Text.h"
#include "TextShine.h"

#define NUM_WHEELS			3
#define NUM_ICONS			10

enum SlotMachineSelection
{
	None = 0,
	Hold0 = 1,
	Hold1 = 2,
	Hold2 = 3,
	Spin = 4
};

class SlotMachineRenderer final : public virtual IRenderer
{
private:

	// Members
	const Image& wheelimage;
	const Image* icons[NUM_ICONS];
	const Image* bluricons[NUM_ICONS];
	const Image& arrowright;
	Canvas blurscanvas;
	float wheelposition[NUM_WHEELS];
	float wheelspeed[NUM_WHEELS];
	SlotMachineSelection selection;
	bool wheelhold[NUM_WHEELS];
	Text holdtext[NUM_WHEELS];
	Text spintext;
	Text spinslefttext;
	const Image& spinslefttexture;
	TimePoint selectionchangetime;
	TimePoint wheelstoptime[NUM_WHEELS];

public:

	SlotMachineRenderer();

	// Methods
	virtual void Render(Canvas& canvas) override final;
	void Start();
	void SetWheelPosition(int wheel, float pos, bool rolling);
	void SetSelection(SlotMachineSelection s);
	void SetHolds(bool hold[NUM_WHEELS]);
	void SetSpins(int sp);
};
