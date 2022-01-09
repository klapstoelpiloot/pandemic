#pragma once
#include "Font.h"
#include "Canvas.h"

struct TextChar
{
	Point position;		// Position of the left-top on the target
	Rect imgrect;		// Character position in source image
};

enum class HorizontalAlign
{
	Left,
	Right,
	Center
};

enum class VerticalAlign
{
	Top,
	Bottom,
	Middle
};

class Text
{
private:

	// The text to render
	String text;

	// The font used to generate this text
	const Font* font;

	// Alignment of the text
	HorizontalAlign alignh;
	VerticalAlign alignv;

	// The array of characters we should be drawing
	vector<TextChar> chars;
	int charspacing;
	Size textsize;
	Size offsetadjust;

	// This updates the arrangement of the characters for drawing
	void Update();

public:

	// Constructor/destructor
	Text();
	Text(const Font& font, HorizontalAlign horizontalalign, VerticalAlign verticalalign, int charspacing = 0);
	Text(const String& text, const Font& font, HorizontalAlign horizontalalign, VerticalAlign verticalalign, int charspacing = 0);
	virtual ~Text();

	// Properties
	inline void SetFont(const Font& font) { this->font = &font; Update(); }
	inline void SetHorizontalAlign(HorizontalAlign horizontalalign) { alignh = horizontalalign; Update(); }
	inline void SetVerticalAlign(VerticalAlign verticalalign) { alignv = verticalalign; Update(); }
	inline const String& GetText() const { return text; }
	inline void SetText(const String& text) { this->text = text; Update(); }
	inline const Size& GetTextSize() const { return textsize; }
	inline Rect GetTextRect(Point pos) const { return Rect(pos.Offset(offsetadjust), textsize); }
	inline void SetCharSpacing(int pixels) { this->charspacing = pixels; Update(); }

	// Drawing
	void DrawOpaque(Canvas& canvas, Point pos, Color c) const;
	void DrawBlend(Canvas& canvas, Point pos, Color c) const;
	void DrawAdd(Canvas& canvas, Point pos, Color c) const;
	void DrawMask(Canvas& canvas, Point pos, Color c) const;
	void DrawTexturedOpaque(Canvas& canvas, Point pos, const IImage& tex, Point texoffset = Point()) const;
	void DrawTexturedMask(Canvas& canvas, Point pos, const IImage& tex, Point texoffset = Point()) const;
	void DrawTexturedBlend(Canvas& canvas, Point pos, const IImage& tex, Point texoffset = Point()) const;
	void DrawTexturedAdd(Canvas& canvas, Point pos, const IImage& tex, Point texoffset = Point()) const;
	void DrawTexturedModOpaque(Canvas& canvas, Point pos, const IImage& tex, Color mod, Point texoffset = Point()) const;
	void DrawTexturedModMask(Canvas& canvas, Point pos, const IImage& tex, Color mod, Point texoffset = Point()) const;
	void DrawTexturedModBlend(Canvas& canvas, Point pos, const IImage& tex, Color mod, Point texoffset = Point()) const;
	void DrawTexturedModAdd(Canvas& canvas, Point pos, const IImage& tex, Color mod, Point texoffset = Point()) const;

	// More specialized drawing
	void DrawShadowMask(Canvas& canvas, Point pos, int distance, Color c) const;
	void DrawOutlineMask(Canvas& canvas, Point pos, int distance, Color c) const;
	void DrawOutlineBlend(Canvas& canvas, Point pos, int distance, Color c) const;
};
