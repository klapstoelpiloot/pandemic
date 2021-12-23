#pragma once
#include <math.h>

struct Vec2
{
public:

	// Fields
	float x;
	float y;

	// Constructors
	Vec2() { x = 0.0f; y = 0.0f; }
	Vec2(float _x, float _y) { x = _x; y = _y; }

	// Methods
	inline static Vec2 Add(Vec2& a, Vec2& b) { return Vec2(a.x + b.x, a.y + b.y); }
	inline static Vec2 Sub(Vec2& a, Vec2& b) { return Vec2(a.x - b.x, a.y - b.y); }
	inline static Vec2 Mul(Vec2& a, float b) { return Vec2(a.x * b, a.y * b); }
	inline static float Dot(Vec2& a, Vec2& b) { return (a.x * b.x) + (a.y * b.y); }
	inline static Vec2 Fract(Vec2& v) { float i; return Vec2(modff(v.x, &i), modff(v.y, &i)); }
	inline static Vec2 Floor(Vec2& v) { return Vec2(floorf(v.x), floorf(v.y)); }
	inline static Vec2 Trunc(Vec2& v) { return Vec2(truncf(v.x), truncf(v.y)); }
	inline static Vec2 Min(Vec2& a, Vec2& b) { return Vec2(std::min(a.x, b.x), std::min(a.y, b.y)); }
	inline static Vec2 Min(Vec2& a, float b) { return Vec2(std::min(a.x, b), std::min(a.y, b)); }
	inline static float LengthSqr(Vec2& v) { return (v.x * v.x) + (v.y * v.y); }
	inline static float Length(Vec2& v) { return std::sqrt((v.x * v.x) + (v.y * v.y)); }
	inline static Vec2 Normalize(Vec2& v) { float l = Vec2::Length(v); return Vec2(v.x / l, v.y / l); }
};

