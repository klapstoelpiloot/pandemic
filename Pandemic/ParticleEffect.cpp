#include <math.h>
#include "ParticleEffect.h"

#define MAX_TIME_STEPS	3

ParticleEffect::ParticleEffect() :
	additive(false),
	lasttime(0),
	lifetime(2000),
	gravity(0.0f, 0.0f),
	fadealpha(0.0f),
	deceleration(0.0f),
	swingstrength(0.0f)
{
	particles.reserve(2000);
}

void ParticleEffect::Begin()
{
	particles.clear();
	lasttime = ch::ToMilliseconds(Clock::now());
}

void ParticleEffect::Render(Canvas& canvas)
{
	int64 t = ch::ToMilliseconds(Clock::now());

	// Remove particles which expired
	for(int i = static_cast<int>(particles.size()) - 1; i >= 0; i--)
	{
		if(t > (particles[i].born + lifetime))
		{
			if(i < (static_cast<int>(particles.size()) - 1))
				particles[i] = particles.back();
			particles.pop_back();
		}
	}

	// Advance color/position of particles
	int stepcount = 0;
	while(((t - lasttime) >= TIMESTEP) && (stepcount < MAX_TIME_STEPS))
	{
		for(Particle& p : particles)
		{
			// Do the math for the next position/color
			p.pos += p.vel;
			p.vel *= deceleration;
			p.vel += gravity;

			// Swing logic. This looks complicated, but is just a fast way of getting the sign of swingdelta into swingdeltasign as 1 or -1.
			float swingdelta = p.meta.x - p.pos.x;
			float swingdeltasign = 1;
			*reinterpret_cast<uint32_t*>(&swingdeltasign) |= (*reinterpret_cast<uint32_t*>(&swingdelta) & 0x80000000);
			p.vel.x += swingdeltasign * swingstrength * p.meta.y;

			p.alpha += fadealpha;
		}

		// Advance
		stepcount++;
		lasttime += TIMESTEP;
	}

	// If there is too much time to catch up on, then just skip some time
	if((t - lasttime) >= TIMESTEP)
		lasttime = t;

	if(additive)
	{
		if(images.size() > 0)
		{
			// Draw particles as images (additive)
			for(Particle& p : particles)
			{
				p.color.a = Color::ToByte(p.alpha);
				if(p.color.a > 0)
				{
					int px = static_cast<int>(std::round(p.pos.x));
					int py = static_cast<int>(std::round(p.pos.y));
					const Image& tex = *(images[p.image]);
					canvas.DrawMonoImageAdd(Point(px - (tex.Width() >> 1), py - (tex.Height() >> 1)), tex, p.color);
				}
			}
		}
		else
		{
			// Draw particles as pixels (additive)
			for(Particle& p : particles)
			{
				p.color.a = Color::ToByte(p.alpha);
				int px = static_cast<int>(std::round(p.pos.x));
				int py = static_cast<int>(std::round(p.pos.y));
				if((px >= 0) && (px < DISPLAY_WIDTH) && (py >= 0) && (py < DISPLAY_HEIGHT))
					canvas.AddPixel(px, py, p.color);
			}
		}
	}
	else
	{
		if(images.size() > 0)
		{
			// Draw particles as images (additive)
			for(Particle& p : particles)
			{
				p.color.a = Color::ToByte(p.alpha);
				if(p.color.a > 0)
				{
					int px = static_cast<int>(std::round(p.pos.x));
					int py = static_cast<int>(std::round(p.pos.y));
					const Image& tex = *(images[p.image]);
					canvas.DrawMonoImageBlend(Point(px - (tex.Width() >> 1), py - (tex.Height() >> 1)), tex, p.color);
				}
			}
		}
		else
		{
			// Draw particles as pixels (blend)
			for(Particle& p : particles)
			{
				p.color.a = Color::ToByte(p.alpha);
				int px = static_cast<int>(std::round(p.pos.x));
				int py = static_cast<int>(std::round(p.pos.y));
				if((px >= 0) && (px < DISPLAY_WIDTH) && (py >= 0) && (py < DISPLAY_HEIGHT))
					canvas.BlendPixel(px, py, p.color);
			}
		}
	}
}

void ParticleEffect::AddImage(const Image* image)
{
	REQUIRE(image->HasColors() == false);
	images.push_back(image);
}
