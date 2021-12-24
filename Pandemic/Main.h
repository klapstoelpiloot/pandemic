#pragma once
#include "Graphics.h"
#include "Configuration.h"
#include "Audio.h"
#include "IOModuleLink.h"
#include "Resources.h"
#include "HighscoreManager.h"
#include "ButtonLEDsController.h"

class Main;
class GameStateMachine;
class MenuStateMachine;

namespace cxxopts
{
	class ParseResult;
}

// Static instance
extern Main* main_instance;

class Main final
{
private:

	// Individual system components
	// These are statically initialized in this order
	Configuration config;
	IOModuleLink io;
	Graphics graphics;
	Audio audio;
	HighscoreManager scores;
	ButtonLEDsController buttons;

	// Resources
	// This is dynamically initialized
	Resources* resources;

	// Game and menu state machines
	MenuStateMachine* menu;
	GameStateMachine* game;

public:

	Main(const cxxopts::ParseResult& cmdargs);
	~Main();

	// Static access
	static inline Configuration& GetConfig() { return main_instance->config; }
	static inline Graphics& GetGraphics() { return main_instance->graphics; }
	static inline Audio& GetAudio() { return main_instance->audio; }
	static inline IOModuleLink& GetIO() { return main_instance->io; }
	static inline HighscoreManager& GetScores() { return main_instance->scores; }
	static inline GameStateMachine& GetGame() { return *(main_instance->game); }
	static inline MenuStateMachine& GetMenu() { return *(main_instance->menu); }
	static inline Resources& GetResources() { return *(main_instance->resources); }
	static inline ButtonLEDsController& GetButtons() { return main_instance->buttons; }

	// Initialization of dynamic parts
	void Setup();

	// Program loop updates
	void Update();
};
