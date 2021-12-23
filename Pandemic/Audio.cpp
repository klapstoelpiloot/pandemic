#include <string.h>
#include "Audio.h"
#include "Tools.h"
#include "File.h"
#include <fmod.hpp>
#include <fmod_errors.h>

// For recording only
FILE* recordfile;
FMOD_RESULT F_CALLBACK RecordDspRead(FMOD_DSP_STATE* dsp_state, float* inbuffer, float* outbuffer, unsigned int length, int inchannels, int* outchannels);

Audio::Audio(const Configuration& cfg) :
	system(nullptr),
	dsprecord(nullptr),
	mastergroup(nullptr),
	stopthread(false)
{
	recordfile = nullptr;

	FMOD_RESULT result;
	FMOD_OUTPUTTYPE mixer = static_cast<FMOD_OUTPUTTYPE>(cfg.GetInt("Audio.Mixer", FMOD_OUTPUTTYPE_AUTODETECT));
	frequency = cfg.GetInt("Audio.Frequency", 0);
	int buffers = cfg.GetInt("Audio.Buffers", 4);
	int buffersize = cfg.GetInt("Audio.BufferSize", 1024);
	volume = cfg.GetInt("Audio.Volume", 100);
	int maxchannels = cfg.GetInt("Audio.MaxChannels", 512);

	// Create fmod
	std::cout << "Initializing audio..." << std::endl;
	result = FMOD::System_Create(&system);
	ENSURE(result == FMOD_OK);
	ENSURE(system != nullptr);
	result = system->setOutput(mixer);
	ENSURE(result == FMOD_OK);
	if(frequency > 0)
	{
		result = system->setSoftwareFormat(frequency, FMOD_SPEAKERMODE_STEREO, 2);
		ENSURE(result == FMOD_OK);
	}
	else
	{
		int driverindex;
		char drivername[256];
		FMOD_GUID driverguid;
		FMOD_SPEAKERMODE driverspeakers;
		int channels;
		result = system->getDriver(&driverindex);
		ENSURE(result == FMOD_OK);
		result = system->getDriverInfo(driverindex, drivername, 255, &driverguid, &frequency, &driverspeakers, &channels);
		ENSURE(result == FMOD_OK);
	}
	result = system->setDSPBufferSize(buffersize, buffers);
	ENSURE(result == FMOD_OK);
	result = system->init(maxchannels, FMOD_INIT_NORMAL, nullptr);
	ENSURE(result == FMOD_OK);

	// Create a DSP for recording (bypass it until we decide to record)
	FMOD_DSP_DESCRIPTION dspdesc;
	memset(&dspdesc, 0, sizeof(dspdesc));
	FMOD_DSP_PARAMETER_DESC* paramdesc[0] =	{ };
	strncpy(dspdesc.name, "Record To Disk", sizeof(dspdesc.name));
	dspdesc.version = 0x00010000;
	dspdesc.numinputbuffers = 1;
	dspdesc.numoutputbuffers = 1;
	dspdesc.read = RecordDspRead;
	dspdesc.create = nullptr;
	dspdesc.release = nullptr;
	dspdesc.getparameterdata = nullptr;
	dspdesc.setparameterfloat = nullptr;
	dspdesc.getparameterfloat = nullptr;
	dspdesc.numparameters = 0;
	dspdesc.paramdesc = paramdesc;
	result = system->createDSP(&dspdesc, &dsprecord);
	ENSURE(result == FMOD_OK);
	result = system->getMasterChannelGroup(&mastergroup);
	ENSURE(result == FMOD_OK);
	result = mastergroup->addDSP(0, dsprecord);
	ENSURE(result == FMOD_OK);
	result = dsprecord->setBypass(true);
	ENSURE(result == FMOD_OK);

	// Apply global volume
	SetVolume(volume);

	// Start the audio processing thread
	thread = std::thread(&Audio::Thread, this);
	pthread_setname_np(thread.native_handle(), "Audio");
}

Audio::~Audio()
{
	// Shutdown processing thread
	if(thread.joinable())
	{
		stopthread = true;
		thread.join();
	}
	
	// Clean up
	mastergroup->removeDSP(dsprecord);
	dsprecord->release();
	dsprecord = nullptr;
	system->close();
	system->release();
	system = nullptr;
	mastergroup = nullptr;
	if(recordfile != nullptr)
	{
		fclose(recordfile);
		recordfile = nullptr;
	}
}

void Audio::SetVolume(int v)
{
	// Set global volume
	volume = std::clamp(v, 0, 100);
	FMOD::ChannelGroup* mastergroup;
	FMOD_RESULT result = system->getMasterChannelGroup(&mastergroup);
	ENSURE(result == FMOD_OK);
	ENSURE(mastergroup != nullptr);
	result = mastergroup->setVolume(static_cast<float>(volume) / 100.0f);
	ENSURE(result == FMOD_OK);
}

void Audio::Record(String path)
{
	String filename = File::CombinePath(path, "Float32_Stereo_" + String::From(frequency) + "Hz.raw");
	recordfile = fopen(filename, "wb");
	dsprecord->setBypass(false);
}

// Audio processing thread
void Audio::Thread()
{
	while(!stopthread)
	{
		system->update();

		std::this_thread::sleep_for(ch::milliseconds(10));
	}
}

// Records audio to a file on disk
FMOD_RESULT F_CALLBACK RecordDspRead(FMOD_DSP_STATE* dsp_state, float* inbuffer, float* outbuffer, unsigned int length, int inchannels, int* outchannels)
{
	REQUIRE((inchannels == 1) || (inchannels == 2));
	REQUIRE((*outchannels == 1) || (*outchannels == 2));

	for(unsigned int s = 0; s < length; s++)
	{
		// Get samples
		float f1 = inbuffer[s * inchannels + 0];
		float f2 = (inchannels == 1) ? f1 : inbuffer[s * inchannels + 1];

		// Pass through
		outbuffer[s * *outchannels + 0] = f1;
		if(*outchannels == 2)
			outbuffer[s * *outchannels + 1] = f2;

		// Output to file
		if(recordfile != nullptr)
		{
			fwrite(&f1, sizeof(float), 1, recordfile);
			fwrite(&f2, sizeof(float), 1, recordfile);
		}
	}

	return FMOD_OK;
}
