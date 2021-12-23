#include "ClassicGameRenderer.h"
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

String GATE_NUMBERS[] = { "2", "3", "4", "1" };

ClassicGameRenderer::ClassicGameRenderer() :
	round(Main::GetResources().GetFont("pixel7.fnt"), HorizontalAlign::Left, VerticalAlign::Middle),
	score(Main::GetResources().GetFont("pixel7.fnt"), HorizontalAlign::Right, VerticalAlign::Middle),
	pucks(Main::GetResources().GetFont("pixel7.fnt"), HorizontalAlign::Center, VerticalAlign::Middle),
	roundlabel(Main::GetResources().Smallest(), HorizontalAlign::Left, VerticalAlign::Top),
	scorelabel(Main::GetResources().Smallest(), HorizontalAlign::Right, VerticalAlign::Top),
	puckslabel(Main::GetResources().Smallest(), HorizontalAlign::Center, VerticalAlign::Top),
	texture(Main::GetResources().GetImage("yellow12.dds")),
	gaterequired{ {false, false, false, false} },
	setrenderer(setparticles, setsparks)
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
	gateparticles.SetGravity(Vec2(0.0f, -1.8f));
	gateparticles.SetSwingStrength(0.05f);
	gateparticles.SetLifetime(3000);
	gateparticles.Begin();

	setparticles.SetAdditive(true);
	setparticles.SetDeceleration(0.95f);
	setparticles.SetFadeAlpha(-3.0f);
	setparticles.SetGravity(Vec2(0.0f, -1.0f));
	setparticles.SetSwingStrength(0.02f);
	setparticles.SetLifetime(3000);
	setparticles.Begin();

	setsparks.SetAdditive(true);
	setsparks.SetDeceleration(0.95f);
	setsparks.SetFadeAlpha(-3.0f);
	setsparks.SetGravity(Vec2(0.0f, -1.0f));
	setsparks.SetSwingStrength(0.02f);
	setsparks.SetLifetime(3000);
	for(int i = 0; i < NUM_SPARK_IMAGES; i++)
		setsparks.AddImage(&(Main::GetResources().GetImage("spark" + String::From(i + 1) + ".dds")));
	setsparks.Begin();
}

void ClassicGameRenderer::Render(Canvas& canvas)
{
	// If a set animation is being played, then render only that
	setrenderer.Render(canvas);
	if(setrenderer.IsVisible())
		return;

	// Determine gate flash color
	bool gateflashbright = (((ToMilliseconds(Clock::now()) / GATE_FLASH_INTERVAL) % 2) == 0);
	Color gateflashcolor = gateflashbright ? GATE_FLASH_COLOR1 : GATE_FLASH_COLOR2;
	Color gate1color = gaterequired[0] ? gateflashcolor : GATE_NORMAL_COLOR;
	Color gate2color = gaterequired[1] ? gateflashcolor : GATE_NORMAL_COLOR;
	Color gate3color = gaterequired[2] ? gateflashcolor : GATE_NORMAL_COLOR;
	Color gate4color = gaterequired[3] ? gateflashcolor : GATE_NORMAL_COLOR;

	// Draw lines above the gates
	DrawGateLine(canvas, GATE1_START, GATE1_END, gate1color);
	DrawGateLine(canvas, GATE2_START, GATE2_END, gate2color);
	DrawGateLine(canvas, GATE3_START, GATE3_END, gate3color);
	DrawGateLine(canvas, GATE4_START, GATE4_END, gate4color);

	// Draw gate numbers
	gatenumbers[0].DrawOutlineMask(canvas, Point((GATE1_START + GATE1_END) / 2, GATE_NUMBER_Y), 1, GATE_OUTLINE);
	gatenumbers[1].DrawOutlineMask(canvas, Point((GATE2_START + GATE2_END) / 2, GATE_NUMBER_Y), 1, GATE_OUTLINE);
	gatenumbers[2].DrawOutlineMask(canvas, Point((GATE3_START + GATE3_END) / 2, GATE_NUMBER_Y), 1, GATE_OUTLINE);
	gatenumbers[3].DrawOutlineMask(canvas, Point((GATE4_START + GATE4_END) / 2, GATE_NUMBER_Y), 1, GATE_OUTLINE);
	gatenumbers[0].DrawMask(canvas, Point((GATE1_START + GATE1_END) / 2, GATE_NUMBER_Y), gate1color);
	gatenumbers[1].DrawMask(canvas, Point((GATE2_START + GATE2_END) / 2, GATE_NUMBER_Y), gate2color);
	gatenumbers[2].DrawMask(canvas, Point((GATE3_START + GATE3_END) / 2, GATE_NUMBER_Y), gate3color);
	gatenumbers[3].DrawMask(canvas, Point((GATE4_START + GATE4_END) / 2, GATE_NUMBER_Y), gate4color);

	// Draw counters
	round.DrawOutlineMask(canvas, Point(ROUND_X, COUNTERS_Y), 1, COUNTERS_OUTLINE);
	score.DrawOutlineMask(canvas, Point(SCORE_X, COUNTERS_Y), 1, COUNTERS_OUTLINE);
	pucks.DrawOutlineMask(canvas, Point(PUCKS_X, COUNTERS_Y), 1, COUNTERS_OUTLINE);
	round.DrawTexturedMask(canvas, Point(ROUND_X, COUNTERS_Y), texture);
	score.DrawTexturedMask(canvas, Point(SCORE_X, COUNTERS_Y), texture);
	pucks.DrawTexturedMask(canvas, Point(PUCKS_X, COUNTERS_Y), texture);

	// Draw counter labels
	roundlabel.DrawOutlineMask(canvas, Point(ROUND_X, LABELS_Y), 1, LABELS_OUTLINE);
	scorelabel.DrawOutlineMask(canvas, Point(SCORE_X, LABELS_Y), 1, LABELS_OUTLINE);
	puckslabel.DrawOutlineMask(canvas, Point(PUCKS_X, LABELS_Y), 1, LABELS_OUTLINE);
	roundlabel.DrawMask(canvas, Point(ROUND_X, LABELS_Y), LABELS_COLOR);
	scorelabel.DrawMask(canvas, Point(SCORE_X, LABELS_Y), LABELS_COLOR);
	puckslabel.DrawMask(canvas, Point(PUCKS_X, LABELS_Y), LABELS_COLOR);

	setparticles.Render(canvas);
	gateparticles.Render(canvas);
	setsparks.Render(canvas);
}

