#include "Configuration.h"
#include "File.h"

// Constructor
Configuration::Configuration()
{
	// Load pandemic.toml from the application directory
	String procdir = File::GetCurrentProcessDir();
	String configfilename = File::CombinePath(procdir, "pandemic.toml");
	std::cout << "Loading configuration " << configfilename << "..." << std::endl;
	toml.LoadFile(configfilename);

	// Read general settings
	datapath = toml.GetString("General.DataPath", procdir);
}
