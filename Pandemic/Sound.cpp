#include <fmod.hpp>
#include <fmod_errors.h>
#include "Sound.h"
#include "Main.h"

Sound::Sound(const String& filename, bool compressed, float volume) :
	sound(nullptr),
	volume(volume),
	lastchannel(nullptr)
{
	FMOD_RESULT result;
	FMOD::System* system = Main::GetAudio().GetSystem();
	
	// Load a sound
	if(compressed)
		result = system->createSound(filename, FMOD_CREATECOMPRESSEDSAMPLE, nullptr, &sound);
	else
		result = system->createSound(filename, FMOD_DEFAULT, nullptr, &sound);
	ENSURE(result == FMOD_OK);
	ENSURE(sound != nullptr);
}

Sound::~Sound()
{
	sound->release();
	sound = nullptr;
}

bool Sound::IsPlaying() const
{
	if(lastchannel == nullptr)
		return false;

	bool playing;
	FMOD_RESULT result = lastchannel->isPlaying(&playing);
	return (result == FMOD_OK);
}

void Sound::Play(bool loop) const
{
	// Play a sound (makes a channel)
	FMOD::System* system = Main::GetAudio().GetSystem();
	FMOD_RESULT result = system->playSound(sound, nullptr, true, &lastchannel);
	ENSURE(result == FMOD_OK);
	ENSURE(lastchannel != nullptr);

	result = lastchannel->setVolume(volume);
	ENSURE(result == FMOD_OK);

	lastchannel->setPaused(false);

	if(loop)
	{
		result = lastchannel->setLoopCount(-1);
		ENSURE(result == FMOD_OK);
	}
}

void Sound::Stop() const
{
	if(lastchannel != nullptr)
	{
		lastchannel->stop();
		lastchannel = nullptr;
	}
}

void Sound::SetVolume(float volume)
{
	this->volume = volume;
	FMOD_RESULT result = lastchannel->setVolume(volume);
	ENSURE(result == FMOD_OK);
}
