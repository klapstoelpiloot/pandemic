#pragma once
#include "GraphicsConstants.h"
#include "Image.h"
#include "Rect.h"

class Canvas final
{
private:

	// The buffer to which we draw
	Color renderbuffer[DISPLAY_WIDTH * DISPLAY_HEIGHT];

	// Helper method to prepare for image drawing. This clips input coordinates, modifies the image rect and
	// determines the drawing rect. Returns false when the image is completely outside the display,
	// otherwise returns true.
	bool PrepareImageDraw(Point pos, const Image& img, Rect& imgrect, Rect& drawrect);

public:

	// Constructor/destructor
	Canvas();
	~Canvas();

	// Direct buffer access
	inline const Color* GetBuffer() const { return renderbuffer; }

	// Rasterizing methods
	void Clear(Color color);
	void CopyTo(Canvas& canvas) const { memcpy(canvas.renderbuffer, renderbuffer, sizeof(renderbuffer)); }
	inline void SetPixel(int x, int y, Color c) { renderbuffer[y * DISPLAY_WIDTH + x] = c; }
	inline Color GetPixel(int x, int y) const { return renderbuffer[y * DISPLAY_WIDTH + x]; }
	inline void BlendPixel(int x, int y, Color c) { renderbuffer[y * DISPLAY_WIDTH + x].Blend(c); }
	inline void AddPixel(int x, int y, Color c) { renderbuffer[y * DISPLAY_WIDTH + x].Add(c); }
	inline void MaskPixel(int x, int y, Color c) { renderbuffer[y * DISPLAY_WIDTH + x].Mask(c); }
	void DrawRectangle(Point p1, Point p2, Color linecolor, Color fillcolor);
	void DrawRectangleBlend(Point p1, Point p2, Color linecolor, Color fillcolor);
	void DrawColorImage(Point pos, const Image& img) { DrawColorImage(pos, img, Rect(Point(0, 0), img.GetSize())); }
	void DrawColorImage(Point pos, const Image& img, Rect imgrect);
	void DrawColorImageBlend(Point pos, const Image& img) { DrawColorImageBlend(pos, img, Rect(Point(0, 0), img.GetSize())); }
	void DrawColorImageBlend(Point pos, const Image& img, Rect imgrect);
	void DrawColorImageAdd(Point pos, const Image& img) { DrawColorImageAdd(pos, img, Rect(Point(0, 0), img.GetSize())); }
	void DrawColorImageAdd(Point pos, const Image& img, Rect imgrect);
	void DrawColorImageMask(Point pos, const Image& img) { DrawColorImageMask(pos, img, Rect(Point(0, 0), img.GetSize())); }
	void DrawColorImageMask(Point pos, const Image& img, Rect imgrect);
	void DrawMonoImage(Point pos, const Image& img, Color color) { DrawMonoImage(pos, img, color, Rect(Point(0, 0), img.GetSize())); }
	void DrawMonoImage(Point pos, const Image& img, Color color, Rect imgrect);
	void DrawMonoImageBlend(Point pos, const Image& img, Color color) { DrawMonoImageBlend(pos, img, color, Rect(Point(0, 0), img.GetSize())); }
	void DrawMonoImageBlend(Point pos, const Image& img, Color color, Rect imgrect);
	void DrawMonoImageAdd(Point pos, const Image& img, Color color) { DrawMonoImageAdd(pos, img, color, Rect(Point(0, 0), img.GetSize())); }
	void DrawMonoImageAdd(Point pos, const Image& img, Color color, Rect imgrect);
	void DrawMonoImageMask(Point pos, const Image& img, Color color) { DrawMonoImageMask(pos, img, color, Rect(Point(0, 0), img.GetSize())); }
	void DrawMonoImageMask(Point pos, const Image& img, Color color, Rect imgrect);
	void DrawMonoTexturedMask(Point pos, const Image& img, const Image& tex, Point texoffset = Point()) { DrawMonoTexturedMask(pos, img, tex, texoffset, Rect(Point(0, 0), img.GetSize())); }
	void DrawMonoTexturedMask(Point pos, const Image& img, const Image& tex, Point texoffset, Rect imgrect);
	void DrawMonoTexturedAdd(Point pos, const Image& img, const Image& tex, Point texoffset = Point()) { DrawMonoTexturedAdd(pos, img, tex, texoffset, Rect(Point(0, 0), img.GetSize())); }
	void DrawMonoTexturedAdd(Point pos, const Image& img, const Image& tex, Point texoffset, Rect imgrect);
};
