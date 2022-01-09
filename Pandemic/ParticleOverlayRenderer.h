#pragma once
#include "Tools.h"
#include "IRenderer.h"
#include "ParticleEffect.h"

// Because I want particle effects to continue (dissipate slowly) when an animation has already finished,
// we register the effects here and this renderer will take care of calling Render() on them.
// Note that this renderer does not own the particle effect. It must be unregistered before destruction.
class ParticleOverlayRenderer final : public virtual IRenderer
{
private:

	vector<ParticleEffect*> particles;

public:

	ParticleOverlayRenderer();

	virtual void Render(Canvas& canvas) override;
	void RegisterParticleEffect(ParticleEffect* pe);
	void UnregisterParticleEffect(ParticleEffect* pe);
};
