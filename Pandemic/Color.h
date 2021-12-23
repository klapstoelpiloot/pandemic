#pragma once
#include <algorithm>
#include "Tools.h"

#define BLACK Color(0, 0, 0)
#define WHITE Color(255, 255, 255)
#define RED Color(255, 0, 0)
#define GREEN Color(0, 255, 0)
#define BLUE Color(0, 0, 255)
#define YELLOW Color(255, 255, 0)

struct Color
{
public:

	// Fields
	byte r;
	byte g;
	byte b;
	byte a;

	// Constructors
	Color() { r = 0u; g = 0u; b = 0u; a = 0u; }
	Color(byte _r, byte _g, byte _b) { r = _r; g = _g; b = _b; a = 255u; }
	Color(byte _r, byte _g, byte _b, byte _a) { r = _r; g = _g; b = _b; a = _a; }

	// Blends the given color with this color by the amount of alpha in the given color
	inline void Blend(Color c)
	{
		r = static_cast<byte>((static_cast<uint>(c.r) * static_cast<uint>(c.a)) / 255u + (static_cast<uint>(r) * static_cast<uint>(255u - c.a)) / 255u);
		g = static_cast<byte>((static_cast<uint>(c.g) * static_cast<uint>(c.a)) / 255u + (static_cast<uint>(g) * static_cast<uint>(255u - c.a)) / 255u);
		b = static_cast<byte>((static_cast<uint>(c.b) * static_cast<uint>(c.a)) / 255u + (static_cast<uint>(b) * static_cast<uint>(255u - c.a)) / 255u);
	}

	// Adds the given color to this color by the amount of alpha in the given color
	inline void Add(Color c)
	{
		r = static_cast<byte>(std::clamp(static_cast<uint>(r) + (static_cast<uint>(c.r) * static_cast<uint>(c.a)) / 255u, 0u, 255u));
		g = static_cast<byte>(std::clamp(static_cast<uint>(g) + (static_cast<uint>(c.g) * static_cast<uint>(c.a)) / 255u, 0u, 255u));
		b = static_cast<byte>(std::clamp(static_cast<uint>(b) + (static_cast<uint>(c.b) * static_cast<uint>(c.a)) / 255u, 0u, 255u));
	}

	// Applies the given color to this color by using the alpha in the given color as a mask.
	// This is intended to be used with alpha at either 255 or 0. Use Blend for arbitrary alpha values.
	inline void Mask(Color c)
	{
		r = (c.r & c.a) | (r & static_cast<byte>(~c.a));
		g = (c.g & c.a) | (g & static_cast<byte>(~c.a));
		b = (c.b & c.a) | (b & static_cast<byte>(~c.a));
	}

	// Modulates the brightness of this color by the amount specified (0-255)
	inline void Modulate(byte m)
	{
		r = static_cast<byte>((static_cast<uint>(r) * static_cast<uint>(m)) / 255u);
		g = static_cast<byte>((static_cast<uint>(g) * static_cast<uint>(m)) / 255u);
		b = static_cast<byte>((static_cast<uint>(b) * static_cast<uint>(m)) / 255u);
	}

	inline static float ToFloat(byte b) { return static_cast<float>(b) * 0.00392156862745f; }
	inline static byte ToByte(float f) { return static_cast<byte>(std::clamp(static_cast<int>(f * 255.0f), 0, 255)); }
};
