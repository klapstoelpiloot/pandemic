#pragma once
#include "Tools.h"
#include "IRenderer.h"
#include "Text.h"
#include "ParticleEffect.h"

class ClassicSetRenderer final : public virtual IRenderer
{
private:

	// Members
	ParticleEffect& particles;
	ParticleEffect& sparks;
	Text settext;
	const Image& texture;
	TimePoint starttime;
	bool visible;

	// Methods
	void SpawnParticle(Color color, int yoffset);

public:

	ClassicSetRenderer(ParticleEffect& particleeffect, ParticleEffect& sparkseffect);

	// Methods
	virtual void Render(Canvas& canvas) override;
	void ShowSet(int index);
	bool IsVisible() const { return visible; }
};
