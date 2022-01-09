#include "Resources.h"
#include "Main.h"
#include "File.h"

Resources::Resources()
{
	Main::GetGraphics().ClearRenderers();
	// The loading is so short, we don't really need this
	//Main::GetGraphics().AddRenderer(&loadingscreen);
	//Main::GetGraphics().Present();

	std::cout << "Loading resources... " << std::endl;
	
	// Find all files in the data path
	vector<String> allfiles;
	const Configuration& config = Main::GetConfig();
	String datapath = config.DataPath();
	bool result = File::GetFiles(datapath, allfiles);
	ENSURE(result);

	// Make lists of the files we want to load.
	// We use these lists to load resources by type.
	vector<String> ddsfiles, fntfiles, wavfiles, mp3files, anifiles;
	CopyFilenamesByExtension(allfiles, ddsfiles, ".dds");
	CopyFilenamesByExtension(allfiles, fntfiles, ".fnt");
	CopyFilenamesByExtension(allfiles, wavfiles, ".wav");
	CopyFilenamesByExtension(allfiles, mp3files, ".mp3");
	CopyFilenamesByExtension(allfiles, anifiles, ".ani");

	// Load music
	for(const String& filename : mp3files)
	{
		String name = File::GetFileName(filename);
		int vol = config.GetInt(String("Resources.") + name + ".Volume", 100);
		Sound* mus = new Sound(filename, true, static_cast<float>(vol) / 100.0f);
		music.insert(MusicMap::value_type(name, mus));
	}

	// Load images
	for(const String& filename : ddsfiles)
	{
		String name = File::GetFileName(filename);
		Image* img = new Image(filename);
		images.insert(ImagesMap::value_type(name, img));
	}

	// Load sounds
	for(const String& filename : wavfiles)
	{
		String name = File::GetFileName(filename);
		int vol = config.GetInt(String("Resources.") + name + ".Volume", 100);
		Sound* snd = new Sound(filename, false, static_cast<float>(vol) / 100.0f);
		sounds.insert(SoundsMap::value_type(name, snd));
	}

	// Load fonts
	for(const String& filename : fntfiles)
	{
		String name = File::GetFileName(filename);
		Font* fnt = new Font(filename, *this);
		fonts.insert(FontsMap::value_type(name, fnt));
	}

	// Load animations
	for(const String& filename : anifiles)
	{
		String name = File::GetFileName(filename);
		Animation* fnt = new Animation(filename, *this);
		animations.insert(AnimationsMap::value_type(name, fnt));
	}

	// Make shortcuts for common resources
	boldbits = &GetFont("boldbits.fnt");
	boldbitslarge = &GetFont("boldbitslarge.fnt");
	smallest = &GetFont("smallest.fnt");
}

Resources::~Resources()
{
	// Unload sounds
	for(MusicMap::value_type& mus : music)
		delete mus.second;
	music.clear();

	// Unload sounds
	for(SoundsMap::value_type& snd : sounds)
		delete snd.second;
	sounds.clear();

	// Unload fonts
	for(FontsMap::value_type& fnt : fonts)
		delete fnt.second;
	fonts.clear();

	// Unload images
	for(ImagesMap::value_type& img : images)
		delete img.second;
	images.clear();

}

const Image& Resources::GetImage(const char* filename) const
{
	auto result = images.find(std::string(filename));
	if(result == images.cend())
	{
		std::cout << "Image with filename '" << filename << "' was not found." << std::endl;
		FAIL("File not found");
	}
	return *(result->second);
}

const Font& Resources::GetFont(const char* filename) const
{
	auto result = fonts.find(std::string(filename));
	if(result == fonts.cend())
	{
		std::cout << "Font with filename '" << filename << "' was not found." << std::endl;
		FAIL("File not found");
	}
	return *(result->second);
}

const Sound& Resources::GetSound(const char* filename) const
{
	auto result = sounds.find(std::string(filename));
	if(result == sounds.cend())
	{
		std::cout << "Sound with filename '" << filename << "' was not found." << std::endl;
		FAIL("File not found");
	}
	return *(result->second);
}

const Sound& Resources::GetMusic(const char* filename) const
{
	auto result = music.find(std::string(filename));
	if(result == music.cend())
	{
		std::cout << "Music with filename '" << filename << "' was not found." << std::endl;
		FAIL("File not found");
	}
	return *(result->second);
}

const Animation& Resources::GetAni(const char* filename) const
{
	auto result = animations.find(std::string(filename));
	if(result == animations.cend())
	{
		std::cout << "Animation with filename '" << filename << "' was not found." << std::endl;
		FAIL("File not found");
	}
	return *(result->second);
}

// This copies all filenames from input to output which match the specified extension
void Resources::CopyFilenamesByExtension(vector<String>& input, vector<String>& output, const char* extension)
{
	String extlowercase = String::ToLower(extension);
	for(const String& filename : input)
	{
		if(String::ToLower(File::GetExtension(filename)) == extlowercase)
			output.push_back(filename);
	}
}
