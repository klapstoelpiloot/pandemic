#pragma once
#include "Canvas.h"

class IRenderer
{
	// Make this an interface, do not allow instantiation
public:
	virtual ~IRenderer() = default;
protected:
	IRenderer() { }
	IRenderer(const IRenderer&) { }
	IRenderer& operator = (const IRenderer&) { return *this; }
public:

	// Methods
	virtual void Render(Canvas& canvas) = 0;
};
