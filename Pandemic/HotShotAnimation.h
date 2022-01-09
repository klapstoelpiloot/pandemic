#pragma once
#include "Tools.h"
#include "Text.h"
#include "ParticleEffect.h"
#include "IAnimationRenderer.h"
#include "ParticleOverlayRenderer.h"

class HotShotAnimation final : public virtual IAnimationRenderer
{
private:

	// Members
	Canvas textmask;
	ParticleOverlayRenderer& particlesoverlay;
	ParticleEffect particles;
	Text text1;
	Text text2;
	TweenXY text1pos;
	TweenXY text2pos;
	TimePoint starttime;
	TimePoint laststeptime;
	TimePoint lastparticletime;

public:

	HotShotAnimation(ParticleOverlayRenderer& particlesoverlay);
	virtual ~HotShotAnimation();

	// Methods
	virtual void Render(Canvas& canvas) override final;
	virtual void Start() override final;
	virtual bool HasFinished() override final { return !ch::IsTimeSet(starttime); }
};
