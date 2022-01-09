#include "ClassicHUDRenderer.h"
#include "Main.h"

// Positions/colors/settings
// Gate X axis positions (start/end) are inclusive, but do not include outline
#define GATE1_START			8
#define GATE1_END			26
#define GATE2_START			39
#define GATE2_END			57
#define GATE3_START			70
#define GATE3_END			88
#define GATE4_START			101
#define GATE4_END			119
#define GATE_LINE_HEIGHT	2
#define GATE_NUMBER_Y		33 - GATE_LINE_HEIGHT
#define GATE_OUTLINE		BLACK
#define GATE_NORMAL_COLOR	WHITE
#define GATE_FLASH_COLOR1	Color(255, 220, 50)
#define GATE_FLASH_COLOR2	Color(150, 100, 0)
#define GATE_FLASH_INTERVAL	400
#define EFFECT_SMALL_DIV	12
#define LABELS_Y			-1
#define COUNTERS_Y			14
#define ROUND_X				2
#define PUCKS_X				64
#define SCORE_X				127
#define LABELS_COLOR		WHITE
#define LABELS_OUTLINE		BLACK
#define COUNTERS_COLOR		WHITE
#define COUNTERS_OUTLINE	BLACK
#define NUM_SPARK_IMAGES	3
#define FADE_OUT_DURATION	ch::milliseconds(200)
#define FLOAT_DURATION		2000
#define SCORE_SET_DELAY		1000

String GATE_NUMBERS[] = { "2", "3", "4", "1" };

ClassicHUDRenderer::ClassicHUDRenderer(ParticleOverlayRenderer& particlesoverlay) :
	round(Main::GetResources().GetFont("pixel7.fnt"), HorizontalAlign::Left, VerticalAlign::Middle),
	score(Main::GetResources().GetFont("pixel7.fnt"), HorizontalAlign::Right, VerticalAlign::Middle),
	pucks(Main::GetResources().GetFont("pixel7.fnt"), HorizontalAlign::Center, VerticalAlign::Middle),
	roundlabel(Main::GetResources().Smallest(), HorizontalAlign::Left, VerticalAlign::Top),
	scorelabel(Main::GetResources().Smallest(), HorizontalAlign::Right, VerticalAlign::Top),
	puckslabel(Main::GetResources().Smallest(), HorizontalAlign::Center, VerticalAlign::Top),
	pointtext {
		Text(Main::GetResources().Smallest(), HorizontalAlign::Center, VerticalAlign::Bottom),
		Text(Main::GetResources().Smallest(), HorizontalAlign::Center, VerticalAlign::Bottom),
		Text(Main::GetResources().Smallest(), HorizontalAlign::Center, VerticalAlign::Bottom),
		Text(Main::GetResources().Smallest(), HorizontalAlign::Center, VerticalAlign::Bottom),
		Text(Main::GetResources().Smallest(), HorizontalAlign::Center, VerticalAlign::Bottom)
	},
	texture(Main::GetResources().GetImage("yellow12.dds")),
	gaterequired{ {false, false, false, false} }
{
	for(uint i = 0; i < gatenumbers.size(); i++)
	{
		Text& gn = gatenumbers[i];
		gn.SetFont(Main::GetResources().Smallest());
		gn.SetHorizontalAlign(HorizontalAlign::Center);
		gn.SetVerticalAlign(VerticalAlign::Bottom);
		gn.SetText(GATE_NUMBERS[i]);
	}

	round.SetText(String::From(0));
	score.SetText(String::From(0));
	pucks.SetText(String::From(0));
	roundlabel.SetText("ROUND");
	scorelabel.SetText("SCORE");
	puckslabel.SetText("PUCKS");

	gateparticles.SetAdditive(true);
	gateparticles.SetDeceleration(0.95f);
	gateparticles.SetFadeAlpha(-3.0f);
	gateparticles.SetGravity(glm::vec2(0.0f, -1.8f));
	gateparticles.SetSwingStrength(0.05f);
	gateparticles.SetLifetime(3000);
	gateparticles.Begin();
	particlesoverlay.RegisterParticleEffect(&gateparticles);
}

