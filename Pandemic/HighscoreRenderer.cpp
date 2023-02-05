#include "HighscoreRenderer.h"
#include "Main.h"

#define SCROLL_DELAY		2000
#define CENTER_X			64
#define GAMETYPE_TEXT_Y		9
#define HISCORE_TEXT_Y		22
#define TROPHY_Y			4
#define FIRST_ITEM_Y		35
#define POS_X				2
#define NAME_X				13
#define SCORE_X				84
#define DATE_X				127
#define ITEM_SPACING		8
#define SCROLL_SPEED		80		// Milliseconds per pixel
#define DATE_COLOR			Color(132, 132, 132)
#define POS_COLOR			Color(132, 132, 132)
#define NAME_COLOR			WHITE
#define SCORE_COLOR			Color(255, 220, 62)
#define EMPTY_LIST_TEXT		"             NOBODY YET"
#define SHINE_INTERVAL		ch::milliseconds(1000)

HighscoreRenderer::HighscoreRenderer() :
	hiscoretext1("ALLTIME HIGHSCORE", Main::GetResources().BoldBits(), HorizontalAlign::Center, VerticalAlign::Middle),
	hiscoretext2("ALLTIME HIGHSCORE", Main::GetResources().BoldBits(), HorizontalAlign::Center, VerticalAlign::Middle),
	gametypetext1(Main::GetResources().BoldBits(), HorizontalAlign::Center, VerticalAlign::Middle),
	gametypetext2(Main::GetResources().BoldBits(), HorizontalAlign::Center, VerticalAlign::Middle),
	yellowtexture(Main::GetResources().GetImage("yellow12.dds")),
	graytexture(Main::GetResources().GetImage("gray12.dds")),
	yellowtrophy(Main::GetResources().GetImage("yellow_trophy.dds")),
	graytrophy(Main::GetResources().GetImage("gray_trophy.dds")),
	numhiscores1(0),
	numhiscores2(0),
	pagesize1(0),
	pagesize2(0),
	continuousloop(true)
{
}

HighscoreRenderer::~HighscoreRenderer()
{
	Cleanup();
}

void HighscoreRenderer::Render(Canvas& canvas)
{
	TimePoint t = Clock::now();
	// Determine scroll position
	int scrollpos = 0;
	if(t > scrollstarttime)
		scrollpos = static_cast<int>(ch::ToMilliseconds(t - scrollstarttime) / SCROLL_SPEED);

	// Start drawing
	int trophy1y = CalculateY(TROPHY_Y, scrollpos);
	int trophy2y = CalculateY(TROPHY_Y + pagesize1, scrollpos);
	int hiscoretext1y = CalculateY(HISCORE_TEXT_Y, scrollpos);
	int gametypetext1y = CalculateY(GAMETYPE_TEXT_Y, scrollpos);
	int hiscoretext2y = CalculateY(HISCORE_TEXT_Y + pagesize1, scrollpos);
	int gametypetext2y = CalculateY(GAMETYPE_TEXT_Y + pagesize1, scrollpos);

	if((t > nextshinetime) && ((gametypetext1y == GAMETYPE_TEXT_Y) || (gametypetext2y == GAMETYPE_TEXT_Y)))
	{
		nextshinetime += SHINE_INTERVAL;
		shine.Begin(Size(DISPLAY_WIDTH, DISPLAY_HEIGHT));
	}

	Point yt1 = Point(CENTER_X - (gametypetext1.GetTextSize().width / 2) - 18, trophy1y);
	Point yt2 = Point(CENTER_X + (gametypetext1.GetTextSize().width / 2) + 4, trophy1y);
	Point gt1 = Point(CENTER_X - (gametypetext2.GetTextSize().width / 2) - 18, trophy2y);
	Point gt2 = Point(CENTER_X + (gametypetext2.GetTextSize().width / 2) + 4, trophy2y);
	canvas.DrawColorImageMask(yt1, yellowtrophy);
	canvas.DrawColorImageMask(yt2, yellowtrophy);
	canvas.DrawColorImageMask(gt1, graytrophy);
	canvas.DrawColorImageMask(gt2, graytrophy);
	shine.Draw(canvas, yellowtrophy, yt1);
	shine.Draw(canvas, yellowtrophy, yt2);
	shine.Draw(canvas, graytrophy, gt1);
	shine.Draw(canvas, graytrophy, gt2);
	hiscoretext1.DrawOutlineMask(canvas, Point(CENTER_X, hiscoretext1y), 1, BLACK);
	hiscoretext1.DrawTexturedMask(canvas, Point(CENTER_X, hiscoretext1y), yellowtexture);
	shine.Draw(canvas, hiscoretext1, 1, Point(CENTER_X, hiscoretext1y));
	gametypetext1.DrawOutlineMask(canvas, Point(CENTER_X, gametypetext1y), 1, BLACK);
	gametypetext1.DrawTexturedMask(canvas, Point(CENTER_X, gametypetext1y), yellowtexture);
	shine.Draw(canvas, gametypetext1, 1, Point(CENTER_X, gametypetext1y));
	hiscoretext2.DrawOutlineMask(canvas, Point(CENTER_X, hiscoretext2y), 1, BLACK);
	hiscoretext2.DrawTexturedMask(canvas, Point(CENTER_X, hiscoretext2y), graytexture);
	shine.Draw(canvas, hiscoretext2, 1, Point(CENTER_X, hiscoretext2y));
	gametypetext2.DrawOutlineMask(canvas, Point(CENTER_X, gametypetext2y), 1, BLACK);
	gametypetext2.DrawTexturedMask(canvas, Point(CENTER_X, gametypetext2y), graytexture);
	shine.Draw(canvas, gametypetext2, 1, Point(CENTER_X, gametypetext2y));
	for(size_t i = 0; i < postexts.size(); i++)
	{
		int offset;
		if(i < numhiscores1)
			offset = FIRST_ITEM_Y + (ITEM_SPACING * static_cast<int>(i));
		else
			offset = FIRST_ITEM_Y + (ITEM_SPACING * static_cast<int>(i - numhiscores1)) + pagesize1;
		int posy = CalculateY(offset, scrollpos);

		postexts[i]->DrawOutlineMask(canvas, Point(POS_X, posy), 1, BLACK);
		postexts[i]->DrawMask(canvas, Point(POS_X, posy), POS_COLOR);
		nametexts[i]->DrawOutlineMask(canvas, Point(NAME_X, posy), 1, BLACK);
		nametexts[i]->DrawMask(canvas, Point(NAME_X, posy), NAME_COLOR);
		scoretexts[i]->DrawOutlineMask(canvas, Point(SCORE_X, posy), 1, BLACK);
		scoretexts[i]->DrawMask(canvas, Point(SCORE_X, posy), SCORE_COLOR);
		datetexts[i]->DrawOutlineMask(canvas, Point(DATE_X, posy), 1, BLACK);
		datetexts[i]->DrawMask(canvas, Point(DATE_X, posy), DATE_COLOR);
	}
}

