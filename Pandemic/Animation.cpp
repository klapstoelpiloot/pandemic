#include "Animation.h"
#include "Main.h"

Animation::Animation(const String& filename, const Resources& resources) :
	image(nullptr),
	duration(0)
{
	TomlParser toml;
	toml.LoadFile(filename);

	// Find the image
	String imagefile = toml.GetString("Ani.Image", "");
	image = &(resources.GetImage(imagefile));

	// Determine frames layout
	framesize.width = toml.GetInt("Ani.FrameWidth", 0);
	framesize.height = toml.GetInt("Ani.FrameHeight", 0);
	int framesx = image->Width() / framesize.width;
	int framesy = image->Height() / framesize.height;
	int defaultframetime = toml.GetInt("Ani.FrameTime", 0);

	// Create frame data for all frames in animation
	int startindex = toml.GetInt("Ani.StartIndex", 0);
	int endindex = toml.GetInt("Ani.EndIndex", (framesx * framesy) - 1);
	int direction = (startindex <= endindex) ? 1 : -1;
	for(int i = startindex; i != (endindex + direction); i += direction)
	{
		// Name of this frame in configuration
		String framename = "Frame";
		framename.Append(i);

		// Where in the image is this frame?
		int iy = i / framesx;
		int ix = i - (iy * framesx);
		Point imagepos(ix * framesize.width, iy * framesize.height);

		// Duration
		uint frametime = static_cast<uint>(toml.GetInt(framename + ".Time", defaultframetime));
		duration += frametime;

		// Add to list
		frames.push_back(FrameData(imagepos, frametime));
	}
}

Animation::Animation(const Image& image) :
	image(&image),
	duration(0)
{
	// Single frame animation from image
	framesize = image.GetSize();
	frames.push_back(FrameData(Point(0, 0), 1));
	duration = 1;
}

Animation::~Animation()
{
	image = nullptr;
}
