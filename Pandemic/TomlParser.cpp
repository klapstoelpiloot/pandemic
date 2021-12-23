#include <ostream>
#include <fstream>
#include "TomlParser.h"
#include "cpptoml_tools.h"

// Constrcutor
TomlParser::TomlParser() :
	root(cpptoml::make_table())
{
}

TomlParser::TomlParser(std::shared_ptr<cpptoml::table>& table) :
	root(table)
{
}

// Destructor
TomlParser::~TomlParser()
{
}

// This loads a configuration from a file
void TomlParser::LoadFile(const String& filename)
{
	root = cpptoml::parse_file(filename.c_str());
}

// This loads a configuration from a string
void TomlParser::LoadString(const String& data)
{
	memstream datastream(reinterpret_cast<const unsigned char*>(data.c_str()), data.Length());
	cpptoml::parser parser(datastream);
	root = parser.parse();
}

// This checks if the specified setting exists in this configuration
bool TomlParser::Exists(const String& setting) const
{
	try
	{
		return root->contains_qualified(setting.c_str());
	}
	catch(std::exception& ex)
	{
		std::cout << "Error checking existance of setting '" << setting.c_str() << "'. " << TypeNameOf(ex) << ": " << ex.what();
		return false;
	}
}

// Templated method to get a value
template<typename T>
bool TomlParser::GetValueAs(const String& setting, T& value) const
{
	typedef typename toml_type_for<T>::type TTOML;

	try
	{
		std::shared_ptr<cpptoml::base> set;
		try
		{
			// Get the specified setting.
			set = root->get_qualified(setting.c_str());
		}
		// If the setting is not in the configuration, this throws out_of_range exception.
		catch(std::out_of_range& ex)
		{
			return false;
		}

		// Better safe than sorry
		if(set == nullptr)
			return false;

		// Check if the setting is a value type (as opposed to a table type)
		if(!set->is_value())
		{
			std::cout << "Error reading setting '" << setting.c_str() << "'. The setting must be of type '" << TypeNameOf(value) << "'.";
			return false;
		}
		
		// Check if the value can be cast to the desired type
		std::shared_ptr<cpptoml::value<TTOML>> val = set->as<TTOML>();
		if(val == nullptr)
		{
			std::cout << "Error reading setting '" << setting.c_str() << "'. The setting must be of type '" << TypeNameOf(value) << "'.";
			return false;
		}

		// Get the setting value
		cpptoml::option<TTOML> result = cpptoml::get_impl<TTOML>(set);
		if(result)
			value = static_cast<T>(*result);
		return static_cast<bool>(result);
	}
	catch(std::exception& ex)
	{
		std::cout << "Error reading setting '" << setting.c_str() << "'. " << TypeNameOf(ex) << ": " << ex.what();
		return false;
	}
}

// Templated method to get a value array
template<typename T>
bool TomlParser::GetValueArrayAs(const String& setting, vector<T>& values) const
{
	typedef typename toml_type_for<T>::type TTOML;

	try
	{
		std::shared_ptr<cpptoml::base> set;
		try
		{
			// Get the specified setting.
			set = root->get_qualified(setting.c_str());
		}
		// If the setting is not in the configuration, this throws out_of_range exception.
		catch(std::out_of_range& ex)
		{
			return false;
		}

		// Better safe than sorry
		if(set == nullptr)
			return false;

		// Check if the setting is an array
		if(!set->is_array())
		{
			std::cout << "Error reading setting '" << setting.c_str() << "'. The setting must be an array of type '" << TypeNameOf<T>() << "'.";
			return false;
		}

		// Get the vector of elements
		cpptoml::option<vector<TTOML>> vec = set->as_array()->get_array_of<TTOML>();
		if(!vec)
		{
			std::cout << "Error reading setting '" << setting.c_str() << "'. The setting must be an array of type '" << TypeNameOf<T>() << "'.";
			return false;
		}

		// Copy elements over
		values.clear();
		values.reserve(vec->size());
		for(const TTOML& v : *vec)
		{
			values.push_back(static_cast<const T>(v));
		}
		return true;
	}
	catch(std::exception& ex)
	{
		std::cout << "Error reading setting '" << setting.c_str() << "'. " << TypeNameOf(ex) << ": " << ex.what();
		return false;
	}
}

// This attempts to retreives a setting as integer
int TomlParser::GetInt(const String& setting, int defaultvalue) const
{
	int value = defaultvalue;
	GetValueAs<int>(setting, value);
	return value;
}

// This attempts to retreives a setting as long integer
int64 TomlParser::GetLong(const String& setting, int64 defaultvalue) const
{
	int64 value = defaultvalue;
	GetValueAs<int64>(setting, value);
	return value;
}

