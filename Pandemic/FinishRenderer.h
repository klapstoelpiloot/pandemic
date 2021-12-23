#pragma once
#include "Tools.h"
#include "IRenderer.h"
#include "Text.h"
#include "TextFlasher.h"
#include "ParticleEffect.h"

class FinishRenderer final : public virtual IRenderer
{
private:

	// Members
	Text gameovertext;
	Text scorelabel;
	Text scorecount;
	Text highscoretext;
	TextFlasher flasher;
	const Image& yellowtexture;
	const Image& graytexture;
	ParticleEffect particles;
	float scrollpos;
	float scrollspeed;
	TimePoint nextsteptime;
	bool isdailyhighscore;

	// Methods
	void SpawnSmoke(int x, int y);

public:

	FinishRenderer();

	virtual void Render(Canvas& canvas) override;
	void ShowGameOver();
	void ShowTotalScore(int count);
	void ShowHighscore(bool isdaily);
	void Clear();
};
