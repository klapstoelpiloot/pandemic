#pragma once
#include "Tools.h"
#include "IRenderer.h"
#include "GameDefines.h"

class ScreenDissolveRenderer final : public virtual IRenderer
{
private:

	// Members
	const Image& noise;
	Canvas oldcanvas;
	TimePoint starttime;
	TimePoint endtime;

public:

	ScreenDissolveRenderer();

	virtual void Render(Canvas& canvas) override;
	void Begin();
	bool IsFinished() const { return (Clock::now() > endtime); }
};
