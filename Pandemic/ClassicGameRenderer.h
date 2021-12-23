#pragma once
#include "Tools.h"
#include "IRenderer.h"
#include "Text.h"
#include "GameDefines.h"
#include "GameBackgroundRenderer.h"
#include "ClassicSetRenderer.h"
#include "ParticleEffect.h"

class ClassicGameRenderer final : public virtual IRenderer
{
private:

	// Text to render
	Text round;
	Text score;
	Text pucks;
	Text roundlabel;
	Text scorelabel;
	Text puckslabel;
	const Image& texture;
	std::array<Text, GAME_GATES> gatenumbers;
	std::array<bool, GAME_GATES> gaterequired;
	ParticleEffect gateparticles;
	ParticleEffect setparticles;
	ParticleEffect setsparks;
	ClassicSetRenderer setrenderer;

	// Position to render the text at
	Point position;

	// Methods
	void DrawGateLine(Canvas& canvas, int startx, int endx, Color color);
	void SpawnGateEffectBig(int startx, int endx);
	void SpawnGateEffectSmall(int startx, int endx);
	void SpawnGateParticle(int x, int y, int centerx, Color color);

public:

	ClassicGameRenderer();

	virtual void Render(Canvas& canvas) override;
	void SetRound(int numround) { round.SetText(String::From(numround)); }
	void SetScore(int newscore) { score.SetText(String::From(newscore)); }
	void SetPucks(int numpucks) { pucks.SetText(String::From(numpucks)); }
	void SetRequiredGates(bool* gates);
	void ShowSetAnimation(int numset);
	void ScoreRequiredGate(int gate);
	void ScoreGate(int gate);
};