// This attempts to retreives a setting as double
double TomlParser::GetDouble(const String& setting, double defaultvalue) const
{
	double value = defaultvalue;
	GetValueAs<double>(setting, value);
	return value;
}

// This attempts to retreives a setting as boolean
bool TomlParser::GetBool(const String& setting, bool defaultvalue) const
{
	bool value = defaultvalue;
	GetValueAs<bool>(setting, value);
	return value;
}

// This attempts to retreives a setting as string
String TomlParser::GetString(const String& setting, const String& defaultvalue) const
{
	String value = defaultvalue;
	GetValueAs<String>(setting, value);
	return value;
}

// This attempts to retreives a setting as string
String TomlParser::GetString(const String& setting, const char* defaultvalue) const
{
	String value = defaultvalue;
	GetValueAs<String>(setting, value);
	return value;
}

// Implementations which use GetValueArrayAs to retreive and format an array of values
bool TomlParser::GetIntArray(const String& setting, vector<int>& array) const { return GetValueArrayAs<int>(setting, array); }
bool TomlParser::GetLongArray(const String& setting, vector<int64>& array) const { return GetValueArrayAs<int64>(setting, array); }
bool TomlParser::GetDoubleArray(const String& setting, vector<double>& array) const { return GetValueArrayAs<double>(setting, array); }
bool TomlParser::GetBoolArray(const String& setting, vector<bool>& array) const { return GetValueArrayAs<bool>(setting, array); }
bool TomlParser::GetStringArray(const String& setting, vector<String>& array) const { return GetValueArrayAs<String>(setting, array); }

// This attempts to retreive an array element
std::shared_ptr<cpptoml::array> TomlParser::GetArray(const String& setting) const
{
	std::shared_ptr<cpptoml::base> set;
	try
	{
		// Get the specified setting.
		set = root->get_qualified(setting.c_str());
	}
	// If the setting is not in the configuration, this throws out_of_range exception.
	catch(std::out_of_range& ex)
	{
		return nullptr;
	}

	// Better safe than sorry
	if((set == nullptr) || !set->is_array())
		return nullptr;

	// Cast the result to array
	// If this is not an array, then the result will be nullptr
	return dynamic_pointer_cast<cpptoml::array>(set);
}

// This attempts to retreive a table element
std::shared_ptr<cpptoml::table> TomlParser::GetTable(const String& path) const
{
	std::shared_ptr<cpptoml::base> set;
	try
	{
		// Get the specified table.
		set = root->get_qualified(path.c_str());
	}
	// If the setting is not in the configuration, this throws out_of_range exception.
	catch(std::out_of_range& ex)
	{
		return nullptr;
	}

	// Better safe than sorry
	if((set == nullptr) || !set->is_table())
		return nullptr;

	// Cast the result to table
	// If this is not a table, then the result will be nullptr
	return dynamic_pointer_cast<cpptoml::table>(set);
}

// This attempts to retreive an array of table elements
std::shared_ptr<cpptoml::table_array> TomlParser::GetTableArray(const String& path) const
{
	std::shared_ptr<cpptoml::base> set;
	try
	{
		// Get the specified table.
		set = root->get_qualified(path.c_str());
	}
	// If the setting is not in the configuration, this throws out_of_range exception.
	catch(std::out_of_range& ex)
	{
		return nullptr;
	}

	// Better safe than sorry
	if((set == nullptr) || !set->is_table_array())
		return nullptr;

	// Cast the result to table
	// If this is not a table, then the result will be nullptr
	return dynamic_pointer_cast<cpptoml::table_array>(set);
}

// Helper function to write a setting
template<typename T>
void TomlParser::SetValueAs(std::shared_ptr<cpptoml::table>& table, const String& key, T value)
{
	typedef typename toml_type_for<T>::type TTOML;
	std::shared_ptr<cpptoml::value<TTOML>> tomlvalue = cpptoml::make_value<TTOML>(static_cast<TTOML>(value));
	std::shared_ptr<cpptoml::base> tomlobj = static_pointer_cast<cpptoml::base>(tomlvalue);
	SetRecursive(table, key, tomlobj);
}

// Helper function to write an array setting
template<typename T>
void TomlParser::SetValueArrayAs(std::shared_ptr<cpptoml::table>& table, const String& key, const vector<T>& values)
{
	typedef typename toml_type_for<T>::type TTOML;
	std::shared_ptr<cpptoml::array> arr = cpptoml::make_array();
	arr->reserve(values.size());
	for(const T& v : values)
	{
		arr->push_back(static_cast<TTOML>(v));
	}
	std::shared_ptr<cpptoml::base> tomlobj = static_pointer_cast<cpptoml::base>(arr);
	SetRecursive(table, key, tomlobj);
}

