#include <math.h>
#include "FinishRenderer.h"
#include "Main.h"

#define CENTER_X			65
#define SCORE_LABEL_X		4
#define SCORE_COUNT_X		126
#define SCROLL_START_Y		15
#define SCROLL_DISTANCE		32
#define SCROLL_SPEED		0.2f
#define TIME_STEP			10
#define NUM_SMOKE_IMAGES	6
#define NUM_SMOKES			60

FinishRenderer::FinishRenderer() :
	gameovertext(Main::GetResources().BoldBitsLarge(), HorizontalAlign::Center, VerticalAlign::Middle),
	scorelabel(Main::GetResources().BoldBitsLarge(), HorizontalAlign::Left, VerticalAlign::Middle),
	scorecount(Main::GetResources().BoldBitsLarge(), HorizontalAlign::Right, VerticalAlign::Middle),
	highscoretext(Main::GetResources().BoldBitsLarge(), HorizontalAlign::Center, VerticalAlign::Middle),
	yellowtexture(Main::GetResources().GetImage("yellow22d.dds")),
	graytexture(Main::GetResources().GetImage("gray22d.dds")),
	scrollpos(SCROLL_START_Y),
	scrollspeed(0.0)
{
	particles.SetAdditive(false);
	particles.SetDeceleration(0.97f);
	particles.SetFadeAlpha(-0.5f);
	particles.SetGravity(glm::vec2(0.0f, -0.1f));
	particles.SetSwingStrength(-0.005f);
	particles.SetLifetime(5000);
	for(int i = 0; i < NUM_SMOKE_IMAGES; i++)
		particles.AddImage(&(Main::GetResources().GetImage("smoke" + String::From(i + 1) + ".dds")));
}

void FinishRenderer::Render(Canvas& canvas)
{
	if(ch::IsTimeSet(nextsteptime))
	{
		// Step time forward and scroll
		while(Clock::now() >= nextsteptime)
		{
			scrollspeed += SCROLL_SPEED;
			scrollpos += scrollspeed;
			nextsteptime += ch::milliseconds(TIME_STEP);
		}

		if(scrollpos > (SCROLL_START_Y + SCROLL_DISTANCE))
		{
			// Reached the end of the scroll
			nextsteptime = TimePoint();
			scrollpos = SCROLL_START_Y + SCROLL_DISTANCE;
			Main::GetResources().GetSound("bang.wav").Play();

			// Spawn some particles at the bottom
			for(int i = 0; i < NUM_SMOKES; i++)
				SpawnSmoke(Random(2, 126), Random(20, 26));

			flasher.Begin();
		}
	}

	int scoreposint = static_cast<int>(std::round(scrollpos));
	int gameovery = scoreposint;
	int scorey = (gameovertext.GetText().Length() > 0) ? (scoreposint - SCROLL_DISTANCE) : scoreposint;
	int highscorey = scoreposint - SCROLL_DISTANCE;

	gameovertext.DrawOutlineMask(canvas, Point(CENTER_X, gameovery), 2, BLACK);
	scorelabel.DrawOutlineMask(canvas, Point(SCORE_LABEL_X, scorey), 2, BLACK);
	scorecount.DrawOutlineMask(canvas, Point(SCORE_COUNT_X, scorey), 2, BLACK);
	highscoretext.DrawOutlineMask(canvas, Point(CENTER_X, highscorey), 2, BLACK);

	gameovertext.DrawTexturedMask(canvas, Point(CENTER_X, gameovery), yellowtexture);
	scorelabel.DrawTexturedMask(canvas, Point(SCORE_LABEL_X, scorey), yellowtexture);
	scorecount.DrawTexturedMask(canvas, Point(SCORE_COUNT_X, scorey), yellowtexture);
	highscoretext.DrawTexturedMask(canvas, Point(CENTER_X, highscorey), isdailyhighscore ? graytexture : yellowtexture);

	flasher.Draw(canvas, gameovertext, Point(CENTER_X, gameovery), 2);
	flasher.Draw(canvas, scorelabel, Point(SCORE_LABEL_X, scorey), 2);
	flasher.Draw(canvas, scorecount, Point(SCORE_COUNT_X, scorey), 2);
	flasher.Draw(canvas, highscoretext, Point(CENTER_X, highscorey), 2);

	particles.Render(canvas);
}

void FinishRenderer::ShowGameOver()
{
	Clear();
	particles.Begin();
	gameovertext.SetText("GAME OVER");
	scrollpos = SCROLL_START_Y;
}

void FinishRenderer::ShowTotalScore(int count)
{
	scorelabel.SetText("SCORE");
	scorecount.SetText(String::From(count));
	nextsteptime = Clock::now();
	scrollpos = SCROLL_START_Y;
	scrollspeed = SCROLL_SPEED;
}

void FinishRenderer::ShowHighscore(bool isdaily)
{
	isdailyhighscore = isdaily;
	gameovertext.SetText("");
	highscoretext.SetText("HIGHSCORE");
	nextsteptime = Clock::now();
	scrollpos = SCROLL_START_Y;
	scrollspeed = SCROLL_SPEED;
}

void FinishRenderer::Clear()
{
	gameovertext.SetText("");
	scorelabel.SetText("");
	scorecount.SetText("");
	highscoretext.SetText("");
	nextsteptime = TimePoint();
	scrollspeed = 0.0;
}

void FinishRenderer::SpawnSmoke(int x, int y)
{
	int image = Random(0, NUM_SMOKE_IMAGES - 1);
	glm::vec2 pos(static_cast<float>(x), static_cast<float>(y));
	glm::vec2 vel(Random(-0.5f, 0.5f), Random(-0.5f, 0.8f));
	byte gray = static_cast<byte>(Random(0, 150));
	Color color(gray, gray, gray);
	float alpha = Random(0.3f, 1.0f);

	// Meta is used for Swing effect
	// X = x position to swing about
	// Y = amount of swing factor
	glm::vec2 meta(64.0f, Random(0.1f, 1.0f));

	particles.Spawn(Particle(image, pos, vel, color, alpha, meta));
}