void ClassicGameRenderer::SetRequiredGates(bool* gates)
{
	for(int g = 0; g < GAME_GATES; g++)
		gaterequired[g] = gates[g];
}

void ClassicGameRenderer::ShowSetAnimation(int numset)
{
	setrenderer.ShowSet(numset);
}

void ClassicGameRenderer::ScoreRequiredGate(int gate)
{
	switch(gate)
	{
		case 0: SpawnGateEffectBig(GATE1_START, GATE1_END); break;
		case 1: SpawnGateEffectBig(GATE2_START, GATE2_END); break;
		case 2: SpawnGateEffectBig(GATE3_START, GATE3_END); break;
		case 3: SpawnGateEffectBig(GATE4_START, GATE4_END); break;
	}
}

void ClassicGameRenderer::ScoreGate(int gate)
{
	switch(gate)
	{
		case 0: SpawnGateEffectSmall(GATE1_START, GATE1_END); break;
		case 1: SpawnGateEffectSmall(GATE2_START, GATE2_END); break;
		case 2: SpawnGateEffectSmall(GATE3_START, GATE3_END); break;
		case 3: SpawnGateEffectSmall(GATE4_START, GATE4_END); break;
	}
}

void ClassicGameRenderer::DrawGateLine(Canvas& canvas, int startx, int endx, Color color)
{
	// Outline on the left
	canvas.SetPixel(startx - 1, 29, GATE_OUTLINE);
	for(int y = 0; y < GATE_LINE_HEIGHT; y++)
		canvas.SetPixel(startx - 1, 31 - y, GATE_OUTLINE);

	// Horizontal lines
	for(int x = startx; x <= endx; x++)
	{
		canvas.SetPixel(x, 29, GATE_OUTLINE);
		for(int y = 0; y < GATE_LINE_HEIGHT; y++)
			canvas.SetPixel(x, 31 - y, color);
	}

	// Outline on the right
	canvas.SetPixel(endx + 1, 29, GATE_OUTLINE);
	for(int y = 0; y < GATE_LINE_HEIGHT; y++)
		canvas.SetPixel(endx + 1, 31 - y, GATE_OUTLINE);
}

void ClassicGameRenderer::SpawnGateEffectBig(int startx, int endx)
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

void ClassicGameRenderer::SpawnGateEffectSmall(int startx, int endx)
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

void ClassicGameRenderer::SpawnGateParticle(int x, int y, int centerx, Color color)
{
	Vec2 p(static_cast<float>(x), static_cast<float>(y));

	// Random start velocity
	Vec2 v(Random(-1.5f, 1.5f), Random(0.2f, -1.0f));

	// Random alpha
	float a = Random(0.5f, 2.5f);

	// Meta is used for Swing effect
	// X = x position to swing about
	// Y = amount of swing factor
	Vec2 meta(static_cast<float>(centerx), Random(0.1f, 1.0f));

	// Make the particle
	gateparticles.Spawn(Particle(p, v, color, a, meta));
}
