#pragma once
#include <math.h>

struct Vec3
{
public:

	// Fields
	float x;
	float y;
	float z;

	// Constructors
	Vec3() { x = 0.0f; y = 0.0f; z= 0.0f; }
	Vec3(float _x, float _y, float _z) { x = _x; y = _y; z = _z; }

	// Methods
	inline static Vec3 Add(Vec3& a, Vec3& b) { return Vec3(a.x + b.x, a.y + b.y, a.z + b.z); }
	inline static Vec3 Sub(Vec3& a, Vec3& b) { return Vec3(a.x - b.x, a.y - b.y, a.z - b.z); }
	inline static Vec3 Mul(Vec3& a, float b) { return Vec3(a.x * b, a.y * b, a.z * b); }
	inline static float Dot(Vec3& a, Vec3& b) { return (a.x * b.x) + (a.y * b.y) + (a.z * b.z); }
	inline static Vec3 Fract(Vec3& v) { float i; return Vec3(modff(v.x, &i), modff(v.y, &i), modff(v.z, &i)); }
	inline static Vec3 Floor(Vec3& v) { return Vec3(floorf(v.x), floorf(v.y), floorf(v.z)); }
	inline static Vec3 Trunc(Vec3& v) { return Vec3(truncf(v.x), truncf(v.y), truncf(v.z)); }
	inline static Vec3 Min(Vec3& a, Vec3& b) { return Vec3(std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z)); }
	inline static Vec3 Min(Vec3& a, float b) { return Vec3(std::min(a.x, b), std::min(a.y, b), std::min(a.z, b)); }
	inline static float LengthSqr(Vec3& v) { return (v.x * v.x) + (v.y * v.y) + (v.z * v.z); }
	inline static float Length(Vec3& v) { return std::sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z)); }
	inline static Vec3 Normalize(Vec3& v) { float l = Vec3::Length(v); return Vec3(v.x / l, v.y / l, v.z / l); }
};

