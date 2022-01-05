#pragma once
#include "Tools.h"
#include "Text.h"
#include "ParticleEffect.h"
#include "IAnimationRenderer.h"
#include "ParticleOverlayRenderer.h"
#include "TextFlasher.h"

class MegaShotAnimation final : public virtual IAnimationRenderer
{
private:

	// Members
	ParticleOverlayRenderer& particlesoverlay;
	ParticleEffect particles;
	Canvas tempcanvas;
	Text text;
	TweenInt textsize;
	TextFlasher flasher;
	const Image& texture;
	TimePoint starttime;
	TimePoint laststeptime;
	TimePoint nextflashtime;
	int flashcount;

public:

	MegaShotAnimation(ParticleOverlayRenderer& particlesoverlay);
	virtual ~MegaShotAnimation();

	// Methods
	virtual void Render(Canvas& canvas) override final;
	virtual void Start() override final;
	virtual bool HasFinished() override final { return !ch::IsTimeSet(starttime); }
};
