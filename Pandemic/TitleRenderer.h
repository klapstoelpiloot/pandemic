#pragma once
#include "Tools.h"
#include "IRenderer.h"
#include "Text.h"
#include "Sound.h"

class TitleRenderer final : public virtual IRenderer
{
private:

	// Resources
	const Image& mulimage;
	const Image& addimage;
	Canvas voronoi;
	Text pressbuttontext;
	Text creditstext;
	TweenFloat splashdistance;
	vector<Point> splashpoints;

	// State
	TimePoint titlestarttime;
	TimePoint creditstarttime;
	TimePoint laststeptime;
	bool showkeyinfo;
	bool asbackground;
	uint creditindex;
	std::function<void()> playtitlemusic;
	int temporalditheroffset;
	bool keyflashstate;

	// Methods
	void SetupSplashes();

public:

	TitleRenderer();
	
	void BeginFirstStart();
	void BeginReturnToTitle();
	virtual void Render(Canvas& canvas) override;
	void SetBackgroundMode(bool asbackground) { this->asbackground = asbackground; }
	void SetPlayTitleMusicFunction(std::function<void()> playtitlemusicfunc) { playtitlemusic = playtitlemusicfunc; }
	void SetKeyFlashState(bool state) { keyflashstate = state; }
};
