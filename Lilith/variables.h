#pragma once


#include <string>

#ifndef VARIABLES_H
#define VARIABLES_H

std::string currentPath;			//current path of executable
std::string installFolder;		//path of folder it should be installed to
std::string installPath;			//full path where executable should be installed to
bool installing;			//bool - defines whether the file is currently being installed (and should be terminated after the initiation sequence,
																							//instead of proceeding to the main loop)
LPTSTR lpArguments;

#endif // !VARIABLES_H