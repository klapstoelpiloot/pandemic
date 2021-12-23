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
	inline int Bottom() const { return y - (height - 1); }
	inline Point LeftTop() const { return Point(x, y); }
	inline Point RightBottom() const { return Point(x + (width - 1), y - (height - 1)); }
	inline Point RightTop() const { return Point(x + (width - 1), y); }
	inline Point LeftBottom() const { return Point(x, y - (height - 1)); }
	inline Size GetSize() const { return Size(width, height); }
	inline bool IsEmpty() const { return (height == 0) || (width == 0); }

	static inline Rect FromLTRB(int left, int top, int right, int bottom) { return Rect(left, top, (right - left) + 1, (top - bottom) + 1); }

	inline bool Contains(int _x, int _y) const { return (_x >= x) && (_x < x + width) && (_y <= y) && (_y > y - height); }
	inline bool Contains(Point p) const { return (p.x >= x) && (p.x < x + width) && (p.y <= y) && (p.y > y - height); }
};
