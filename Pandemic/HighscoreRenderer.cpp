#include "HighscoreRenderer.h"
#include "Main.h"

#define SCROLL_DELAY		2000
#define CENTER_X			64
#define GAME_TEXT_Y			9
#define SCOPE_TEXT_Y		22
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

HighscoreRenderer::HighscoreRenderer() :
	hiscopetext("ALLTIME HIGHSCORE", Main::GetResources().BoldBits(), HorizontalAlign::Center, VerticalAlign::Middle),
	dayscopetext("DAILY HIGHSCORE", Main::GetResources().BoldBits(), HorizontalAlign::Center, VerticalAlign::Middle),
	higametext(Main::GetResources().BoldBits(), HorizontalAlign::Center, VerticalAlign::Middle),
	daygametext(Main::GetResources().BoldBits(), HorizontalAlign::Center, VerticalAlign::Middle),
	yellowtexture(Main::GetResources().GetImage("yellow12.dds")),
	graytexture(Main::GetResources().GetImage("gray12.dds")),
	yellowtrophy(Main::GetResources().GetImage("yellow_trophy.dds")),
	graytrophy(Main::GetResources().GetImage("gray_trophy.dds")),
	numhiscores(0),
	numdayscores(0),
	hipagesize(0),
	daypagesize(0),
	continuousloop(true)
{
}

HighscoreRenderer::~HighscoreRenderer()
{
	Cleanup();
}

void HighscoreRenderer::Render(Canvas& canvas)
{
	// Determine scroll position
	int scrollpos = 0;
	if(Clock::now() > scrollstarttime)
		scrollpos = static_cast<int>(ch::ToMilliseconds(Clock::now() - scrollstarttime) / SCROLL_SPEED);

	// Start drawing
	int hitrophyy = CalculateY(TROPHY_Y, scrollpos);
	int daytrophyy = CalculateY(TROPHY_Y + hipagesize, scrollpos);
	int hiscopetexty = CalculateY(SCOPE_TEXT_Y, scrollpos);
	int higametexty = CalculateY(GAME_TEXT_Y, scrollpos);
	int dayscopetexty = CalculateY(SCOPE_TEXT_Y + hipagesize, scrollpos);
	int daygametexty = CalculateY(GAME_TEXT_Y + hipagesize, scrollpos);

	canvas.DrawColorImageMask(Point(CENTER_X - (higametext.GetTextSize().width / 2) - 18, hitrophyy), yellowtrophy);
	canvas.DrawColorImageMask(Point(CENTER_X + (higametext.GetTextSize().width / 2) + 4, hitrophyy), yellowtrophy);
	canvas.DrawColorImageMask(Point(CENTER_X - (daygametext.GetTextSize().width / 2) - 18, daytrophyy), graytrophy);
	canvas.DrawColorImageMask(Point(CENTER_X + (daygametext.GetTextSize().width / 2) + 4, daytrophyy), graytrophy);
	hiscopetext.DrawOutlineMask(canvas, Point(CENTER_X, hiscopetexty), 1, BLACK);
	hiscopetext.DrawTexturedMask(canvas, Point(CENTER_X, hiscopetexty), yellowtexture);
	higametext.DrawOutlineMask(canvas, Point(CENTER_X, higametexty), 1, BLACK);
	higametext.DrawTexturedMask(canvas, Point(CENTER_X, higametexty), yellowtexture);
	dayscopetext.DrawOutlineMask(canvas, Point(CENTER_X, dayscopetexty), 1, BLACK);
	dayscopetext.DrawTexturedMask(canvas, Point(CENTER_X, dayscopetexty), graytexture);
	daygametext.DrawOutlineMask(canvas, Point(CENTER_X, daygametexty), 1, BLACK);
	daygametext.DrawTexturedMask(canvas, Point(CENTER_X, daygametexty), graytexture);
	for(size_t i = 0; i < postexts.size(); i++)
	{
		int offset;
		if(i < numhiscores)
			offset = FIRST_ITEM_Y + (ITEM_SPACING * static_cast<int>(i));
		else
			offset = FIRST_ITEM_Y + (ITEM_SPACING * static_cast<int>(i - numhiscores)) + hipagesize;
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

void HighscoreRenderer::Setup(const ScoresTable& hightable, const ScoresTable& daytable, GameType type)
{
	Cleanup();

	// Describe the game type
	const GameDesc& gamedesc = GameDesc::Find(type);
	higametext.SetText(gamedesc.name);
	daygametext.SetText(gamedesc.name);

	// List the alltime highscores
	if(hightable.size() > 0)
	{
		int pos = 1;
		for(const ScoreRecord& record : hightable)
			MakeTextFromRecord(record, pos++);
		numhiscores = hightable.size();
	}
	else
	{
		MakeText(EMPTY_LIST_TEXT, "", "", "");
		numhiscores = 1;
	}

	// I want at least 2 items per page, pad with empty item...
	if(numhiscores == 1)
	{
		MakeText("", "", "", "");
		numhiscores++;
	}

	// List the daily highscores
	if(daytable.size() > 0)
	{
		int pos = 1;
		for(const ScoreRecord& record : daytable)
			MakeTextFromRecord(record, pos++);
		numdayscores = daytable.size();
	}
	else
	{
		MakeText(EMPTY_LIST_TEXT, "", "", "");
		numdayscores = 1;
	}

	// I want at least 2 items per page, pad with empty item...
	if(numdayscores == 1)
	{
		MakeText("", "", "", "");
		numdayscores++;
	}

	// Determine page sizes.
	// A page is a single table with headings included and the size is in pixels.
	hipagesize = FIRST_ITEM_Y + (numhiscores * ITEM_SPACING);
	daypagesize = FIRST_ITEM_Y + (numdayscores * ITEM_SPACING);
	begintime = Clock::now();
	scrollstarttime = Clock::now() + ch::milliseconds(SCROLL_DELAY);
}

bool HighscoreRenderer::IsFinished()
{
	if(continuousloop || (Clock::now() < scrollstarttime))
		return false;

	int scrollpos = static_cast<int>(ch::ToMilliseconds(Clock::now() - scrollstarttime) / SCROLL_SPEED);
	return (scrollpos > (hipagesize + daypagesize));
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
	int twopagesize = hipagesize + daypagesize;
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
