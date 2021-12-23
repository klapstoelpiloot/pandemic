#pragma once

struct Size
{
public:

	// Fields
	int width;
	int height;

	// Constructors
	Size() { width = 0; height = 0; }
	Size(int _width, int _height) { width = _width; height = _height; }
};

