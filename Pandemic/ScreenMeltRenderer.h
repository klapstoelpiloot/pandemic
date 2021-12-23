#pragma once
#include "Tools.h"
#include "IRenderer.h"
#include "GameDefines.h"

class ScreenMeltRenderer final : public virtual IRenderer
{
private:

	// Members
	Canvas meltcanvas;
	std::array<int, DISPLAY_WIDTH> offsets;
	TimePoint starttime;
	TimePoint endtime;

public:

	ScreenMeltRenderer();

	virtual void Render(Canvas& canvas) override;
	void Begin();
	bool IsFinished() const { return (Clock::now() > endtime); }
};
