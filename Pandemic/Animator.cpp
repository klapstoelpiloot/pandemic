#include "Animator.h"

Animator::Animator() :
	ani(nullptr),
	currentindex(0)
{
}

Animator::~Animator()
{
	ani = nullptr;
}

void Animator::Play(const Animation& animation)
{
	ani = &animation;
	currentindex = 0;
	nextframetime = Clock::now() + ch::milliseconds(ani->GetFrames()[0].duration);
}

bool Animator::IsStopped() const
{
	return (ani == nullptr) || ((currentindex == (ani->GetFrames().size() - 1)) && (Clock::now() >= nextframetime));
}

void Animator::AdvanceAnimation()
{
	REQUIRE(ani != nullptr);
	while((Clock::now() >= nextframetime) && (currentindex < (ani->GetFrames().size() - 1)))
	{
		currentindex++;
		nextframetime += ch::milliseconds(ani->GetFrames()[currentindex].duration);
	}
}

void Animator::DrawColor(Canvas& canvas, Point pos)
{
	if(ani == nullptr)
		return;

	AdvanceAnimation();
	Rect imgrect(ani->GetFrames()[currentindex].imagepos, ani->GetFrameSize());
	canvas.DrawColorImage(pos, ani->GetImage(), imgrect);
}

void Animator::DrawColorBlend(Canvas& canvas, Point pos)
{
	if(ani == nullptr)
		return;

	AdvanceAnimation();
	Rect imgrect(ani->GetFrames()[currentindex].imagepos, ani->GetFrameSize());
	canvas.DrawColorImageBlend(pos, ani->GetImage(), imgrect);
}

void Animator::DrawColorAdd(Canvas& canvas, Point pos)
{
	if(ani == nullptr)
		return;

	AdvanceAnimation();
	Rect imgrect(ani->GetFrames()[currentindex].imagepos, ani->GetFrameSize());
	canvas.DrawColorImageAdd(pos, ani->GetImage(), imgrect);
}

void Animator::DrawColorMask(Canvas& canvas, Point pos)
{
	if(ani == nullptr)
		return;

	AdvanceAnimation();
	Rect imgrect(ani->GetFrames()[currentindex].imagepos, ani->GetFrameSize());
	canvas.DrawColorImageMask(pos, ani->GetImage(), imgrect);
}

void Animator::DrawMono(Canvas& canvas, Point pos, Color color)
{
	if(ani == nullptr)
		return;

	AdvanceAnimation();
	Rect imgrect(ani->GetFrames()[currentindex].imagepos, ani->GetFrameSize());
	canvas.DrawMonoImage(pos, ani->GetImage(), color, imgrect);
}

void Animator::DrawMonoBlend(Canvas& canvas, Point pos, Color color)
{
	if(ani == nullptr)
		return;

	AdvanceAnimation();
	Rect imgrect(ani->GetFrames()[currentindex].imagepos, ani->GetFrameSize());
	canvas.DrawMonoImageBlend(pos, ani->GetImage(), color, imgrect);
}

void Animator::DrawMonoAdd(Canvas& canvas, Point pos, Color color)
{
	if(ani == nullptr)
		return;

	AdvanceAnimation();
	Rect imgrect(ani->GetFrames()[currentindex].imagepos, ani->GetFrameSize());
	canvas.DrawMonoImageAdd(pos, ani->GetImage(), color, imgrect);
}

void Animator::DrawMonoMask(Canvas& canvas, Point pos, Color color)
{
	if(ani == nullptr)
		return;

	AdvanceAnimation();
	Rect imgrect(ani->GetFrames()[currentindex].imagepos, ani->GetFrameSize());
	canvas.DrawMonoImageMask(pos, ani->GetImage(), color, imgrect);
}
