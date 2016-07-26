#pragma once

#include <Windows.h>
#include <string>

#include "settings.h"

#ifndef GENERAL_H
#define GENERAL_H


bool regValueExists(HKEY hKey, LPCSTR keyPath, LPCSTR valueName);	//checks if a certain value exists in the registry
bool setStartup(PCWSTR pszAppName, PCWSTR pathToExe, PCWSTR args);	//registers a program in startup with supplied name, path to exe and startup arguments
bool directoryExists(const char* dirName);							//checks if directory exists
void startProcess(LPCTSTR lpApplicationName, LPTSTR lpArguments);		//starts a process
std::string getInstallFolder();											//gets install folder (example: C:\users\USER\AppData\Roaming\InstallDIR)
std::string getInstallPath(std::string instFolder);						//gets installpath (environment folder + folder name (if supplied) + file name)
std::string getCurrentPath();										//gets current path of executable


#endif // !GENERAL_H