// Implementations which use SetValueAs to store a value
void TomlParser::SetInt(const String& setting, int value) { SetValueAs<int>(root, setting, value); }
void TomlParser::SetLong(const String& setting, int64 value) { SetValueAs<int64>(root, setting, value); }
void TomlParser::SetDouble(const String& setting, double value) { SetValueAs<double>(root, setting, value); }
void TomlParser::SetString(const String& setting, const String& value) { SetValueAs<String>(root, setting, value); }
void TomlParser::SetString(const String& setting, const char* value) { SetValueAs<const char*>(root, setting, value); }
void TomlParser::SetBool(const String& setting, bool value) { SetValueAs<bool>(root, setting, value); }

// Implementations which use SetValueAs to store an array of values
void TomlParser::SetIntArray(const String& setting, const vector<int>& values) { SetValueArrayAs<int>(root, setting, values); }
void TomlParser::SetLongArray(const String& setting, const vector<int64>& values) { SetValueArrayAs<int64>(root, setting, values); }
void TomlParser::SetDoubleArray(const String& setting, const vector<double>& values) { SetValueArrayAs<double>(root, setting, values); }
void TomlParser::SetStringArray(const String& setting, const vector<String>& values) { SetValueArrayAs<String>(root, setting, values); }
void TomlParser::SetBoolArray(const String& setting, const vector<bool>& values) { SetValueArrayAs<bool>(root, setting, values); }

// This writes the configuration to a file.
// May throw exceptions when the writing fails.
void TomlParser::WriteFile(const String& filename) const
{
	std::ofstream file(filename);
	if(!file.is_open())
		throw std::domain_error(ConcatString("Unable to write configuration to file \"", filename, "\"."));
	file << (*root);
}

// This serializes the configuration to a string.
String TomlParser::WriteString() const
{
	std::stringstream stream;
	stream << (*root);
	return String(stream.str());
}

// This returns a list of setting names for the specified section.
// If the specified section is not a table, or cannot be found, this returns an empty vector.
vector<String> TomlParser::GetSettings(const String& section) const
{
	vector<String> results;
	std::shared_ptr<cpptoml::base> set;
	try
	{
		// Get the specified section.
		set = root->get_qualified(section.c_str());
	}
	// If the section is not in the configuration, this throws out_of_range exception.
	catch(std::out_of_range& ex)
	{
		return results;
	}

	// Check if the setting is a table
	if((set != nullptr) && set->is_table())
	{
		// Copy all key names to the results
		std::shared_ptr<cpptoml::table> table = set->as_table();
		for(auto&& kv : *table)
			results.push_back(String(kv.first));
	}

	return results;
}

// This writes an cpptoml object to the configuration.
void TomlParser::SetRecursive(std::shared_ptr<cpptoml::table>& table, const String& key, std::shared_ptr<cpptoml::base>& obj)
{
	int dotpos = key.Find('.');
	if(dotpos > -1)
	{
		REQUIRE(dotpos > 0);
		REQUIRE((uint)dotpos < key.Length() - 1);

		// The key specifies a value inside a table. We must find or construct the table
		// and recursively call SetArrayRecursive again set the value in the table...
		String tablename = key.Substring(0, dotpos);
		String remaining = key.Substring(dotpos + 1);
		std::shared_ptr<cpptoml::table> innertable = table->get_table(tablename.stl());
		if(innertable == nullptr)
		{
			// Make a new table
			innertable = cpptoml::make_table();
			table->insert(tablename.stl(), innertable);
		}

		SetRecursive(innertable, remaining, obj);
	}
	else
	{
		// Insert the array in the given table
		table->erase(key.stl());
		table->insert(key.stl(), obj);
	}
}

// This writes an array to the configuration.
void TomlParser::SetArray(const String& setting, std::shared_ptr<cpptoml::array> values)
{
	std::shared_ptr<cpptoml::base> tomlobj = static_pointer_cast<cpptoml::base>(values);
	SetRecursive(root, setting, tomlobj);
}

// This writes a table to the configuration.
void TomlParser::SetTable(const String& setting, std::shared_ptr<cpptoml::table> table)
{
	std::shared_ptr<cpptoml::base> tomlobj = static_pointer_cast<cpptoml::base>(table);
	SetRecursive(root, setting, tomlobj);
}

// This writes an array of tables to the configuration.
void TomlParser::SetTableArray(const String& setting, std::shared_ptr<cpptoml::table_array> tables)
{
	std::shared_ptr<cpptoml::base> tomlobj = static_pointer_cast<cpptoml::base>(tables);
	SetRecursive(root, setting, tomlobj);
}

