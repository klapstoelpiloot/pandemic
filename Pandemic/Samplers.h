#pragma once
#include "Tools.h"
#include "Color.h"

/*
  These samplers are used for high performance access to the pixel data.
  This is needed, because the Image and Canvas use virtual functions (for IImage) which cannot be inlined and are slow.
  You don't want to call a virtual GetPixel for every pixel. Instead, you call a virtual GetColorSampler once,
  which gives you this object and you call the the () operator for every pixel, which can be inlined.
*/

// A sampler to get pixels from a monochrome data block
struct MonoSampler
{
	private:
		const byte* data;
		int width;

	public:
		MonoSampler(const byte* _data, int _width) : data(_data), width(_width) {}
		inline byte operator() (int x, int y) const { return data[y * width + x]; }
};

// A sampler to get pixels from a color data block
struct ColorSampler
{
private:
	const Color* data;
	int width;

public:
	ColorSampler(const Color* _data, int _width) : data(_data), width(_width) {}
	inline Color operator() (int x, int y) const { return data[y * width + x]; }
};
