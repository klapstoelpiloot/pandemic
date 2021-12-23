#pragma once
#include "IRenderer.h"

class ProgressBar : virtual public IRenderer
{
private:

	float progress;
	byte alpha;

public:

	ProgressBar();

	virtual void Render(Canvas& canvas) override;
	void SetProgress(float percent) { progress = percent; }
	void SetTranslucency(byte a) { alpha = a; }
};