void ClassicHUDRenderer::Render(Canvas& canvas)
{
	TimePoint t = Clock::now();
	int dt = static_cast<int>(ch::ToMilliseconds(t - laststeptime));
	laststeptime = t;
	if(!ch::IsTimeSet(fadeoutstart) || (t < (fadeoutstart + FADE_OUT_DURATION)))
	{
		// Remove floating points that have finished
		for(int i = points.size() - 1; i >= 0; i--)
		{
			if(pointprogress[points[i]].progress() >= 1.0f)
				points.erase(points.begin() + i);
		}

		// Time to show the set score?
		if(ch::IsTimeSet(scoresettime) && (t > scoresettime))
		{
			scoresettime = TimePoint();
			int x = 0;
			switch(Random(0, 2))
			{
				case 0: x = (GATE1_END + GATE2_START) / 2; break;
				case 1: x = (GATE2_END + GATE3_START) / 2; break;
				case 2: x = (GATE3_END + GATE4_START) / 2; break;
			}
			FloatPoints("+10", x, GATE_NUMBER_Y);
		}

		byte alpha;
		if(ch::IsTimeSet(fadeoutstart))
		{
			alpha = static_cast<byte>(255.0f - std::clamp((static_cast<float>(ch::ToMilliseconds(t - fadeoutstart)) / static_cast<float>(FADE_OUT_DURATION.count())) * 255.0f, 0.0f, 255.0f));
		}
		else
		{
			alpha = 255;
		}

		// Determine gate flash color
		bool gateflashbright = (((ToMilliseconds(Clock::now()) / GATE_FLASH_INTERVAL) % 2) == 0);
		Color gateflashcolor = gateflashbright ? GATE_FLASH_COLOR1 : GATE_FLASH_COLOR2;
		Color gate1color = gaterequired[0] ? gateflashcolor : GATE_NORMAL_COLOR;
		Color gate2color = gaterequired[1] ? gateflashcolor : GATE_NORMAL_COLOR;
		Color gate3color = gaterequired[2] ? gateflashcolor : GATE_NORMAL_COLOR;
		Color gate4color = gaterequired[3] ? gateflashcolor : GATE_NORMAL_COLOR;

		// Draw lines above the gates
		DrawGateLine(canvas, GATE1_START, GATE1_END, Color(gate1color, alpha));
		DrawGateLine(canvas, GATE2_START, GATE2_END, Color(gate2color, alpha));
		DrawGateLine(canvas, GATE3_START, GATE3_END, Color(gate3color, alpha));
		DrawGateLine(canvas, GATE4_START, GATE4_END, Color(gate4color, alpha));

		// Draw gate numbers
		gatenumbers[0].DrawOutlineBlend(canvas, Point((GATE1_START + GATE1_END) / 2, GATE_NUMBER_Y), 1, Color(GATE_OUTLINE, alpha));
		gatenumbers[1].DrawOutlineBlend(canvas, Point((GATE2_START + GATE2_END) / 2, GATE_NUMBER_Y), 1, Color(GATE_OUTLINE, alpha));
		gatenumbers[2].DrawOutlineBlend(canvas, Point((GATE3_START + GATE3_END) / 2, GATE_NUMBER_Y), 1, Color(GATE_OUTLINE, alpha));
		gatenumbers[3].DrawOutlineBlend(canvas, Point((GATE4_START + GATE4_END) / 2, GATE_NUMBER_Y), 1, Color(GATE_OUTLINE, alpha));
		gatenumbers[0].DrawBlend(canvas, Point((GATE1_START + GATE1_END) / 2, GATE_NUMBER_Y), Color(gate1color, alpha));
		gatenumbers[1].DrawBlend(canvas, Point((GATE2_START + GATE2_END) / 2, GATE_NUMBER_Y), Color(gate2color, alpha));
		gatenumbers[2].DrawBlend(canvas, Point((GATE3_START + GATE3_END) / 2, GATE_NUMBER_Y), Color(gate3color, alpha));
		gatenumbers[3].DrawBlend(canvas, Point((GATE4_START + GATE4_END) / 2, GATE_NUMBER_Y), Color(gate4color, alpha));

		// Draw counters
		round.DrawOutlineBlend(canvas, Point(ROUND_X, COUNTERS_Y), 1, Color(COUNTERS_OUTLINE, alpha));
		score.DrawOutlineBlend(canvas, Point(SCORE_X, COUNTERS_Y), 1, Color(COUNTERS_OUTLINE, alpha));
		pucks.DrawOutlineBlend(canvas, Point(PUCKS_X, COUNTERS_Y), 1, Color(COUNTERS_OUTLINE, alpha));
		round.DrawTexturedModBlend(canvas, Point(ROUND_X, COUNTERS_Y), texture, Color(255, 255, 255, alpha));
		score.DrawTexturedModBlend(canvas, Point(SCORE_X, COUNTERS_Y), texture, Color(255, 255, 255, alpha));
		pucks.DrawTexturedModBlend(canvas, Point(PUCKS_X, COUNTERS_Y), texture, Color(255, 255, 255, alpha));

		// Draw counter labels
		roundlabel.DrawOutlineBlend(canvas, Point(ROUND_X, LABELS_Y), 1, Color(LABELS_OUTLINE, alpha));
		scorelabel.DrawOutlineBlend(canvas, Point(SCORE_X, LABELS_Y), 1, Color(LABELS_OUTLINE, alpha));
		puckslabel.DrawOutlineBlend(canvas, Point(PUCKS_X, LABELS_Y), 1, Color(LABELS_OUTLINE, alpha));
		roundlabel.DrawBlend(canvas, Point(ROUND_X, LABELS_Y), Color(LABELS_COLOR, alpha));
		scorelabel.DrawBlend(canvas, Point(SCORE_X, LABELS_Y), Color(LABELS_COLOR, alpha));
		puckslabel.DrawBlend(canvas, Point(PUCKS_X, LABELS_Y), Color(LABELS_COLOR, alpha));

		// Advance floating points and draw them
		for(int index : points)
		{
			Point p = pointprogress[index].step(dt);

			// Determine color
			int m = (((ch::ToMilliseconds(t) + p.y * 17) % 100) > 50) ? 1 : 0;
			Color c = Color(255, static_cast<byte>(155 + 100 * m), static_cast<byte>(255 * m), alpha);

			pointtext[index].DrawOutlineBlend(canvas, p, 1, Color(BLACK, alpha));
			pointtext[index].DrawBlend(canvas, p, c);
		}
	}
}

