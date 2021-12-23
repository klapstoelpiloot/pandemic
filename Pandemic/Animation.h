#pragma once
#include "Image.h"
#include "FrameData.h"

class Resources;

class Animation
{
private:

	// Image which contains all frames
	const Image* image;

	// Size of a single frame in the image
	Size framesize;

	// Frame information
	vector<FrameData> frames;

	// Total duration in milliseconds
	uint duration;

public:

	// Constructor
	Animation(const String& filename, const Resources& resources);
	Animation(const Image& image);
	~Animation();

	// Access
	inline const Image& GetImage() const { return *image; }
	inline Size GetFrameSize() const { return framesize; }
	inline const vector<FrameData>& GetFrames() const { return frames; }
	inline uint GetDuration() const { return duration; }
};

