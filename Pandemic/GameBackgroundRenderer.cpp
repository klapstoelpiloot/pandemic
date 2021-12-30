#include "GameBackgroundRenderer.h"
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"
#include "glm/geometric.hpp"
#include <math.h>

vector<Color> COLORS { Color(120, 0, 0), Color(110, 50, 0), Color(0, 100, 0), Color(0, 110, 110), Color(0, 30, 160), Color(110, 50, 100) };

GameBackgroundRenderer::GameBackgroundRenderer() :
	starttime(Clock::now()),
	timesec(0.0f),
	temporalditheroffset(0),
	colorindex(0)
{
	// Because we don't draw all the pixels every frame, we need to
	// draw on a separate buffer which we copy to the canvas every frame.
	backbuffer.Clear(BLACK);
}

void GameBackgroundRenderer::Begin()
{
	starttime = Clock::now();
	colorindex = 0;
}

void GameBackgroundRenderer::SetNextColor()
{
	colorindex++;
	if(colorindex == static_cast<int>(COLORS.size()))
		colorindex = 0;
}

void GameBackgroundRenderer::SetPrevColor()
{
	colorindex--;
	if(colorindex < 0)
		colorindex = static_cast<int>(COLORS.size()) - 1;
}

void GameBackgroundRenderer::Render(Canvas& canvas)
{
	timesec = static_cast<float>(ch::ToMilliseconds(Clock::now() - starttime)) / 1000.0f;
	temporalditheroffset++;

	for(int y = 0; y < DISPLAY_HEIGHT; y++)
	{
		for(int x = 0; x < DISPLAY_WIDTH; x++)
		{
			// Don't redraw every pixel every frame, that is too expensive.
			// I'm also happy with the grainy effect that this dither gives!
			if(((x ^ y ^ temporalditheroffset) & 0x03) == 0)
			{
				float a = PixelShader(static_cast<float>(x), static_cast<float>(DISPLAY_HEIGHT - y));
				Color c = COLORS[colorindex];
				c.Modulate(static_cast<byte>(std::clamp(a * 255.0f, 0.0f, 255.0f)));
				backbuffer.SetPixel(x, y, c);
			}
		}
	}

	// Copy image to canvas
	backbuffer.CopyTo(canvas);
}

glm::vec3 Hash33(glm::vec3 p)
{
	glm::vec3 a = glm::vec3(7.0, 157.0, 113.0);
	glm::vec3 b = glm::vec3(2097152.0, 262144.0, 32768.0);
	float n = sin(glm::dot(p, a));
	glm::vec3 c = b * n;
	return glm::fract(c);
}

float Voronoi(glm::vec3 p)
{
	glm::vec3 g = glm::floor(p);
	p = glm::fract(p);

	float d = 1.0f;

	for(int j = -1; j <= 1; j++)
	{
		for(int i = -1; i <= 1; i++)
		{
			glm::vec3 gb, h, ph, r;
			glm::vec3 b = glm::vec3(static_cast<float>(i), static_cast<float>(j), -1.0f);

			r = b - p + Hash33(g + b);
			d = glm::min(d, glm::dot(r, r));

			b.z = 0.0f;
			r = b - p + Hash33(g + b);
			d = glm::min(d, glm::dot(r, r));

			b.z = 1.0f;
			r = b - p + Hash33(g + b);
			d = glm::min(d, glm::dot(r, r));
		}
	}

	return d; // Range: [0, 1]
}

// Standard fBm function with some time dialation to give a parallax 
// kind of effect. In other words, the position and time frequencies 
// are changed at different rates from layer to layer.
float NoiseLayers(glm::vec3 p, float timesec)
{
	constexpr int ITERATIONS = 2;
	constexpr float SPEED = 1.0f;
	constexpr float CIRCULAR_STRETCH = 0.5f;
	constexpr float LAYER_POS_MUL = 4.0f;
	constexpr float LAYER_TIME_MUL = 0.5f;
	constexpr float LAYER_INTENSITY_MUL = 0.5f;

	glm::vec3 t = glm::vec3(0.0f, 0.0f, (p.z * CIRCULAR_STRETCH) + (timesec * SPEED));
	float tot = 0.0f, sum = 0.0f, amp = 1.0f;
	for(int i = 0; i < ITERATIONS; i++)
	{
		tot += Voronoi(p + t) * amp;
		p *= LAYER_POS_MUL;
		t *= LAYER_TIME_MUL;
		sum += amp;
		amp *= LAYER_INTENSITY_MUL;
	}

	return tot / sum; // Range: [0, 1]
}

float GameBackgroundRenderer::PixelShader(float x, float y)
{
	constexpr float SHIFT_SPEED = 0.2f;
	constexpr float ROTATE_SPEED = 0.13f;
	constexpr float SHIFT_DISTANCE_X = 0.6f;
	constexpr float SHIFT_DISTANCE_Y = 0.3f;
	constexpr float ROTATE_DISTANCE = 0.5f;
	constexpr float TUNNEL_DEPTH = 4.0f;
	constexpr float ZOOM = 6.0f;

	// Coordinates with (0, 0) in the center
	float u = (x - (DISPLAY_WIDTH / 2)) / DISPLAY_HEIGHT;
	float v = (y - (DISPLAY_HEIGHT / 2)) / DISPLAY_HEIGHT;

	// Shifting the central position around.
	u += sin(timesec * SHIFT_SPEED) * SHIFT_DISTANCE_X;
	v += cos(timesec * SHIFT_SPEED) * SHIFT_DISTANCE_Y;

	// Constructing the unit ray.
	glm::vec3 r = glm::vec3(u, v, 3.1415926535898f / TUNNEL_DEPTH);
	glm::vec3 rd = glm::normalize(r);

	// Rotating the ray about the XY plane, to simulate a rolling camera.
	float cs = cos(timesec * ROTATE_SPEED) * ROTATE_DISTANCE;
	float si = sin(timesec * ROTATE_SPEED) * ROTATE_DISTANCE;
	rd.x = rd.x * cs + rd.y * -si;
	rd.y = rd.x * si + rd.y * cs;

	// Passing a unit ray multiple into the Voronoi layer function, which 
	// is nothing more than an fBm setup with some time dialation.
	glm::vec3 rd2 = rd * ZOOM;
	float c = NoiseLayers(rd2, timesec);

	// Monochrome output
	return c * c;
}
