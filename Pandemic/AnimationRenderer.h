#pragma once
#include "Tools.h"
#include "IRenderer.h"
#include "GameDefines.h"
#include "Animator.h"

class AnimationRenderer final : public virtual IRenderer
{
private:

	// Members
	Animator ani;
	Point pos;
	bool hidewhenstopped;

public:

	AnimationRenderer();

	virtual void Render(Canvas& canvas) override;
	void SetPosition(Point position) { pos = position; }
	void Play(const Animation& animation, bool hidewhendone);
	bool HidesWhenStopped() const { return hidewhenstopped; }
	bool IsStopped() const { return ani.IsStopped(); }
};
