#pragma once
#include "Tools.h"
#include "IRenderer.h"
#include "Text.h"

class EnterNameRenderer final : public virtual IRenderer
{
private:

	// Text to render
	Text toptext;
	Text bottomtext;
	Text nametext;
	Text editchar;
	Text greenbuttontext;
	Text redbuttontext;
	const Image& texture;
	const Image& darktexture;
	const Image& greenbutton;
	const Image& redbutton;
	TimePoint starttime;

	// Animation
	bool flashvalue;

public:

	EnterNameRenderer();

	virtual void Render(Canvas& canvas) override;
	void SetName(String name);
	void SetEditChar(String c);
};
