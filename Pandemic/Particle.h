#pragma once
#include "Vec2.h"
#include "Color.h"

struct Particle
{
	// Members
	int64 born;
	int image;
	Vec2 pos;
	Vec2 vel;
	Color color;
	float alpha;
	Vec2 meta;

	// Constructor
	Particle(Vec2 position, Vec2 velocity, Color color, Vec2 meta) :
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
	Particle(Vec2 position, Vec2 velocity, Color color, float alpha, Vec2 meta) :
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
	Particle(int imageindex, Vec2 position, Vec2 velocity, Color color, Vec2 meta) :
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
	Particle(int imageindex, Vec2 position, Vec2 velocity, Color color, float alpha, Vec2 meta) :
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
