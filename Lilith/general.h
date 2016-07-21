#pragma once

#include <Windows.h>
#include <string>

#include "settings.h"

#ifndef GENERAL_H
#define GENERAL_H


bool directoryExists(const char* dirName);							//checks if directory exists
void startProcess(LPCTSTR lpApplicationName, LPTSTR lpArguments);		//starts a process
std::string getInstallFolder();											//gets install folder (example: C:\users\USER\AppData\Roaming\InstallDIR)
std::string getInstallPath(std::string instFolder);						//gets installpath (environment folder + folder name (if supplied) + file name)
std::string getCurrentPath();										//gets current path of executable


#endif // !GENERAL_H