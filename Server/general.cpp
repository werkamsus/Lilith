#include "general.h"

void General::outputMsg(std::string message, int msgType)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	switch (msgType)
	{
	case 1:
		std::cout << message << std::endl;
		break;
	case 2:
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
		std::cout << "[ERROR] " << message << std::endl;
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		break;
	case 3:
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY);
		std::cout << "[CMD] " << message << std::endl;
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		break;
	}
}