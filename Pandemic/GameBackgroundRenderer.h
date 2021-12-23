#pragma once
#include "Tools.h"
#include "IRenderer.h"
#include "GameDefines.h"
#include "Vec3.h"

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
	Vec3 Hash33(Vec3 p);
	float Voronoi(Vec3 p);
	float NoiseLayers(Vec3 p);
	float PixelShader(float x, float y);

public:

	GameBackgroundRenderer();

	virtual void Render(Canvas& canvas) override;
	void Begin();
	void SetNextColor();
	void SetPrevColor();
};
