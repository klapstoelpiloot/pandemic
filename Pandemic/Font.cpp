#include "Font.h"
#include "File.h"
#include "Resources.h"

// Constructor
Font::Font(const String& filename, const Resources& resources) :
	image(),
	lineheight(0),
	linebase(0)
{
	// Read the font file as text and split into lines
	String fontinfo = File::ReadAsText(filename);
	vector<String> lines;
	fontinfo.Split(lines, '\n');

	// Parse general font information
	char imagefilename[256];
	int scancount = std::sscanf(lines[1], "common lineHeight=%i base=%i", &lineheight, &linebase);
	ENSURE(scancount == 2);
	scancount = std::sscanf(lines[2], "page id=0 file=%s255", &imagefilename);
	ENSURE(scancount == 1);

	// Find the image
	// We do not take ownership of the image. The Resources library stays owner.
	String cleanimagefilename = imagefilename;
	cleanimagefilename.Replace("\"", "");
	image = &(resources.GetImage(cleanimagefilename));

	// Parse character information
	int numchars, ln = 3;
	scancount = std::sscanf(lines[ln++], "chars count=%i", &numchars);
	ENSURE(scancount == 1);
	for(int i = 0; i < numchars; i++)
	{
		FontChar fc;
		uint id;
		scancount = std::sscanf(lines[ln++], "char id=%i x=%i y=%i width=%i height=%i xoffset=%i yoffset=%i xadvance=%i",
			&id, &fc.imgrect.x, &fc.imgrect.y, &fc.imgrect.width, &fc.imgrect.height, &fc.offset.x, &fc.offset.y, &fc.advance);
		ENSURE(scancount == 8);
		charmap.insert(FontCharPair(id, fc));
	}

	// Parse kerning information, if available
	if(lines.size() > static_cast<size_t>(ln))
	{
		int numkernings;
		scancount = std::sscanf(lines[ln++], "kernings count=%i", &numkernings);
		if(scancount == 1)
		{
			for(int i = 0; i < numkernings; i++)
			{
				KerningChars chars;
				int amount;
				scancount = std::sscanf(lines[ln++], "kerning first=%i second=%i amount=%i", &chars.first, &chars.second, &amount);
				ENSURE(scancount == 3);
				kernmap.insert(KerningPair(chars, amount));
			}
		}
	}
}

// Destructor
Font::~Font()
{
	image = nullptr;
}

