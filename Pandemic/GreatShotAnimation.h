#pragma once
#include "Tools.h"
#include "Text.h"
#include "ParticleEffect.h"
#include "IAnimationRenderer.h"
#include "ParticleOverlayRenderer.h"
#include "TextShine.h"

class GreatShotAnimation final : public virtual IAnimationRenderer
{
private:

	// Members
	Text text1;
	Text text2;
	TextShine shine1;
	TextShine shine2;
	TweenXY text1pos;
	TweenXY text2pos;
	const Image& texture;
	TimePoint starttime;
	TimePoint laststeptime;
	TimePoint lastparticletime;
	bool shinedone;

public:

	GreatShotAnimation(ParticleOverlayRenderer& particlesoverlay);
	virtual ~GreatShotAnimation();

	// Methods
	virtual void Render(Canvas& canvas) override final;
	virtual void Start() override final;
	virtual bool HasFinished() override final { return !ch::IsTimeSet(starttime); }
};
