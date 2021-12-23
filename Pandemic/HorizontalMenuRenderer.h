#pragma once
#include "Tools.h"
#include "IPositionalRenderer.h"

class HorizontalMenuRenderer final : public virtual IPositionalRenderer
{
private:

	// Items in the menu
	vector<IPositionalRenderer*> items;
	int currentindex;

	// Position for the currentindex item
	int verticalposition;
	int currentposition;

	// Animation status
	bool isanimating;
	int nextindex;
	bool movingleft;
	TimePoint beginmovetime;

	// Left/right arrows
	bool showarrows;
	const Image& arrowleft;
	const Image& arrowright;

public:

	HorizontalMenuRenderer();

	virtual void Render(Canvas& canvas) override;
	virtual void SetPosition(Point pos) override { verticalposition = pos.y; }
	void MoveToIndex(int index, bool scrollleft);
	void SetAtIndex(int index);
	void AddItem(IPositionalRenderer* renderer);
	void RemoveItem();
	void Clear();
	void ShowArrows(bool show) { showarrows = show; }
};