void ClassicHUDRenderer::SetRequiredGates(bool* gates)
{
	for(int g = 0; g < GAME_GATES; g++)
		gaterequired[g] = gates[g];
}

void ClassicHUDRenderer::ScoreRequiredGate(int gate)
{
	switch(gate)
	{
		case 0:
			SpawnGateEffectBig(GATE1_START, GATE1_END);
			FloatPoints(String("+") + GATE_NUMBERS[gate], (GATE1_START + GATE1_END) / 2 - 2, GATE_NUMBER_Y);
			break;

		case 1:
			SpawnGateEffectBig(GATE2_START, GATE2_END);
			FloatPoints(String("+") + GATE_NUMBERS[gate], (GATE2_START + GATE2_END) / 2 - 2, GATE_NUMBER_Y);
			break;

		case 2:
			SpawnGateEffectBig(GATE3_START, GATE3_END);
			FloatPoints(String("+") + GATE_NUMBERS[gate], (GATE3_START + GATE3_END) / 2 - 2, GATE_NUMBER_Y);
			break;

		case 3:
			SpawnGateEffectBig(GATE4_START, GATE4_END);
			FloatPoints(String("+") + GATE_NUMBERS[gate], (GATE4_START + GATE4_END) / 2 - 2, GATE_NUMBER_Y);
			break;
	}
}

void ClassicHUDRenderer::ScoreGate(int gate)
{
	switch(gate)
	{
		case 0:
			SpawnGateEffectSmall(GATE1_START, GATE1_END);
			FloatPoints(String("+") + GATE_NUMBERS[gate], (GATE1_START + GATE1_END) / 2 - 2, GATE_NUMBER_Y);
			break;

		case 1:
			SpawnGateEffectSmall(GATE2_START, GATE2_END);
			FloatPoints(String("+") + GATE_NUMBERS[gate], (GATE2_START + GATE2_END) / 2 - 2, GATE_NUMBER_Y);
			break;

		case 2:
			SpawnGateEffectSmall(GATE3_START, GATE3_END);
			FloatPoints(String("+") + GATE_NUMBERS[gate], (GATE3_START + GATE3_END) / 2 - 2, GATE_NUMBER_Y);
			break;

		case 3:
			SpawnGateEffectSmall(GATE4_START, GATE4_END);
			FloatPoints(String("+") + GATE_NUMBERS[gate], (GATE4_START + GATE4_END) / 2 - 2, GATE_NUMBER_Y);
			break;
	}
}

