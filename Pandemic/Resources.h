#pragma once
#include <unordered_map>
#include "Image.h"
#include "Font.h"
#include "Sound.h"
#include "Animation.h"
#include "ProgressBar.h"

typedef std::unordered_map<std::string, Image*> ImagesMap;
typedef std::unordered_map<std::string, Font*> FontsMap;
typedef std::unordered_map<std::string, Sound*> SoundsMap;
typedef std::unordered_map<std::string, Sound*> MusicMap;
typedef std::unordered_map<std::string, Animation*> AnimationsMap;

class Resources final
{
private:

	// We use this to display our progress
	ProgressBar bar;

	// Resources
	ImagesMap images;
	FontsMap fonts;
	SoundsMap sounds;
	MusicMap music;
	AnimationsMap animations;

	// Common resource shortcuts
	const Font* boldbits;
	const Font* boldbitslarge;
	const Font* smallest;

	// Methods
	void CopyFilenamesByExtension(vector<String>& input, vector<String>& output, const char* extension);
	void UpdateProgress(ProgressBar& bar, uint totalfiles, uint& filesdone);

public:

	Resources();
	~Resources();

	// Access
	ProgressBar* GetProgressBar() { return &bar; }
	const Image& GetImage(const char* filename) const;
	const Font& GetFont(const char* filename) const;
	const Sound& GetSound(const char* filename) const;
	const Sound& GetMusic(const char* filename) const;
	const Animation& GetAni(const char* filename) const;

	// Common resources
	const Font& BoldBits() { return *boldbits; }
	const Font& BoldBitsLarge() { return *boldbitslarge; }
	const Font& Smallest() { return *smallest; }
};

