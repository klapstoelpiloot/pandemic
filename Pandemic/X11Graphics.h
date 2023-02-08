#pragma once
#include "IGraphicsHAL.h"
#include "Configuration.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

class X11Graphics final : public virtual IGraphicsHAL
{
private:

	// The display
	Display* display;
	int screen;

	// Window handle
	Window window;

	// Graphics context
	GC gc;

	// The image on which we will render
	XImage* img;
	char* imgdata;

public:

	X11Graphics(const Configuration& cfg);
	virtual ~X11Graphics();

	// Methods
	virtual void Present(Canvas& sourcecanvas) override final;
	virtual void SetBrightness(int b) override final;
	virtual int GetBrightness() const override final;
};
