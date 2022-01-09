#include <math.h>
#include "SetAnimation.h"
#include "Main.h"

#define SET_SOUND_FILES			8
#define NUM_PARTICLES			50
#define SCROLL_DURATION			1000
#define WAIT_DURATION			1000
#define CENTER_Y				32
#define SETTEXT_X				75
#define SET_X					80
#define NUM_SPARK_IMAGES		3

SetAnimation::SetAnimation(ParticleOverlayRenderer& particlesoverlay) :
	particlesoverlay(particlesoverlay),
	settext("SET", Main::GetResources().BoldBitsLarge(), HorizontalAlign::Right, VerticalAlign::Bottom, 1),
	nextset(Main::GetResources().BoldBitsLarge(), HorizontalAlign::Left, VerticalAlign::Bottom, 1),
	prevset(Main::GetResources().BoldBitsLarge(), HorizontalAlign::Left, VerticalAlign::Bottom, 1),
	texture(Main::GetResources().GetImage("yellow22d.dds")),
	index(0)
{
	particles.SetAdditive(true);
	particles.SetDeceleration(0.95f);
	particles.SetFadeAlpha(-2.0f);
	particles.SetGravity(glm::vec2(0.0f, -1.8f));
	particles.SetSwingStrength(0.05f);
	particles.SetLifetime(3000);
	particles.Begin();

	sparks.SetAdditive(true);
	sparks.SetDeceleration(0.95f);
	sparks.SetFadeAlpha(-2.0f);
	sparks.SetGravity(glm::vec2(0.0f, -1.8f));
	sparks.SetSwingStrength(0.05f);
	sparks.SetLifetime(3000);
	for(int i = 0; i < NUM_SPARK_IMAGES; i++)
		sparks.AddImage(&(Main::GetResources().GetImage("spark" + String::From(i + 1) + ".dds")));
	sparks.Begin();

	particlesoverlay.RegisterParticleEffect(&particles);
	particlesoverlay.RegisterParticleEffect(&sparks);
}

SetAnimation::~SetAnimation()
{
	particlesoverlay.UnregisterParticleEffect(&particles);
}

void SetAnimation::Start()
{
	if(index <= SET_SOUND_FILES)
	{
		String soundfile = "set" + String::From(index) + ".wav";
		Main::GetResources().GetSound(soundfile).Play();
	}

	nextset.SetText(String::From(index));
	if(index > 1)
		prevset.SetText(String::From(index - 1));
	else
		prevset.SetText("");

	starttime = Clock::now();
	laststeptime = Clock::now();
	bangtime = Clock::now() + ch::milliseconds(SCROLL_DURATION);
	numberoffset = tweeny::from(0).to(CENTER_Y).during(SCROLL_DURATION).via(easing::cubicIn).wait(WAIT_DURATION);
	if(index == 1)
	{
		numberoffset.step(SCROLL_DURATION / 3);
		bangtime = Clock::now() + ch::milliseconds(SCROLL_DURATION - SCROLL_DURATION / 3);
	}
	setshinetime = bangtime + ch::milliseconds(100);
}

void SetAnimation::Render(Canvas& canvas)
{
	TimePoint t = Clock::now();
	if(!ch::IsTimeSet(starttime) || (t > (starttime + ch::milliseconds(numberoffset.duration()))))
	{
		// Animation ended
		starttime = TimePoint();
		return;
	}

	// Advance time
	int dt = static_cast<int>(ch::ToMilliseconds(t - laststeptime));
	laststeptime = t;
	int textoffset = numberoffset.step(dt);

	if(ch::IsTimeSet(bangtime) && (t > bangtime))
	{
		bangtime = TimePoint();
		textshine.Begin(settext.GetTextSize());

		// Play timed sound
		Main::GetResources().GetSound("set.wav").Play();

		// Spawn some particles
		for(int i = 0; i < NUM_PARTICLES; i++)
		{
			SpawnParticle(YELLOW, Random(SET_X, SET_X + nextset.GetTextSize().width), Random(CENTER_Y - nextset.GetTextSize().height, CENTER_Y));
			SpawnParticle(WHITE, Random(SET_X, SET_X + nextset.GetTextSize().width), Random(CENTER_Y - nextset.GetTextSize().height, CENTER_Y));
		}
	}

	if(ch::IsTimeSet(setshinetime) && (t > setshinetime))
	{
		setshinetime = TimePoint();
		setshine.Begin(nextset.GetTextSize());
	}

	settext.DrawOutlineMask(canvas, Point(SETTEXT_X, CENTER_Y), 2, BLACK);
	nextset.DrawOutlineMask(canvas, Point(SET_X, textoffset), 2, BLACK);
	prevset.DrawOutlineMask(canvas, Point(SET_X, CENTER_Y + textoffset), 2, BLACK);

	settext.DrawTexturedMask(canvas, Point(SETTEXT_X, CENTER_Y), texture);
	nextset.DrawTexturedMask(canvas, Point(SET_X, textoffset), texture);
	prevset.DrawTexturedMask(canvas, Point(SET_X, CENTER_Y + textoffset), texture);

	textshine.Draw(canvas, settext, 2, Point(SETTEXT_X, CENTER_Y));
	setshine.Draw(canvas, nextset, 2, Point(SET_X, textoffset));
}

void SetAnimation::SpawnParticle(Color color, int x, int y)
{
	// Spawn position
	glm::vec2 p(static_cast<float>(x), static_cast<float>(y));

	// Random start velocity
	glm::vec2 v = glm::vec2(Random(-2.0f, 2.0f), Random(2.0f, -1.0f));

	// Random alpha
	float a = Random(1.0f, 2.5f);

	// Meta is used for Swing effect
	// X = x position to swing about
	// Y = amount of swing factor
	glm::vec2 meta(SET_X + 5, Random(0.1f, 1.0f));

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
