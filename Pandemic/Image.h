#pragma once
#include "Tools.h"
#include "Color.h"
#include "Size.h"

class Image
{
private:

	// Fields
	bool hascolors;
	int width;
	int height;
	byte* data;

public:

	Image();
	Image(const String& filename);
	virtual ~Image();

	// Loading
	void Load(const String& filename);
	void Unload();

	// Properties
	inline bool HasColors() const { return hascolors; }
	inline int Width() const { return width; }
	inline int Height() const { return height; }
	inline Size GetSize() const { return Size(width, height); }
	inline const byte* ByteData() const { return data; }
	inline const Color* ColorData() const
	{
		#ifdef DEBUG
			REQUIRE(hascolors);
		#endif
		return reinterpret_cast<Color*>(data);
	}
	inline byte GetByte(int x, int y) const { return *(data + y * width + x); }
	inline Color GetColor(int x, int y) const
	{
		#ifdef DEBUG
			REQUIRE(hascolors);
		#endif
		return *(reinterpret_cast<Color*>(data) + y * width + x);
	}
};
