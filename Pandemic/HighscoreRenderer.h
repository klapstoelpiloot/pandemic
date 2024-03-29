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
	Text hiscoretext1;
	Text hiscoretext2;
	Text gametypetext1;
	Text gametypetext2;
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
	size_t numhiscores1;
	size_t numhiscores2;
	int pagesize1;
	int pagesize2;
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
	void Setup(const ScoresTable& casualtable, const ScoresTable& protable);
	void SetContinuousLoop(bool loop) { continuousloop = loop; }
	bool IsFinished();
};
