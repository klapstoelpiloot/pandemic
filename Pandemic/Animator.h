#pragma once
#include "Animation.h"
#include "Canvas.h"

class Animator
{
private:

	// Playing animation, nullptr when not playing anything
	const Animation* ani;

	// Current position in animation
	// Note that this is not the image index, but the index in the animation frames
	// and always progresses from 0 to the number of frames - 1.
	uint currentindex;

	// Time at which we transition to the next frame index
	TimePoint nextframetime;

	// Methods
	void AdvanceAnimation();

public:

	// Constructor
	Animator();
	~Animator();

	// Methods
	void Play(const Animation& animation);
	bool IsStopped() const;

	// Drawing
	void DrawColor(Canvas& canvas, Point pos);
	void DrawColorBlend(Canvas& canvas, Point pos);
	void DrawColorAdd(Canvas& canvas, Point pos);
	void DrawColorMask(Canvas& canvas, Point pos);
	void DrawMono(Canvas& canvas, Point pos, Color color);
	void DrawMonoBlend(Canvas& canvas, Point pos, Color color);
	void DrawMonoAdd(Canvas& canvas, Point pos, Color color);
	void DrawMonoMask(Canvas& canvas, Point pos, Color color);
};

