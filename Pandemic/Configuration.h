#pragma once
#include "Tools.h"
#include "TomlParser.h"

class Configuration
{
private:
	
	// Toml parser
	TomlParser toml;
	
	// General settings
	String datapath;

public:

	// Constructor
	Configuration();

	// General settings
	const String& DataPath() const { return datapath; }

	// Methods to access specific settings
	bool GetBool(const String& setting, bool defaultvalue) const { return toml.GetBool(setting, defaultvalue); }
	int GetInt(const String& setting, int defaultvalue) const { return toml.GetInt(setting, defaultvalue); }
	int64 GetLong(const String& setting, int64 defaultvalue) const { return toml.GetLong(setting, defaultvalue); }
	double GetDouble(const String& setting, double defaultvalue) const { return toml.GetDouble(setting, defaultvalue); }
	String GetString(const String& setting, const String& defaultvalue) const { return toml.GetString(setting, defaultvalue); }
	String GetString(const String& setting, const char* defaultvalue) const { return toml.GetString(setting, defaultvalue); }
};
