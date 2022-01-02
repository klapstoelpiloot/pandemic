#pragma once
#include "Size.h"

struct Point
{
public:

	// Fields
	int x;
	int y;

	// Constructors
	Point() { x = 0; y = 0; }
	Point(int _x, int _y) { x = _x; y = _y; }
	Point(const std::array<int, 2U>& p) { x = p[0]; y = p[1]; }

	// Methods
	inline Point Offset(int movex, int movey) const
	{
		Point p = *this;
		p.x += movex;
		p.y += movey;
		return p;
	}

	inline Point Offset(Size size) const
	{
		Point p = *this;
		p.x += size.width;
		p.y += size.height;
		return p;
	}

	inline Point Clip(int width, int height) const
	{
		Point p = *this;

		if(p.x < 0)
			p.x = 0;
		else if(p.x >= width)
			p.x = width - 1;

		if(p.y < 0)
			p.y = 0;
		else if(p.y >= height)
			p.y = height - 1;

		return p;
	}
};

