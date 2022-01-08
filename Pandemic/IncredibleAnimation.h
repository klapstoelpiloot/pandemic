#pragma once
#include "Tools.h"
#include "Text.h"
#include "ParticleEffect.h"
#include "IAnimationRenderer.h"
#include "ParticleOverlayRenderer.h"
#include "TextFlasher.h"

class IncredibleAnimation final : public virtual IAnimationRenderer
{
private:

	// Members
	Text character[10];
	Point charpos[10];
	TextFlasher flasher;
	TweenInt offset;
	const Image& texture;
	TimePoint starttime;
	TimePoint flashtime;
	TimePoint laststeptime;

public:

	IncredibleAnimation(ParticleOverlayRenderer& particlesoverlay);
	virtual ~IncredibleAnimation();

	// Methods
	virtual void Render(Canvas& canvas) override final;
	virtual void Start() override final;
	virtual bool HasFinished() override final { return !ch::IsTimeSet(starttime); }
};
