#pragma once
#include <thread>
#include "Configuration.h"

namespace FMOD
{
	class System;
	class DSP;
	class ChannelGroup;
};

class Audio final
{
private:

	// General stuff
	std::thread thread;
	FMOD::System* system;
	FMOD::DSP* dsprecord;
	FMOD::ChannelGroup* mastergroup;
	volatile bool stopthread;
	int frequency;
	int volume;

	// Methods
	void Thread();

public:

	Audio(const Configuration& cfg);
	~Audio();

	// Methods
	FMOD::System* GetSystem() { return system; }
	int GetVolume() const { return volume; }
	void SetVolume(int v);
	void Record(String path);
};
