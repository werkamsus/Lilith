#include <Windows.h>
#include <string>

#include "general.h"

using namespace std;

bool directoryExists(const char* dirName)			//checks if directory exists
{
	DWORD attribs = ::GetFileAttributesA(dirName);
	if (attribs == INVALID_FILE_ATTRIBUTES)
		return false;
	return true;			//original code : return (attribs & FILE_ATTRIBUTE_DIRECTORY); [CHANGED BC WARNING]
}

void startProcess(LPCTSTR lpApplicationName, LPTSTR lpArguments)		//starts a process
{
	// additional information
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	// set the size of the structures
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	// start the program up
	CreateProcess(lpApplicationName,   // the path
		lpArguments,        // Command line
		NULL,           // Process handle not inheritable
		NULL,           // Thread handle not inheritable
		FALSE,          // Set handle inheritance to FALSE
		0,              // No creation flags
		NULL,           // Use parent's environment block
		NULL,           // Use parent's starting directory 
		&si,            // Pointer to STARTUPINFO structure
		&pi);           // Pointer to PROCESS_INFORMATION structure
						// Close process and thread handles. 
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
}

string getInstallFolder()		//gets install folder (example: C:\users\USER\AppData\Roaming\InstallDIR)
{
	string rest = "";
	if (!(folderName == ""))
		rest = "\\" + folderName;

	string concat;
	char* buf = 0;
	size_t sz = 0;
	if (_dupenv_s(&buf, &sz, installLocation.c_str()) == 0) //gets environment variable
		if (buf != NULL)
		{

			concat = string(buf) + rest; //concatenates string
			free(buf);
		}
	return concat;
}

string getInstallPath(string instFolder)		//gets installpath (environment folder + folder name (if supplied) + file name)
{
	string concat;
	concat = instFolder + "\\" + fileName;

	return concat;
}

string getCurrentPath()		//gets current path of executable
{
	char buf[MAX_PATH];
	GetModuleFileName(0, buf, MAX_PATH);
	return string(buf);
}