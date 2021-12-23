#pragma once
#include <typeinfo>
#include "String.h"

// This helps decoding the type name return by type_info::name
String DemangleTypeName(const char* name);

// This returns the class name of the given object as a string
template <class T>
String TypeNameOf(const T& t)
{
	return DemangleTypeName(typeid(t).name());
}

// This returns the class name of the given class type
template <class T>
String TypeNameOf()
{
	return DemangleTypeName(typeid(T).name());
}
