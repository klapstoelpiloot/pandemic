#pragma once
#include "Tools.h"
#include "Text.h"
#include "ParticleEffect.h"
#include "IAnimationRenderer.h"
#include "ParticleOverlayRenderer.h"

class SetAnimation final : public virtual IAnimationRenderer
{
private:

	// Members
	ParticleOverlayRenderer& particlesoverlay;
	ParticleEffect particles;
	ParticleEffect sparks;
	Text settext;
	const Image& texture;
	TimePoint starttime;
	int index;

	// Methods
	void SpawnParticle(Color color, int yoffset);

public:

	SetAnimation(ParticleOverlayRenderer& particlesoverlay);
	virtual ~SetAnimation();

	// Methods
	virtual void Render(Canvas& canvas) override final;
	void SetIndex(int _index) { index = _index; }
	virtual void Start() override final;
	virtual bool HasFinished() override final { return !ch::IsTimeSet(starttime); }
};
