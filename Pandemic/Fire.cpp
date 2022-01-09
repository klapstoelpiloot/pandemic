#include "Fire.h"
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"
#include "glm/geometric.hpp"
#include "glm/trigonometric.hpp"
#include "GraphicsConstants.h"
#include <math.h>

glm::vec2 hash(glm::vec2 p)
{
	p = glm::vec2(glm::dot(p, glm::vec2(127.1f, 311.7f)),
		glm::dot(p, glm::vec2(269.5f, 183.3f)));
	return -1.0f + 2.0f * glm::fract(glm::sin(p) * 43758.5453123f);
}

float noise(glm::vec2 p)
{
	const float K1 = 0.366025404f;
	const float K2 = 0.211324865f;
	glm::vec2 i = glm::floor(p + (p.x + p.y) * K1);
	glm::vec2 a = p - i + (i.x + i.y) * K2;
	glm::vec2 o = glm::step(glm::vec2(a.y, a.x), a);
	glm::vec2 b = a - o + K2;
	glm::vec2 c = a - 1.0f + 2.0f * K2;
	glm::vec3 h = glm::max(0.5f - glm::vec3(glm::dot(a, a), glm::dot(b, b), glm::dot(c, c)), 0.0f);
	glm::vec3 n = h * h * h * h * glm::vec3(glm::dot(a, hash(i + 0.0f)), glm::dot(b, hash(i + o)), glm::dot(c, hash(i + 1.0f)));
	return glm::dot(n, glm::vec3(70.0f));
}

float fbm(glm::vec2 st)
{
	constexpr int OCTAVES = 6;
	float value = 0.0f;
	float amplitude = .5f;
	for(int i = 0; i < OCTAVES; i++)
	{
		value += amplitude * noise(st);
		st *= 2.0f;
		amplitude *= 0.5f;
	}
	return value * 0.5f + 0.5f;
}

Color Fire::PixelShader(float u, float v, float timesec)
{
	constexpr glm::vec2 fireMovement = glm::vec2(-0.1f, 0.5f);
	constexpr glm::vec2 distortionMovement = glm::vec2(-0.01f, 0.2f);
	constexpr float distortionStrength = 0.3f;

	float color = 0.0f;
	glm::vec2 uv = glm::vec2(u, v);

	//These variables are used to modify the speed at which the texture moves
	float strength = glm::floor(uv.x + 1.0f);
	float t = glm::max(2.0f, 1.25f * strength) * timesec * 0.75f;

	color += fbm(uv * glm::vec2(1.0f, 0.3f) + distortionMovement * t);
	glm::vec2 displacement = glm::clamp((glm::vec2(color) - .5f) * distortionStrength, -1.0f, 1.0f);
	uv += displacement;

	glm::vec2 uvT = (uv * glm::vec2(1.0f, 0.5f)) + fireMovement * t * 0.55f;
	float col = glm::pow(fbm(8.0f * uvT), 1.0f);

	float gradient = 5.0f;
	float finalNoise = col * gradient;

	glm::vec3 colorFinal = finalNoise * glm::vec3(col, col * col * col, col * col * col * col * col * col);

	return Color(static_cast<byte>(std::clamp(colorFinal.r * 255.0f, 0.0f, 255.0f)),
		static_cast<byte>(std::clamp(colorFinal.g * 255.0f, 0.0f, 255.0f)),
		static_cast<byte>(std::clamp(colorFinal.b * 255.0f, 0.0f, 255.0f)));
}

