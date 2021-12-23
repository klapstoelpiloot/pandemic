#pragma once
#include "Tools.h"
#include "IPositionalRenderer.h"
#include "Text.h"

class MenuItemRenderer final : public virtual IPositionalRenderer
{
private:

	// Text to render
	Text text;
	const Image& texture;

	// Position to render the text at
	Point position;

public:

	MenuItemRenderer(String name);

	virtual void Render(Canvas& canvas) override;
	virtual void SetPosition(Point pos) override { position = pos; }
	void SetName(String name) { text.SetText(name); }
};
