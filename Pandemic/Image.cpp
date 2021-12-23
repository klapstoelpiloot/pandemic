#include "Image.h"
#include "Tools.h"
#include "DDS.h"

// Constructor
Image::Image() :
	hascolors(false),
	width(0),
	height(0),
	data(nullptr)
{
}

// Constructor
Image::Image(const String& filename) : Image()
{
	Load(filename);
}

// Destructor
Image::~Image()
{
	Unload();
}

void Image::Unload()
{
	width = 0;
	height = 0;
	hascolors = false;
	SAFE_FREE(data);
}

// Load image from DDS file
void Image::Load(const String& filename)
{
	Unload();

	// The formats we can deal with
	static const dds_u32 SUPPORTED_FORMATS[] = { DDS_FMT_R8G8B8A8, DDS_FMT_B8G8R8A8, DDS_FMT_B8G8R8X8, DDS_FMT_M8, 0 };

	// Load the DDS header
	dds_info ddsinfo;
	int result = dds_load_from_file(filename, &ddsinfo, SUPPORTED_FORMATS);
	ENSURE(result == DDS_SUCCESS);
	ASSERT(ddsinfo.image.depth <= 1, "Volume images are not supported");
	ASSERT(ddsinfo.mipcount <= 1, "Mipmaps are not supported");
	ASSERT((ddsinfo.flags & DDS_CUBEMAP_FULL) == 0, "Cubemaps are not supported");
	
	// Load the image data
	width = ddsinfo.image.width;
	height = ddsinfo.image.height;
	hascolors = (ddsinfo.image.format != DDS_FMT_M8);
	data = dds_read_all(&ddsinfo);

	// Check if we need to swap color components
	if((ddsinfo.image.format == DDS_FMT_B8G8R8A8) || (ddsinfo.image.format == DDS_FMT_B8G8R8X8))
	{
		// Swap red and blue
		Color* p = reinterpret_cast<Color*>(data);
		for(int i = 0; i < (width * height); i++)
		{
			std::swap(p->r, p->b);
			p++;
		}
	}
}
