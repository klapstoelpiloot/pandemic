#pragma once
#include "Tools.h"
#include "IRenderer.h"
#include "Text.h"
#include "TextFlasher.h"

class IntermissionRenderer final : public virtual IRenderer
{
private:

	// Members
	Text roundtext;
	Text bigtext1;
	Text bigtext2;
	Text toptext;
	Text bottomtext;
	TextFlasher roundflasher;
	const Image& texture;
	const Image& bigtexture;
	int bigtext1pos;
	int bigtext2pos;

public:

	IntermissionRenderer();

	virtual void Render(Canvas& canvas) override;
	void ShowRoundComplete(int index);
	void ShowRound(int index);
	void ShowPucksLeft(int pucks, bool lastround);
	void Clear();
};
