#pragma once
#include <algorithm>
#include "Tools.h"

#define BLACK Color(0, 0, 0)
#define WHITE Color(255, 255, 255)
#define RED Color(255, 0, 0)
#define GREEN Color(0, 255, 0)
#define BLUE Color(0, 0, 255)
#define YELLOW Color(255, 255, 0)

// This divides by 255 and is about 10% faster on the Raspberry Pi than what the compiler produces for / 255
// See: https://www.reddit.com/r/C_Programming/comments/gudfyk/faster_divide_by_255/
#define DIV_255_FAST(x)    (((x) + (((x) + 257) >> 8)) >> 8)

// Modulates two byte colors (for example, a red value and an alpha value)
#define MOD_BYTE_COLOR(a, b) static_cast<byte>(DIV_255_FAST(static_cast<uint>(a) * static_cast<uint>(b)))

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
	Color(Color _c, byte _a) { r = _c.r; g = _c.g; b = _c.b; a = MOD_BYTE_COLOR(_c.a, _a); }

	// TODO: I could probably squeeze some more performance out of the operations below with SIMD instructions

	// Blends the given color with this color by the amount of alpha in the given color
	inline void Blend(Color c)
	{
		uint cra = static_cast<uint>(c.r) * static_cast<uint>(c.a);
		uint cga = static_cast<uint>(c.g) * static_cast<uint>(c.a);
		uint cba = static_cast<uint>(c.b) * static_cast<uint>(c.a);
		uint ria = static_cast<uint>(r) * (255u - static_cast<uint>(c.a));
		uint gia = static_cast<uint>(g) * (255u - static_cast<uint>(c.a));
		uint bia = static_cast<uint>(b) * (255u - static_cast<uint>(c.a));
		r = static_cast<byte>(DIV_255_FAST(cra) + DIV_255_FAST(ria));
		g = static_cast<byte>(DIV_255_FAST(cga) + DIV_255_FAST(gia));
		b = static_cast<byte>(DIV_255_FAST(cba) + DIV_255_FAST(bia));
	}

	// Adds the given color to this color by the amount of alpha in the given color
	inline void Add(Color c)
	{
		uint cra = static_cast<uint>(c.r) * static_cast<uint>(c.a);
		uint cga = static_cast<uint>(c.g) * static_cast<uint>(c.a);
		uint cba = static_cast<uint>(c.b) * static_cast<uint>(c.a);
		r = static_cast<byte>(std::clamp(static_cast<uint>(r) + DIV_255_FAST(cra), 0u, 255u));
		g = static_cast<byte>(std::clamp(static_cast<uint>(g) + DIV_255_FAST(cga), 0u, 255u));
		b = static_cast<byte>(std::clamp(static_cast<uint>(b) + DIV_255_FAST(cba), 0u, 255u));
	}

	// Applies the given color to this color by using the alpha in the given color as a mask.
	// This is intended to be used with alpha at either 255 or 0. Use Blend for arbitrary alpha values.
	inline void Mask(Color c)
	{
		r = (c.r & c.a) | (r & static_cast<byte>(~c.a));
		g = (c.g & c.a) | (g & static_cast<byte>(~c.a));
		b = (c.b & c.a) | (b & static_cast<byte>(~c.a));
		a |= c.a;
	}

	// Modulates the brightness of this color by the amount specified (0-255)
	inline void ModulateRGB(byte m)
	{
		r = MOD_BYTE_COLOR(r, m);
		g = MOD_BYTE_COLOR(g, m);
		b = MOD_BYTE_COLOR(b, m);
	}

	// Modulates the alpha of this color by the amount specified (0-255)
	inline void ModulateA(byte m)
	{
		a = MOD_BYTE_COLOR(a, m);
	}

	// Modulates the color and alpha of this color by the amount specified (0-255)
	inline void ModulateRGBA(byte m)
	{
		r = MOD_BYTE_COLOR(r, m);
		g = MOD_BYTE_COLOR(g, m);
		b = MOD_BYTE_COLOR(b, m);
		a = MOD_BYTE_COLOR(a, m);
	}

	// Modulates the color and alpha of this color by the specified values in c
	inline void ModulateRGBA(Color c)
	{
		r = MOD_BYTE_COLOR(r, c.r);
		g = MOD_BYTE_COLOR(g, c.g);
		b = MOD_BYTE_COLOR(b, c.b);
		a = MOD_BYTE_COLOR(a, c.a);
	}

	inline static float ToFloat(byte b) { return static_cast<float>(b) * 0.00392156862745f; }
	inline static byte ToByte(float f) { return static_cast<byte>(std::clamp(static_cast<int>(f * 255.0f), 0, 255)); }
	inline static byte Modulate(byte a, byte b) { return static_cast<byte>(DIV_255_FAST(static_cast<uint>(a) * static_cast<uint>(b))); }
};
