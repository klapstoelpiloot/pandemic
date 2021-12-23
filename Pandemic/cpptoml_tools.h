#pragma once
#include "cpptoml.h"
#include "Tools.h"

// This controls what cpptoml storage type we use for specific types
template<typename T> class toml_type_for;
template<> class toml_type_for<bool> { public: typedef bool type; };
template<> class toml_type_for<int> { public: typedef int64_t type; };
template<> class toml_type_for<uint> { public: typedef int64_t type; };
template<> class toml_type_for<short> { public: typedef int64_t type; };
template<> class toml_type_for<ushort> { public: typedef int64_t type; };
template<> class toml_type_for<sbyte> { public: typedef int64_t type; };
template<> class toml_type_for<byte> { public: typedef int64_t type; };
template<> class toml_type_for<int64> { public: typedef int64_t type; };
template<> class toml_type_for<float> { public: typedef double type; };
template<> class toml_type_for<double> { public: typedef double type; };
template<> class toml_type_for<String> { public: typedef std::string type; };
template<> class toml_type_for<const char*> { public: typedef std::string type; };
template<> class toml_type_for<std::string> { public: typedef std::string type; };


