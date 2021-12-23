#pragma once
#include "Tools.h"

namespace FMOD
{
	class Sound;
	class Channel;
};

class Sound final
{
private:

	FMOD::Sound* sound;
	float volume;
	mutable FMOD::Channel* lastchannel;

public:

	Sound(const String& filename, bool compressed, float volume);
	~Sound();

	bool IsPlaying() const;
	void Play(bool loop = false) const;
	void Stop() const;
	void SetVolume(float volume);
};
