#pragma once
#include "Graphics.h"
#include "Particle.h"

class ParticleEffect
{
private:

	// Constants
	static constexpr int64 TIMESTEP = 10;
	static constexpr float TIMESTEP_DT = TIMESTEP / 1000.0f;

	// Members
	vector<const Image*> images;
	vector<Particle> particles;
	bool additive;
	int64 lasttime;
	int64 lifetime;
	glm::vec2 gravity;
	float fadealpha;
	float deceleration;
	float swingstrength;

public:

	ParticleEffect();

	// Properties
	void SetAdditive(bool renderadditive) { additive = renderadditive; }
	void SetGravity(glm::vec2 g) { gravity = g * TIMESTEP_DT; }
	void SetLifetime(int64 milliseconds) { lifetime = milliseconds; }
	void SetFadeAlpha(float a) { fadealpha = a * TIMESTEP_DT; }
	void SetDeceleration(float v) { deceleration = v; }
	void SetSwingStrength(float v) { swingstrength = v; }

	// Methods
	void Begin();
	void Render(Canvas& canvas);
	void AddImage(const Image* image);
	void Spawn(Particle p) { particles.push_back(p); }
};
