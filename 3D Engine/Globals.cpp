#include "Globals.h"
#include <random>

// ~~ FUNCTIONS ~~
bool FileExist(const std::string & name)
{
	if (FILE *file = fopen(name.c_str(), "r"))
	{
		fclose(file);
		return true;
	}
	else
	{
		return false;
	}
}

std::string GetFileNameFromPath(const char * path)
{
	std::string filename = path;

	// Remove directory
	const size_t last_slash = filename.find_last_of("\\/");
	if (std::string::npos != last_slash)
	{
		filename.erase(0, last_slash + 1);
	}
	// Remove extension
	const size_t extension_pos = filename.rfind('.');
	if (std::string::npos != extension_pos)
	{
		filename.erase(extension_pos);
	}

	return filename;
}

uint GenRandomNumber()
{
	uint number = 0;
	std::random_device rd;
	std::mt19937_64 eng(rd());
	std::uniform_int_distribution<uint> distr;
	number = distr(eng);
	
	return number;
}
