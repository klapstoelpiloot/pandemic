#pragma once
#include "Canvas.h"

class IGraphicsHAL
{
	// Make this an interface, do not allow instantiation
public:
	virtual ~IGraphicsHAL() = default;
protected:
	IGraphicsHAL() { }
	IGraphicsHAL(const IGraphicsHAL&) { }
	IGraphicsHAL& operator = (const IGraphicsHAL&) { return *this; }
public:

	// Methods
	virtual void Present(Canvas& sourcecanvas) = 0;
	virtual void SetBrightness(int b) = 0;
	virtual int GetBrightness() const = 0;
};
