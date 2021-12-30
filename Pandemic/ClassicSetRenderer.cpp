#include <math.h>
#include "ClassicSetRenderer.h"
#include "Main.h"

#define TEXT_POS_X			64
#define TEXT_START_Y		30
#define TEXT_MOVE_HEIGHT	33
#define SHOW_DURATION		1600
#define INITIAL_SPAWNS		400
#define SPAWNS_PER_FRAME	2
#define MOVE_TIME_DIV		500
#define NUM_SPARK_IMAGES	3

ClassicSetRenderer::ClassicSetRenderer(ParticleEffect& particleeffect, ParticleEffect& sparkseffect) :
	particles(particleeffect),
	sparks(sparkseffect),
	settext(Main::GetResources().BoldBitsLarge(), HorizontalAlign::Center, VerticalAlign::Top),
	texture(Main::GetResources().GetImage("yellow22d.dds")),
	visible(false)
{
}

void ClassicSetRenderer::Render(Canvas& canvas)
{
	TimePoint t = Clock::now();

	// Update visibility
	visible = (t < (starttime + ch::milliseconds(SHOW_DURATION)));
	if(!visible)
		return;

	float offset = std::sin(static_cast<float>(ch::ToMilliseconds(t - starttime)) / MOVE_TIME_DIV) * TEXT_MOVE_HEIGHT;
	int offsetint = static_cast<int>(std::round(offset));

	// Spawn some particles
	for(int i = 0; i < SPAWNS_PER_FRAME; i++)
	{
		SpawnParticle(WHITE, offsetint);
		SpawnParticle(YELLOW, offsetint);
	}

	// Draw set text
	Point pos(TEXT_POS_X, TEXT_START_Y - offsetint);
	settext.DrawOutlineMask(canvas, pos, 2, BLACK);
	particles.Render(canvas);
	settext.DrawTexturedMask(canvas, pos, texture);
	sparks.Render(canvas);
}

void ClassicSetRenderer::ShowSet(int index)
{
	settext.SetText("SET  " + String::From(index));
	starttime = Clock::now();
	visible = true;

	// Spawn some particles
	particles.Begin();
	sparks.Begin();
	for(int i = 0; i < INITIAL_SPAWNS; i++)
	{
		SpawnParticle(WHITE, 0);
		SpawnParticle(YELLOW, 0);
	}
}

void ClassicSetRenderer::SpawnParticle(Color color, int yoffset)
{
	const Size& size = settext.GetTextSize();

	// Spawn position
	int x = TEXT_POS_X + (Random(0, size.width) - (size.width / 2));
	int y = TEXT_START_Y + (Random(0, size.height) - (size.height / 2)) - yoffset;
	glm::vec2 p(static_cast<float>(x), static_cast<float>(y));

	// Random start velocity
	glm::vec2 v;
	if(yoffset == 0)
		v = glm::vec2(Random(-3.0f, 3.0f), Random(0.0f, -2.0f));
	else
		v = glm::vec2(Random(-0.5f, 0.5f), Random(1.0f, -0.5f));

	// Random alpha
	float a = Random(1.0f, 2.5f);

	// Meta is used for Swing effect
	// X = x position to swing about
	// Y = amount of swing factor
	glm::vec2 meta(64.0f, Random(0.1f, 1.0f));

	// Pixel or spark?
	if(Random(0, 10) == 0)
	{
		// Make a spark image particle
		int imgindex = Random(0, NUM_SPARK_IMAGES - 1);
		sparks.Spawn(Particle(imgindex, p, v, color, a, meta));
	}
	else
	{
		// Make a pixel particle
		particles.Spawn(Particle(p, v, color, a, meta));
	}
}
