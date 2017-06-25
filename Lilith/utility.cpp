#include "utility.h"

bool Utility::fileExists(std::string path)
{
	FILE *file;
	if (fopen_s(&file, path.c_str(), "r") == 0)
	{
		fclose(file);
		return true;
	}
	else
		return false;
}