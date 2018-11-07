#include "Globals.h"

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