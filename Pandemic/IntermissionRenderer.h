#pragma once
#include "Tools.h"
#include "IRenderer.h"
#include "Text.h"
#include "TextFlasher.h"
#include "GameData.h"

class IntermissionRenderer final : public virtual IRenderer
{
private:

	// Members
	Canvas tempcanvas;
	Text roundtext;
	Text bigtext1;
	Text bigtext2;
	Text smalltext1;
	Text smalltext2;
	TextFlasher roundflasher;
	TweenInt roundprogress;
	TimePoint starttime;
	TimePoint laststeptime;
	const Image& texture;
	const Image& bigtexture;
	int bigtext1pos;
	int bigtext2pos;
	int smalltext1pos;
	int smalltext2pos;
	const Image& greenbutton;
	int round;
	bool flashsmalltext1;
	bool flashsmalltext2;
	bool showgreenbutton;
	bool flashgreenbutton;
	int pucksleft;

public:

	IntermissionRenderer();

	virtual void Render(Canvas& canvas) override;
	void ShowRoundComplete(int index);
	void ShowRound(int index);
	void ShowPucksLeft(GameData& gd);
	void FlashGreenForNextRound();
	void ShowPucksBlockingBridge();
	void ShowPucksBlockingGates(bool gates[4]);
	void Clear();
};
