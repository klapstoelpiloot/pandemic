#include <utility>
#include <cassert>
#include <math.h>
#include "Tools.h"
#include "X11Graphics.h"
#include "Configuration.h"

#define WINDOW_BORDER			10
#define DOT_SIZE				4
#define DOT_LESS_BRIGHTNESS		180
#define DOT_DARK_BRIGHTNESS		100

X11Graphics::X11Graphics(const Configuration& cfg) :
	display(nullptr),
	screen(0),
	window(0),
	img(nullptr),
	imgdata(nullptr)
{
	unsigned long black, white;

	std::cout << "Initializing X11 graphics..." << std::endl;

	// Start X11
	display = XOpenDisplay(nullptr);
	screen = DefaultScreen(display);
	black = BlackPixel(display, screen);
	white = WhitePixel(display, screen);

	// Setup window and GC
	window = XCreateSimpleWindow(display, DefaultRootWindow(display), 20, 20,
		DISPLAY_WIDTH * DOT_SIZE + WINDOW_BORDER * 2, DISPLAY_HEIGHT * DOT_SIZE + WINDOW_BORDER * 2, WINDOW_BORDER, white, black);
	XSetStandardProperties(display, window, "Pandemic", "HI!", None, NULL, 0, NULL);
	XSelectInput(display, window, ExposureMask|ButtonPressMask|KeyPressMask);
	gc = XCreateGC(display, window, 0, 0);
	XSetBackground(display, gc, white);
	XSetForeground(display, gc, black);
	XClearWindow(display, window);
	XMapRaised(display, window);

	// Setup image which we'll use to present
	imgdata = new char[DISPLAY_WIDTH * DOT_SIZE * DISPLAY_HEIGHT * DOT_SIZE * 32];
	img = XCreateImage(display, DefaultVisual(display, screen), 24, ZPixmap, 0, imgdata, DISPLAY_WIDTH * DOT_SIZE, DISPLAY_HEIGHT * DOT_SIZE, 32, 0);
	ENSURE(img != nullptr);
	XInitImage(img);
	for(int y = 0; y < DISPLAY_HEIGHT * DOT_SIZE; y++)
	{
		for(int x = 0; x < DISPLAY_WIDTH * DOT_SIZE; x++)
		{
			XPutPixel(img, x, y, 0);
		}
	}
}

X11Graphics::~X11Graphics()
{
	XDestroyImage(img);
	img = nullptr;
	imgdata = nullptr;
	XDestroyWindow(display, window);
	XFreeGC(display, gc);
	XCloseDisplay(display);
	display = nullptr;
}

void X11Graphics::Present(Canvas& sourcecanvas)
{
	XEvent event;
	bool event_received = XCheckMaskEvent(display, -1, &event);
	const Color* p = sourcecanvas.GetBuffer();
	for(int y = 0; y < DISPLAY_HEIGHT; y++)
	{
		for(int x = 0; x < DISPLAY_WIDTH; x++)
		{
			int ox = x * DOT_SIZE;
			int oy = y * DOT_SIZE;

			// Full bright color
			unsigned long x1 = p->b | (p->g << 8) | (p->r << 16);

			// Less bright color
			Color lessp = *p;
			lessp.ModulateRGB(DOT_LESS_BRIGHTNESS);
			unsigned long x2 = lessp.b | (lessp.g << 8) | (lessp.r << 16);

			// Half bright color
			Color darkp = *p;
			darkp.ModulateRGB(DOT_DARK_BRIGHTNESS);
			unsigned long x3 = darkp.b | (darkp.g << 8) | (darkp.r << 16);

			// Draw a 4x4 'dot' which looks like a LED from a dot matrix display
			XPutPixel(img, ox + 1, oy + 0, x2);
			XPutPixel(img, ox + 2, oy + 0, x2);
			XPutPixel(img, ox + 1, oy + 3, x2);
			XPutPixel(img, ox + 2, oy + 3, x2);
			XPutPixel(img, ox + 0, oy + 1, x2);
			XPutPixel(img, ox + 0, oy + 2, x2);
			XPutPixel(img, ox + 3, oy + 1, x2);
			XPutPixel(img, ox + 3, oy + 2, x2);
			XPutPixel(img, ox + 1, oy + 1, x1);
			XPutPixel(img, ox + 1, oy + 2, x1);
			XPutPixel(img, ox + 2, oy + 1, x1);
			XPutPixel(img, ox + 2, oy + 2, x1);
			XPutPixel(img, ox + 0, oy + 0, x3);
			XPutPixel(img, ox + 0, oy + 3, x3);
			XPutPixel(img, ox + 3, oy + 0, x3);
			XPutPixel(img, ox + 3, oy + 3, x3);
			p++;
		}
	}
	XPutImage(display, window, gc, img, 0, 0, WINDOW_BORDER, WINDOW_BORDER, DISPLAY_WIDTH * DOT_SIZE, DISPLAY_HEIGHT * DOT_SIZE);
}

void X11Graphics::SetBrightness(int b)
{
	// Not supported
}
