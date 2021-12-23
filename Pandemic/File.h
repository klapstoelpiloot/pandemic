#pragma once
#include "Tools.h"

class File
{
public:
	// Check if a file exists
	static bool FileExists(const String& filename);

	// Tests if a directory exists
	static bool DirectoryExists(const String& dirname);

	// Creates the specified directory
	static bool CreateDirectory(const String& dirname);

	// Reads an enitre file as String
	static String ReadAsText(const String& filename);

	// Writes a String to a file
	static void WriteText(const String& text, const String& filename);

	// Reads an entire file as raw data
	static void ReadAsBinary(const String& filename, vector<char>& data);

	// Combine two path strings and insert a separator if needed
	static String CombinePath(const char* path1, const char* path2);

	// Check if a path is relative
	static bool IsPathRelative(const char* pathfilename);

	// Returns the filename (or directory name) only of the specified path
	static String GetFileName(const char* pathfilename);

	// Returns the extension of the specified path/filename
	static String GetExtension(const char* pathfilename);

	// Returns the root name of the specified path
	static String GetRootName(const char* pathfilename);

	// Returns the (parent) directory of the specified path
	static String GetDirectory(const char* pathfilename);

	// Returns the child directories of the specified path. Note: Does not clear the list in advance!
	static bool GetSubDirectories(const char* pathfilename, vector<String>& list);

	// Returns the files (excluding subdirectories) in the specified path. Note: Does not clear the list in advance!
	static bool GetFiles(const char* pathfilename, vector<String>& list);

	// This returns the user home directory
	// The string ends with a forward slash
	static String GetUserHomeDir();

	// This returns the directory of the calling process.
	// The string ends with a forward slash.
	static String GetCurrentProcessDir();
};
