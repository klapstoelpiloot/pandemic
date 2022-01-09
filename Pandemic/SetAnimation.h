#pragma once
#include "Tools.h"
#include "Text.h"
#include "ParticleEffect.h"
#include "IAnimationRenderer.h"
#include "ParticleOverlayRenderer.h"
#include "TextShine.h"

class SetAnimation final : public virtual IAnimationRenderer
{
private:

	// Members
	ParticleOverlayRenderer& particlesoverlay;
	ParticleEffect particles;
	ParticleEffect sparks;
	Text settext;
	Text nextset;
	Text prevset;
	const Image& texture;
	TweenInt numberoffset;
	TextShine textshine;
	TextShine setshine;
	TimePoint starttime;
	TimePoint bangtime;
	TimePoint laststeptime;
	TimePoint setshinetime;
	int index;

	// Methods
	void SpawnParticle(Color color, int x, int y);

public:

	SetAnimation(ParticleOverlayRenderer& particlesoverlay);
	virtual ~SetAnimation();

	// Methods
	virtual void Render(Canvas& canvas) override final;
	void SetIndex(int _index) { index = _index; }
	virtual void Start() override final;
	virtual bool HasFinished() override final { return !ch::IsTimeSet(starttime); }
};
