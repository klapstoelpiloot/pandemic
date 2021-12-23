#include <fstream>
#include <streambuf>
#include <filesystem>
#include <cstdlib>
#include <memory>
#include <pwd.h>
#include "File.h"

using namespace std::filesystem;

// Tests if a file exists
bool File::FileExists(const String& filename)
{
	std::error_code ec;

	// Check to make sure this is not a directory path
	// This sets ec when the directory or file does not exist, so we ignore ec for now.
	if(is_directory(filename.c_str(), ec))
		return false;

	// Check to make sure this is a file path
	if(!is_regular_file(filename.c_str(), ec))
		return false;

	// Check if the file exists
	bool result = exists(filename.c_str(), ec);
	if(ec)
		throw std::system_error(ec);

	return result;
}

// Tests if a directory exists
bool File::DirectoryExists(const String& dirname)
{
	std::error_code ec;

	// Check to make sure this is a directory path
	return (is_directory(dirname.c_str(), ec) && (ec.value() == 0));
}

bool File::CreateDirectory(const String& dirname)
{
	return create_directory(dirname.c_str());
}

// Reads a file as text and returns it as String
String File::ReadAsText(const String& filename)
{
	// Open the file
	std::ifstream stream(filename.c_str(), std::ios_base::in);
	if(!stream.good())
		throw std::ios_base::failure("Failed to open file.");

	// Determine the size of the file
	stream.seekg(0, std::ios::end);
	size_t size = checked_cast<size_t>(stream.tellg());

	// Allocate memory for the data
	String str(' ', checked_cast<unsigned int>(size));
	
	// Read the data from file
	stream.seekg(0, std::ios::beg);
	stream.read(&str.Front(), size);
	stream.close();

	return str;
}

// Writes a String to a file, overwriting any previous file
void File::WriteText(const String& text, const String& filename)
{
	// Open the file
	std::ofstream stream(filename.c_str(), std::ios_base::out | std::ios_base::trunc);

	// Write the string to the file
	stream.write(&text.Front(), text.Length());
}

// Reads a binary file and returns the data
void File::ReadAsBinary(const String& filename, vector<char>& data)
{
	// Open the file
	std::ifstream stream(filename.c_str(), std::ios::binary | std::ios::in);
	if(!stream.good())
		throw std::ios_base::failure("Failed to open file.");

	// Determine the size of the file
	stream.seekg(0, std::ios::end);
	size_t fsize = checked_cast<size_t>(stream.tellg());

	// Allocate memory for the data
	// NOTE: reserve() does not guarantee proper memory allocation for use with data()
	data.resize(fsize);

	// Read the file data
	stream.seekg(0, std::ios::beg);
	stream.read(data.data(), fsize);
	stream.close();
}

// Combine two path strings and insert a separator if needed
String File::CombinePath(const char* path1, const char* path2)
{
	REQUIRE(path1 != nullptr);
	REQUIRE(path2 != nullptr);
	String p1 = path1;
	String p2 = path2;
	String separator = path::preferred_separator;
	if(!p1.EndsWith(separator) && !p2.StartsWith(separator)) p1 += separator;
	return p1 + p2;
}

// Check if a path is relative
bool File::IsPathRelative(const char* pathfilename)
{
	REQUIRE(pathfilename != nullptr);
	path p = pathfilename;
	return p.is_relative();
}

// Returns the root name of the specified path
String File::GetRootName(const char* pathfilename)
{
	REQUIRE(pathfilename != nullptr);
	path p = pathfilename;
	ENSURE(p.begin() != p.end());
	p = *p.begin();
	return String(p.c_str());
}

// Returns the filename (or directory name) only of the specified path
String File::GetFileName(const char* pathfilename)
{
	REQUIRE(pathfilename != nullptr);
	path p = pathfilename;
	p = p.filename();
	return String(p.c_str());
}

// Returns the extension of the specified path/filename
String File::GetExtension(const char* pathfilename)
{
	REQUIRE(pathfilename != nullptr);
	path p = pathfilename;
	p = p.extension();
	return String(p.c_str());
}

// Returns the (parent) directory of the specified path
String File::GetDirectory(const char* pathfilename)
{
	REQUIRE(pathfilename != nullptr);
	path p = pathfilename;
	p = p.parent_path();
	return String(p.c_str());
}

bool File::GetSubDirectories(const char* pathfilename, vector<String>& list)
{
	REQUIRE(pathfilename != nullptr);
	path p = pathfilename;
	if(!exists(p)) return false;
	directory_iterator it(p);
	directory_iterator end;
	while(it != end)
	{
		// Add only directories to the list
		if(is_directory(*it))
			list.push_back(it->path().c_str());

		it++;
	}
	return true;
}

bool File::GetFiles(const char* pathfilename, vector<String>& list)
{
	REQUIRE(pathfilename != nullptr);
	path p = pathfilename;
	if(!exists(p)) return false;
	directory_iterator it(p);
	directory_iterator end;
	while(it != end)
	{
		// Add only files to the list
		if(!is_directory(*it))
			list.push_back(it->path().c_str());

		it++;
	}
	return true;
}

// This returns the user home directory
// The string ends with a forward slash
String File::GetUserHomeDir()
{
	char buffer[1024];
	passwd pw;
	passwd* pwptr;

	getpwuid_r(getuid(), &pw, buffer, 1024, &pwptr);

	String homedir = pwptr->pw_dir;
	if(!homedir.EndsWith("/"))
		homedir.Append("/");
	return homedir;
}

// This returns the directory of the calling process.
// The string ends with a forward slash.
String File::GetCurrentProcessDir()
{
	char buffer[1024];
	ssize_t pathlen = readlink("/proc/self/exe", buffer, 1023);
	ASSERT(pathlen > -1, "Unable to get the process directory.");
	String pathstr = GetDirectory(buffer);
	if(!pathstr.EndsWith("/"))
		pathstr.Append("/");
	return pathstr;
}
