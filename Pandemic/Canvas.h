#pragma once
#include "GraphicsConstants.h"
#include "Image.h"
#include "Rect.h"

class Canvas final : public virtual IImage
{
private:

	// The buffer to which we draw
	Color renderbuffer[DISPLAY_WIDTH * DISPLAY_HEIGHT];

	// Helper method to prepare for image drawing. This clips input coordinates, modifies the image rect and
	// determines the drawing rect. Returns false when the image is completely outside the display,
	// otherwise returns true.
	bool PrepareImageDraw(Point pos, const IImage& img, Rect& imgrect, Rect& drawrect);

public:

	// Constructor/destructor
	Canvas();
	~Canvas();

	// Direct buffer access
	inline const Color* GetBuffer() const { return renderbuffer; }

	// IImage implementation
	virtual bool HasColors() const override final { return true; }
	virtual int Width() const override final { return DISPLAY_WIDTH; }
	virtual int Height() const override final { return DISPLAY_HEIGHT; }
	virtual Size GetSize() const override final { return Size(DISPLAY_WIDTH, DISPLAY_HEIGHT); }
	virtual const byte* ByteData() const override final { return reinterpret_cast<const byte*>(renderbuffer); }
	virtual const Color* ColorData() const override final { return renderbuffer; }
	virtual MonoSampler GetMonoSampler() const override final { NOT_SUPPORTED; return MonoSampler(nullptr, 0); }
	virtual ColorSampler GetColorSampler() const override final { return ColorSampler(renderbuffer, DISPLAY_WIDTH); }