void HighscoreRenderer::Setup(const ScoresTable& casualtable, const ScoresTable& protable)
{
	Cleanup();

	// Describe the game type
	gametypetext1.SetText(GameDesc::Find(GameType::Casual).name);
	gametypetext2.SetText(GameDesc::Find(GameType::Pro).name);

	// List the casual highscores
	if(casualtable.size() > 0)
	{
		int pos = 1;
		for(const ScoreRecord& record : casualtable)
			MakeTextFromRecord(record, pos++);
		numhiscores1 = casualtable.size();
	}
	else
	{
		MakeText(EMPTY_LIST_TEXT, "", "", "");
		numhiscores1 = 1;
	}

	// I want at least 2 items per page, pad with empty item...
	if(numhiscores1 == 1)
	{
		MakeText("", "", "", "");
		numhiscores1++;
	}

	// List the pro highscores
	if(protable.size() > 0)
	{
		int pos = 1;
		for(const ScoreRecord& record : protable)
			MakeTextFromRecord(record, pos++);
		numhiscores2 = protable.size();
	}
	else
	{
		MakeText(EMPTY_LIST_TEXT, "", "", "");
		numhiscores2 = 1;
	}

	// I want at least 2 items per page, pad with empty item...
	if(numhiscores2 == 1)
	{
		MakeText("", "", "", "");
		numhiscores2++;
	}

	// Determine page sizes.
	// A page is a single table with headings included and the size is in pixels.
	pagesize1 = FIRST_ITEM_Y + (numhiscores1 * ITEM_SPACING);
	pagesize2 = FIRST_ITEM_Y + (numhiscores2 * ITEM_SPACING);
	begintime = Clock::now();
	scrollstarttime = Clock::now() + ch::milliseconds(SCROLL_DELAY);
	nextshinetime = Clock::now() + SHINE_INTERVAL;
}

bool HighscoreRenderer::IsFinished()
{
	if(continuousloop || (Clock::now() < scrollstarttime))
		return false;

	int scrollpos = static_cast<int>(ch::ToMilliseconds(Clock::now() - scrollstarttime) / SCROLL_SPEED);
	return (scrollpos > (pagesize1 + pagesize2));
}

void HighscoreRenderer::Cleanup()
{
	for(Text* t : postexts)
		delete t;
	for(Text* t : nametexts)
		delete t;
	for(Text* t : datetexts)
		delete t;
	for(Text* t : scoretexts)
		delete t;
	postexts.clear();
	nametexts.clear();
	datetexts.clear();
	scoretexts.clear();
}

String HighscoreRenderer::FormatDate(TimePoint tp)
{
	time_t tt = TimePoint::clock::to_time_t(tp);
	tm ttparts;
	localtime_r(&tt, &ttparts);
	return String::From(ttparts.tm_mday) + "-" + String::From(ttparts.tm_mon + 1) + "-" + String::From(ttparts.tm_year + 1900).Substring(2);
}

int HighscoreRenderer::CalculateY(int firsty, int scrollpos)
{
	int twopagesize = pagesize1 + pagesize2;
	int twopageindex = (scrollpos - firsty + (twopagesize / 2)) / twopagesize;
	return firsty + (twopageindex * twopagesize) - scrollpos;
}

void HighscoreRenderer::MakeTextFromRecord(const ScoreRecord& record, int pos)
{
	MakeText(String::From(pos), record.name.Substring(0, 10), String::From(record.score), FormatDate(record.date));
}

void HighscoreRenderer::MakeText(String pos, String name, String score, String date)
{
	Text* pt = new Text(pos, Main::GetResources().Smallest(), HorizontalAlign::Left, VerticalAlign::Middle);
	Text* nt = new Text(name, Main::GetResources().Smallest(), HorizontalAlign::Left, VerticalAlign::Middle);
	Text* dt = new Text(date, Main::GetResources().Smallest(), HorizontalAlign::Right, VerticalAlign::Middle);
	Text* st = new Text(score, Main::GetResources().Smallest(), HorizontalAlign::Right, VerticalAlign::Middle);
	postexts.push_back(pt);
	nametexts.push_back(nt);
	datetexts.push_back(dt);
	scoretexts.push_back(st);
}
