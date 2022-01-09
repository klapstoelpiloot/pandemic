#pragma once
#include "Tools.h"
#include "IRenderer.h"
#include "Text.h"
#include "GameDefines.h"
#include "ParticleOverlayRenderer.h"

class ClassicHUDRenderer final : public virtual IRenderer
{
private:

	// Text to render
	Text round;
	Text score;
	Text pucks;
	Text roundlabel;
	Text scorelabel;
	Text puckslabel;
	Text pointtext[8];
	TweenXY pointprogress[8];
	vector<int> points;
	const Image& texture;
	std::array<Text, GAME_GATES> gatenumbers;
	std::array<bool, GAME_GATES> gaterequired;
	ParticleEffect gateparticles;
	TimePoint fadeoutstart;
	TimePoint laststeptime;
	TimePoint scoresettime;

	// Methods
	void DrawGateLine(Canvas& canvas, int startx, int endx, Color color);
	void SpawnGateEffectBig(int startx, int endx);
	void SpawnGateEffectSmall(int startx, int endx);
	void SpawnGateParticle(int x, int y, int centerx, Color color);
	void FloatPoints(String text, int x, int y);

public:

	ClassicHUDRenderer(ParticleOverlayRenderer& particlesoverlay);

	virtual void Render(Canvas& canvas) override;
	void SetRound(int numround) { round.SetText(String::From(numround)); }
	void SetScore(int newscore) { score.SetText(String::From(newscore)); }
	void SetPucks(int numpucks) { pucks.SetText(String::From(numpucks)); }
	void SetRequiredGates(bool* gates);
	void Show() { fadeoutstart = TimePoint(); }
	void Hide() { fadeoutstart = Clock::now(); }
	void ScoreRequiredGate(int gate);
	void ScoreGate(int gate);
	void ScoreSet();
};
