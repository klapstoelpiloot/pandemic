#pragma once
#include "glm/vec2.hpp"
#include "Color.h"

struct Particle
{
	// Members
	int64 born;
	int image;
	glm::vec2 pos;
	glm::vec2 vel;
	Color color;
	float alpha;
	glm::vec2 meta;

	// Constructor
	Particle(glm::vec2 position, glm::vec2 velocity, Color color, glm::vec2 meta) :
		born(ch::ToMilliseconds(Clock::now())),
		image(0),
		pos(position),
		vel(velocity),
		color(color),
		alpha(Color::ToFloat(color.a)),
		meta(meta)
	{
	}

	// Constructor
	Particle(glm::vec2 position, glm::vec2 velocity, Color color, float alpha, glm::vec2 meta) :
		born(ch::ToMilliseconds(Clock::now())),
		image(0),
		pos(position),
		vel(velocity),
		color(color),
		alpha(alpha),
		meta(meta)
	{
	}

	// Constructor
	Particle(int imageindex, glm::vec2 position, glm::vec2 velocity, Color color, glm::vec2 meta) :
		born(ch::ToMilliseconds(Clock::now())),
		image(imageindex),
		pos(position),
		vel(velocity),
		color(color),
		alpha(Color::ToFloat(color.a)),
		meta(meta)
	{
	}

	// Constructor
	Particle(int imageindex, glm::vec2 position, glm::vec2 velocity, Color color, float alpha, glm::vec2 meta) :
		born(ch::ToMilliseconds(Clock::now())),
		image(imageindex),
		pos(position),
		vel(velocity),
		color(color),
		alpha(alpha),
		meta(meta)
	{
	}
};
