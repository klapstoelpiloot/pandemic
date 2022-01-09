#pragma once
#include "Tools.h"
#include "Text.h"
#include "ParticleEffect.h"
#include "IAnimationRenderer.h"
#include "ParticleOverlayRenderer.h"
#include "TextFlasher.h"

class ImpressiveAnimation final : public virtual IAnimationRenderer
{
private:

	// Members
	ParticleOverlayRenderer& particlesoverlay;
	ParticleEffect particles;
	Text character[10];
	Point charpos[10];
	TextFlasher flasher[10];
	TweenInt offset;
	const Image& texture;
	TimePoint starttime;
	TimePoint nextflashtime;
	TimePoint laststeptime;
	int flashcount;

public:

	ImpressiveAnimation(ParticleOverlayRenderer& particlesoverlay);
	virtual ~ImpressiveAnimation();

	// Methods
	virtual void Render(Canvas& canvas) override final;
	virtual void Start() override final;
	virtual bool HasFinished() override final { return !ch::IsTimeSet(starttime); }
};
