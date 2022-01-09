#include "ParticleOverlayRenderer.h"

ParticleOverlayRenderer::ParticleOverlayRenderer()
{
}

void ParticleOverlayRenderer::Render(Canvas& canvas)
{
	for(auto pe : particles)
		pe->Render(canvas);
}

void ParticleOverlayRenderer::RegisterParticleEffect(ParticleEffect* pe)
{
	particles.push_back(pe);
}

void ParticleOverlayRenderer::UnregisterParticleEffect(ParticleEffect* pe)
{
	auto pos = std::find(particles.begin(), particles.end(), pe);
	if(pos != particles.end())
		particles.erase(pos);
}
