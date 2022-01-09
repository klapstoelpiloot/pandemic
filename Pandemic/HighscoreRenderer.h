#pragma once
#include "Tools.h"
#include "IRenderer.h"
#include "Text.h"
#include "ScoresTable.h"
#include "GameDesc.h"
#include "ScoresPeriod.h"
#include "TextShine.h"

class HighscoreRenderer final : public virtual IRenderer
{
private:

	// Members
	Text hiscoretext;
	Text periodscoretext;
	Text higametext;
	Text periodgametext;
	vector<Text*> postexts;
	vector<Text*> nametexts;
	vector<Text*> datetexts;
	vector<Text*> scoretexts;
	const Image& yellowtexture;
	const Image& graytexture;
	const Image& yellowtrophy;
	const Image& graytrophy;
	TimePoint begintime;
	TimePoint scrollstarttime;
	size_t numhiscores;
	size_t numdayscores;
	int hipagesize;
	int daypagesize;
	bool continuousloop;
	TextShine shine;
	TimePoint nextshinetime;

	// Methods
	void Cleanup();
	String FormatDate(TimePoint tp);
	int CalculateY(int firsty, int scrollpos);
	void MakeTextFromRecord(const ScoreRecord& record, int pos);
	void MakeText(String pos, String name, String score, String date);

public:

	HighscoreRenderer();
	virtual ~HighscoreRenderer();

	// Methods
	virtual void Render(Canvas& canvas) override;
	void Setup(const ScoresTable& hightable, const ScoresTable& daytable, GameType type, ScoresPeriod interval);
	void SetContinuousLoop(bool loop) { continuousloop = loop; }
	bool IsFinished();
};