void ClassicHUDRenderer::ScoreSet()
{
	if(!ch::IsTimeSet(scoresettime))
		scoresettime = Clock::now() + ch::milliseconds(SCORE_SET_DELAY);
}

void ClassicHUDRenderer::DrawGateLine(Canvas& canvas, int startx, int endx, Color color)
{
	// Outline on the left
	canvas.BlendPixel(startx - 1, 29, Color(GATE_OUTLINE, color.a));
	for(int y = 0; y < GATE_LINE_HEIGHT; y++)
		canvas.BlendPixel(startx - 1, 31 - y, Color(GATE_OUTLINE, color.a));

	// Horizontal lines
	for(int x = startx; x <= endx; x++)
	{
		canvas.BlendPixel(x, 29, Color(GATE_OUTLINE, color.a));
		for(int y = 0; y < GATE_LINE_HEIGHT; y++)
			canvas.BlendPixel(x, 31 - y, color);
	}

	// Outline on the right
	canvas.BlendPixel(endx + 1, 29, Color(GATE_OUTLINE, color.a));
	for(int y = 0; y < GATE_LINE_HEIGHT; y++)
		canvas.BlendPixel(endx + 1, 31 - y, Color(GATE_OUTLINE, color.a));
}

void ClassicHUDRenderer::SpawnGateEffectBig(int startx, int endx)
{
	int center = (startx + endx) / 2;

	// Horizontal lines
	for(int x = startx; x <= endx; x++)
	{
		for(int y = 0; y < GATE_LINE_HEIGHT; y++)
			SpawnGateParticle(x, 31 - y, center, GATE_FLASH_COLOR1);
	}

	// Number
	int hw = (gatenumbers[0].GetTextSize().width + 1) / 2;
	int h = gatenumbers[0].GetTextSize().height;
	for(int x = (center - hw); x <= (center + hw); x++)
	{
		for(int y = 0; y < h; y++)
			SpawnGateParticle(x, 31 - y, center, GATE_FLASH_COLOR1);
	}
}

void ClassicHUDRenderer::SpawnGateEffectSmall(int startx, int endx)
{
	int center = (startx + endx) / 2;
	int offset = static_cast<int>(ch::ToMilliseconds(Clock::now()) % EFFECT_SMALL_DIV);

	// Horizontal lines
	for(int x = startx; x <= endx; x++)
	{
		for(int y = 0; y < GATE_LINE_HEIGHT; y++)
		{
			if(((x + y + offset) % EFFECT_SMALL_DIV == 0))
				SpawnGateParticle(x, 31 - y, center, GATE_NORMAL_COLOR);
		}
	}

	// Number
	int hw = (gatenumbers[0].GetTextSize().width + 1) / 2;
	int h = gatenumbers[0].GetTextSize().height;
	for(int x = (center - hw); x <= (center + hw); x++)
	{
		for(int y = 0; y < h; y++)
		{
			if(((x + y + offset) % EFFECT_SMALL_DIV == 0))
				SpawnGateParticle(x, 31 - y, center, GATE_NORMAL_COLOR);
		}
	}
}

void ClassicHUDRenderer::SpawnGateParticle(int x, int y, int centerx, Color color)
{
	glm::vec2 p(static_cast<float>(x), static_cast<float>(y));

	// Random start velocity
	glm::vec2 v(Random(-1.5f, 1.5f), Random(0.2f, -1.0f));

	// Random alpha
	float a = Random(0.5f, 2.5f);

	// Meta is used for Swing effect
	// X = x position to swing about
	// Y = amount of swing factor
	glm::vec2 meta(static_cast<float>(centerx), Random(0.1f, 1.0f));

	// Make the particle
	gateparticles.Spawn(Particle(p, v, color, a, meta));
}

void ClassicHUDRenderer::FloatPoints(String text, int x, int y)
{
	// Find an unused index
	int index = -1;
	for(int i = 0; i < 8; i++)
	{
		auto it = std::find(points.begin(), points.end(), i);
		if(it == points.end())
		{
			index = i;
			break;
		}
	}

	if(index == -1)
		return;

	// Add index to the list while this floats up
	points.push_back(index);
	pointprogress[index] = tweeny::from(x, y).to(x, 0).during(FLOAT_DURATION);
	pointtext[index].SetText(text);
}
