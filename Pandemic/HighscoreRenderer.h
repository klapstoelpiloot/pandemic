#pragma once
#include "Tools.h"
#include "IRenderer.h"
#include "Text.h"
#include "ScoresTable.h"
#include "GameDesc.h"

class HighscoreRenderer final : public virtual IRenderer
{
private:

	// Members
	Text hiscopetext;
	Text dayscopetext;
	Text higametext;
	Text daygametext;
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
	void Setup(const ScoresTable& hightable, const ScoresTable& daytable, GameType type);
	void SetContinuousLoop(bool loop) { continuousloop = loop; }
	bool IsFinished();
};
