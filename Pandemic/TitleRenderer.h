#pragma once
#include "Tools.h"
#include "IRenderer.h"
#include "Text.h"
#include "Sound.h"

class TitleRenderer final : public virtual IRenderer
{
private:

	// Resources
	const Image& titleimage;
	Text pressbuttontext;
	Text creditstext;

	// State
	TimePoint titlestarttime;
	TimePoint creditstarttime;
	bool showkeyinfo;
	bool asbackground;
	uint creditindex;
	std::function<void()> playtitlemusic;

	// Methods
	void RenderTitleColumn(Canvas& canvas, int x, int img_y, int img_len, int canvas_y, int canvas_len, byte mod);

public:

	TitleRenderer();
	
	void BeginFirstStart();
	void BeginReturnToTitle();
	virtual void Render(Canvas& canvas) override;
	void SetBackgroundMode(bool asbackground) { this->asbackground = asbackground; }
	void SetPlayTitleMusicFunction(std::function<void()> playtitlemusicfunc) { playtitlemusic = playtitlemusicfunc; }
};
