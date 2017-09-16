#include "general.h"

bool General::cmdMode = false;

void General::outputMsg(std::string message, int msgType)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	switch (msgType)
	{
	case 1:
		SetConsoleTextAttribute(hConsole, 10);
		std::cout << message << std::endl;
		SetConsoleTextAttribute(hConsole, 7);
		break;
	case 2:
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
		std::cout << "[ERROR] " << message << std::endl;
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		break;
	case 3:
		//SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY);
		std::cout << message;
		//SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		break;
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
