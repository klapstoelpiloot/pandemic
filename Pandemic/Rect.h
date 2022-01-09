#pragma once
#include "Point.h"

struct Rect
{
	int x;
	int y;
	int width;
	int height;
	
	Rect() { x = 0; y = 0; width = 0; height = 0; }
	Rect(int _x, int _y, int _width, int _height) { x = _x; y = _y; width = _width; height = _height; }
	Rect(Point lefttop, Size size) { x = lefttop.x; y = lefttop.y; width = size.width; height = size.height; }

	inline int Left() const { return x; }
	inline int Top() const { return y; }
	inline int Right() const { return x + (width - 1); }
	inline int Bottom() const { return y + (height - 1); }
	inline Size GetSize() const { return Size(width, height); }
	inline bool IsEmpty() const { return (height == 0) || (width == 0); }
};
