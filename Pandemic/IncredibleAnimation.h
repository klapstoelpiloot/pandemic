#pragma once
#include "Tools.h"
#include "Text.h"
#include "ParticleEffect.h"
#include "IAnimationRenderer.h"
#include "ParticleOverlayRenderer.h"
#include "TextFlasher.h"
#include "TextShine.h"

class IncredibleAnimation final : public virtual IAnimationRenderer
{
private:

	// Members
	Text text;
	TextFlasher flasher;
	TextShine shine;
	TweenInt offset;
	const Image& texture;
	TimePoint starttime;
	TimePoint flashtime;
	TimePoint shinetime;
	TimePoint laststeptime;

public:

	IncredibleAnimation(ParticleOverlayRenderer& particlesoverlay);
	virtual ~IncredibleAnimation();

	// Methods
	virtual void Render(Canvas& canvas) override final;
	virtual void Start() override final;
	virtual bool HasFinished() override final { return !ch::IsTimeSet(starttime); }
};
