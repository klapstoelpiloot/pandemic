#include "RTTI.h"
#include <memory>
#include <cxxabi.h>

#ifdef __GNUG__

// This helps decoding the type name return by type_info::name
String DemangleTypeName(const char* name)
{
	int status = 1;
	std::unique_ptr<char, void(*)(void*)> res
	{
		abi::__cxa_demangle(name, NULL, NULL, &status),
		std::free
	};

	return (status == 0) ? res.get() : name;
}

#else

// Does nothing if not g++
String DemangleTypeName(const char* name)
{
	return String(name);
}

#endif
