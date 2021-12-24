#include <unistd.h>
#include <cstring>
#include <stdio.h>
#include <signal.h>
#include <math.h>
#include <chrono>
#include <thread>
#include "Main.h"
#include "InputHandler.h"
#include "Text.h"
#include "File.h"
#include "GameStateMachine.h"
#include "MenuStateMachine.h"

// ccxopts raises some warnings (and rightfully so) about integer conversion
// but instead of messing with the source code I chose to silence these warnings.
// TODO: Update this source code when the author has a proper fix.
// https://github.com/jarro2783/cxxopts
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
#include "cxxopts.hpp"
#pragma GCC diagnostic pop

// This lists the available options on the command line and parses the given options.
// Using the ParseResult we can easily determine what options were specified.
cxxopts::ParseResult ParseCommandLineOptions(int argc, char* argv[])
{
	try
	{
		// List the available options
		cxxopts::Options options("Pandemic", "Pandemic command line options");
		options
			.add_options()
			("help", "Shows information about the command line options.")
			("record", "Record all display frames to the configured directory.")
			("showfps", "Outputs the frames per second to standard output.")
			;

			// Parse the arguments with these options
		cxxopts::ParseResult cmdargs = options.parse(argc, argv);

		// If the user is just asking for help,
		// output the available command line options...
		if(cmdargs.count("help"))
		{
			std::cout << options.help() << std::endl;
			exit(0);
		}

		return cmdargs;
	}
	catch(const cxxopts::OptionException& e)
	{
		std::cout << "Error parsing options: " << e.what() << std::endl;
		exit(1);
	}
}

// This handles the exit request
volatile bool exitsignal = false;
static void SignalInterruptHandler(int signo)
{
	exitsignal = true;
}

// Global instance
Main* main_instance = nullptr;

// Constructor
Main::Main(const cxxopts::ParseResult& cmdargs) :
	config(),
	io(config),
	graphics(config, (cmdargs.count("showfps") > 0)),
	audio(config),
	scores(config),
	buttons(),
	resources(nullptr),
	menu(nullptr),
	game(nullptr)
{
	if(cmdargs.count("record") > 0)
	{
		// Make a directory for recording
		String recordpath;
		int dirindex = 1;
		do
		{
			recordpath = File::CombinePath(config.GetString("General.RecordingPath", ""), String::From(dirindex++));
		}
		while(File::DirectoryExists(recordpath));
		std::cout << "Recording to " << recordpath << std::endl;
		File::CreateDirectory(recordpath);

		// Begin recording
		graphics.Record(recordpath);
		audio.Record(recordpath);
	}
}

// Destructor
Main::~Main()
{
	SAFE_DELETE(game);
	SAFE_DELETE(menu);
	SAFE_DELETE(resources);
}

// Initialization of dynamic parts
void Main::Setup()
{
	// Load resources
	REQUIRE(resources == nullptr);
	resources = new Resources();

	// Make the statemachines
	menu = new MenuStateMachine();
	game = new GameStateMachine();

	// Set the first state
	game->ChangeState(game->GetTitleState());
}

// Program loop updates
void Main::Update()
{
	// Receive and process messages from the IO module
	IOModule_IOMessage msg;
	while(io.ReceiveMessage(msg))
	{
		bool handled = menu->HandleMessage(msg);
		if(!handled)
			game->HandleMessage(msg);
	}

	// Update the state(machines)
	menu->Update();
	game->Update();

	// Render the graphics
	graphics.Present();
}

// Program entrance point
int main(int argc, char* argv[])
{
	// Parse command line options
	cxxopts::ParseResult cmdargs = ParseCommandLineOptions(argc, argv);

	std::cout << "Main startup..." << std::endl;

	// Set up the signal handlers
	signal(SIGTERM, SignalInterruptHandler);
	signal(SIGINT, SignalInterruptHandler);

	// Set a name for the main thread.
	// We like thread names, because it makes debugging easier.
	pthread_setname_np(pthread_self(), "Pandemic");

	// This will handle keyboard input on std in
	InputHandler inputhandler(true);
	inputhandler.Start();

	// Load the game
	main_instance = new Main(cmdargs);
	main_instance->Setup();
	std::cout << "Initialization complete, begin game." << std::endl;

	// Program loop
	while(!exitsignal && !inputhandler.IsExitRequested())
		main_instance->Update();

	// Clean up
	SAFE_DELETE(main_instance);
	inputhandler.Stop();

	std::cout << "Goodbye!" << std::endl;
	return 0;
}
