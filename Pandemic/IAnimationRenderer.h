#pragma once
#include "IRenderer.h"

class IAnimationRenderer : virtual public IRenderer
{
	// Make this an interface, do not allow instantiation
public:
	virtual ~IAnimationRenderer() = default;
protected:
	IAnimationRenderer() { }
	IAnimationRenderer(const IAnimationRenderer&) { }
	IAnimationRenderer& operator = (const IAnimationRenderer&) { return *this; }
public:

	// Methods
	virtual void Start() = 0;
	virtual bool HasFinished() = 0;
};
