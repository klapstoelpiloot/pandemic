#pragma once
#include "IRenderer.h"

class IPositionalRenderer : virtual public IRenderer
{
	// Make this an interface, do not allow instantiation
public:
	virtual ~IPositionalRenderer() = default;
protected:
	IPositionalRenderer() { }
	IPositionalRenderer(const IPositionalRenderer&) { }
	IPositionalRenderer& operator = (const IPositionalRenderer&) { return *this; }
public:

	// Methods
	virtual void SetPosition(Point pos) = 0;
};
