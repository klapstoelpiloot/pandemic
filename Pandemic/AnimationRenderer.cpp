#include "AnimationRenderer.h"

AnimationRenderer::AnimationRenderer() :
	hidewhenstopped(true)
{
}

void AnimationRenderer::Render(Canvas& canvas)
{
	if(!hidewhenstopped || !ani.IsStopped())
		ani.DrawColorBlend(canvas, pos);
}

void AnimationRenderer::Play(const Animation& animation, bool hidewhendone)
{
	ani.Play(animation);
	hidewhenstopped = hidewhendone;
}
