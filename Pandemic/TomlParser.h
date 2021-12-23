#pragma once
#include "Tools.h"
#include "cpptoml.h"

// Implements the TOML file format as a configuration source
class TomlParser
{
private:

	// This hold the loaded configuration
	std::shared_ptr<cpptoml::table> root;
	
	// Helper function to get the desired type
	template<typename T>
	bool GetValueAs(const String& setting, T& value) const;
	template<typename T>
	bool GetValueArrayAs(const String& setting, vector<T>& values) const;

	// Helper function to write a setting
	template<typename T>
	void SetValueAs(std::shared_ptr<cpptoml::table>& table, const String& key, T value);
	template<typename T>
	void SetValueArrayAs(std::shared_ptr<cpptoml::table>& table, const String& key, const vector<T>& values);

	// Helper function to write an array
	void SetRecursive(std::shared_ptr<cpptoml::table>& table, const String& key, std::shared_ptr<cpptoml::base>& values);

public:

	// Constructor/destructor
	TomlParser();
	TomlParser(std::shared_ptr<cpptoml::table>& table);
	virtual ~TomlParser();

	// Implementation
	void LoadFile(const String& filename);
	void LoadString(const String& data);
	void WriteFile(const String& filename) const;
	String WriteString() const;
	void SetRoot(std::shared_ptr<cpptoml::table>& table) { root = table; }
	std::shared_ptr<cpptoml::table> GetRoot() { return root; }
	bool Exists(const String& setting) const;
	vector<String> GetSettings(const String& section) const;
	int GetInt(const String& setting, int defaultvalue) const;
	int64 GetLong(const String& setting, int64 defaultvalue) const;
	double GetDouble(const String& setting, double defaultvalue) const;
	String GetString(const String& setting, const String& defaultvalue) const;
	String GetString(const String& setting, const char* defaultvalue) const;
	std::shared_ptr<cpptoml::array> GetArray(const String& setting) const;
	std::shared_ptr<cpptoml::table> GetTable(const String& path) const;
	std::shared_ptr<cpptoml::table_array> GetTableArray(const String& path) const;
	bool GetBool(const String& setting, bool defaultvalue) const;
	bool GetIntArray(const String& setting, vector<int>& array) const;
	bool GetLongArray(const String& setting, vector<int64>& array) const;
	bool GetDoubleArray(const String& setting, vector<double>& array) const;
	bool GetStringArray(const String& setting, vector<String>& array) const;
	bool GetBoolArray(const String& setting, vector<bool>& array) const;
	void SetInt(const String& setting, int value);
	void SetLong(const String& setting, int64 value);
	void SetDouble(const String& setting, double value);
	void SetString(const String& setting, const String& value);
	void SetString(const String& setting, const char* value);
	void SetBool(const String& setting, bool value);
	void SetArray(const String& setting, std::shared_ptr<cpptoml::array> values);
	void SetTable(const String& setting, std::shared_ptr<cpptoml::table> table);
	void SetTableArray(const String& setting, std::shared_ptr<cpptoml::table_array> tables);
	void SetIntArray(const String& setting, const vector<int>& values);
	void SetLongArray(const String& setting, const vector<int64>& values);
	void SetDoubleArray(const String& setting, const vector<double>& values);
	void SetStringArray(const String& setting, const vector<String>& values);
	void SetBoolArray(const String& setting, const vector<bool>& values);
};