	// Rasterizing methods
	void Clear(Color color);
	void CopyTo(Canvas& canvas) const { memcpy(canvas.renderbuffer, renderbuffer, sizeof(renderbuffer)); }
	void WriteToFile(String filename) const;
	inline void SetPixel(int x, int y, Color c) { renderbuffer[y * DISPLAY_WIDTH + x] = c; }
	inline Color GetPixel(int x, int y) const { return renderbuffer[y * DISPLAY_WIDTH + x]; }
	inline void BlendPixel(int x, int y, Color c) { renderbuffer[y * DISPLAY_WIDTH + x].Blend(c); }
	inline void AddPixel(int x, int y, Color c) { renderbuffer[y * DISPLAY_WIDTH + x].Add(c); }
	inline void MaskPixel(int x, int y, Color c) { renderbuffer[y * DISPLAY_WIDTH + x].Mask(c); }
	void DrawRectangle(Point p1, Point p2, Color linecolor, Color fillcolor);
	void DrawRectangleBlend(Point p1, Point p2, Color linecolor, Color fillcolor);
	void DrawColorImage(Point pos, const IImage& img) { DrawColorImage(pos, img, Rect(Point(0, 0), img.GetSize())); }
	void DrawColorImage(Point pos, const IImage& img, Rect imgrect);
	void DrawColorImageBlend(Point pos, const IImage& img) { DrawColorImageBlend(pos, img, Rect(Point(0, 0), img.GetSize())); }
	void DrawColorImageBlend(Point pos, const IImage& img, Rect imgrect);
	void DrawColorImageAdd(Point pos, const IImage& img) { DrawColorImageAdd(pos, img, Rect(Point(0, 0), img.GetSize())); }
	void DrawColorImageAdd(Point pos, const IImage& img, Rect imgrect);
	void DrawColorImageMask(Point pos, const IImage& img) { DrawColorImageMask(pos, img, Rect(Point(0, 0), img.GetSize())); }
	void DrawColorImageMask(Point pos, const IImage& img, Rect imgrect);
	void DrawMonoImage(Point pos, const IImage& img, Color color) { DrawMonoImage(pos, img, color, Rect(Point(0, 0), img.GetSize())); }
	void DrawMonoImage(Point pos, const IImage& img, Color color, Rect imgrect);
	void DrawMonoImageBlend(Point pos, const IImage& img, Color color) { DrawMonoImageBlend(pos, img, color, Rect(Point(0, 0), img.GetSize())); }
	void DrawMonoImageBlend(Point pos, const IImage& img, Color color, Rect imgrect);
	void DrawMonoImageAdd(Point pos, const IImage& img, Color color) { DrawMonoImageAdd(pos, img, color, Rect(Point(0, 0), img.GetSize())); }
	void DrawMonoImageAdd(Point pos, const IImage& img, Color color, Rect imgrect);
	void DrawMonoImageMask(Point pos, const IImage& img, Color color) { DrawMonoImageMask(pos, img, color, Rect(Point(0, 0), img.GetSize())); }
	void DrawMonoImageMask(Point pos, const IImage& img, Color color, Rect imgrect);
	void DrawMonoTextured(Point pos, const IImage& img, const IImage& tex, Point texoffset = Point()) { DrawMonoTextured(pos, img, tex, texoffset, Rect(Point(0, 0), img.GetSize())); }
	void DrawMonoTextured(Point pos, const IImage& img, const IImage& tex, Point texoffset, Rect imgrect);
	void DrawMonoTexturedMask(Point pos, const IImage& img, const IImage& tex, Point texoffset = Point()) { DrawMonoTexturedMask(pos, img, tex, texoffset, Rect(Point(0, 0), img.GetSize())); }
	void DrawMonoTexturedMask(Point pos, const IImage& img, const IImage& tex, Point texoffset, Rect imgrect);
	void DrawMonoTexturedBlend(Point pos, const IImage& img, const IImage& tex, Point texoffset = Point()) { DrawMonoTexturedBlend(pos, img, tex, texoffset, Rect(Point(0, 0), img.GetSize())); }
	void DrawMonoTexturedBlend(Point pos, const IImage& img, const IImage& tex, Point texoffset, Rect imgrect);
	void DrawMonoTexturedAdd(Point pos, const IImage& img, const IImage& tex, Point texoffset = Point()) { DrawMonoTexturedAdd(pos, img, tex, texoffset, Rect(Point(0, 0), img.GetSize())); }
	void DrawMonoTexturedAdd(Point pos, const IImage& img, const IImage& tex, Point texoffset, Rect imgrect);
	void DrawMonoTexturedMod(Point pos, const IImage& img, const IImage& tex, Color mod, Point texoffset = Point()) { DrawMonoTexturedMod(pos, img, tex, mod, texoffset, Rect(Point(0, 0), img.GetSize())); }
	void DrawMonoTexturedMod(Point pos, const IImage& img, const IImage& tex, Color mod, Point texoffset, Rect imgrect);
	void DrawMonoTexturedModMask(Point pos, const IImage& img, const IImage& tex, Color mod, Point texoffset = Point()) { DrawMonoTexturedModMask(pos, img, tex, mod, texoffset, Rect(Point(0, 0), img.GetSize())); }
	void DrawMonoTexturedModMask(Point pos, const IImage& img, const IImage& tex, Color mod, Point texoffset, Rect imgrect);
	void DrawMonoTexturedModBlend(Point pos, const IImage& img, const IImage& tex, Color mod, Point texoffset = Point()) { DrawMonoTexturedModBlend(pos, img, tex, mod, texoffset, Rect(Point(0, 0), img.GetSize())); }
	void DrawMonoTexturedModBlend(Point pos, const IImage& img, const IImage& tex, Color mod, Point texoffset, Rect imgrect);
	void DrawMonoTexturedModAdd(Point pos, const IImage& img, const IImage& tex, Color mod, Point texoffset = Point()) { DrawMonoTexturedModAdd(pos, img, tex, mod, texoffset, Rect(Point(0, 0), img.GetSize())); }
	void DrawMonoTexturedModAdd(Point pos, const IImage& img, const IImage& tex, Color mod, Point texoffset, Rect imgrect);
};
