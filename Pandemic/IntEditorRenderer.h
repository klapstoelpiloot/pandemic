#pragma once
#include "Tools.h"
#include "IPositionalRenderer.h"
#include "Text.h"

class IntEditorRenderer final : public virtual IPositionalRenderer
{
private:

	// Text to render
	Text nametext;
	Text valuetext;
	const Image& texture;
	const Image& darktexture;
	String valuepostfix;
	TimePoint starttime;

	// Position to render the text at
	Point position;
	int valuespacing;

	// Animation
	bool flashvalue;

public:

	IntEditorRenderer(String name, String postfix, int spacing);

	virtual void Render(Canvas& canvas) override;
	virtual void SetPosition(Point pos) override { position = pos; }
	void SetValue(int v);
	void FlashValue(bool flash) { flashvalue = flash; }
};
