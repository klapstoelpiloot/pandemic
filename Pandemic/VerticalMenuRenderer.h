#pragma once
#include "Tools.h"
#include "IPositionalRenderer.h"

class VerticalMenuRenderer final : public virtual IRenderer
{
private:

	// Items in the menu
	vector<IPositionalRenderer*> items;

	// Current position for the first item
	int firstposition;

	// Animation status
	byte opacity;
	bool isanimating;
	bool movingup;
	int movestartpos;
	TimePoint beginmovetime;

	// Methods
	int DetermineTargetPos();

public:

	VerticalMenuRenderer();

	virtual void Render(Canvas& canvas) override;
	void AddItem(IPositionalRenderer* renderer);
	void RemoveItem(IPositionalRenderer* renderer);
	void SetOpacity(byte _opacity) { opacity = _opacity; }
};
