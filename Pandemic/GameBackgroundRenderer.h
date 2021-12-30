#pragma once
#include "Tools.h"
#include "IRenderer.h"
#include "GameDefines.h"

class GameBackgroundRenderer final : public virtual IRenderer
{
private:

	// Members
	Canvas backbuffer;
	TimePoint starttime;
	float timesec;
	int temporalditheroffset;
	int colorindex;

	// Methods
	float PixelShader(float x, float y);

public:

	GameBackgroundRenderer();

	virtual void Render(Canvas& canvas) override;
	void Begin();
	void SetNextColor();
	void SetPrevColor();
};
