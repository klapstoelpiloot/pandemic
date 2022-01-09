#pragma once
#include "Tools.h"
#include "Text.h"
#include "ParticleEffect.h"
#include "IAnimationRenderer.h"
#include "ParticleOverlayRenderer.h"
#include "TextFlasher.h"

class ExcellentAnimation final : public virtual IAnimationRenderer
{
private:

	// Members
	Canvas tempcanvas;
	Text text;
	TweenFloat burnprogress;
	const Image& burnmap;
	const Image& burntex;
	const Image& burncolors;
	TimePoint starttime;
	TimePoint laststeptime;

public:

	ExcellentAnimation(ParticleOverlayRenderer& particlesoverlay);
	virtual ~ExcellentAnimation();

	// Methods
	virtual void Render(Canvas& canvas) override final;
	virtual void Start() override final;
	virtual bool HasFinished() override final { return !ch::IsTimeSet(starttime); }
};
