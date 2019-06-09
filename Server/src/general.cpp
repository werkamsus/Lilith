#include "general.h"

bool General::cmdMode = false;

void General::outputMsg(std::string message, int msgType)
{
	switch (msgType)
	{
	case 1:
		std::cout << termcolor::green << message;
		std::cout << termcolor::reset << "" << std::endl;
		break;
	case 2:
		std::cout << termcolor::red << "[ERROR] " << message;
		std::cout << termcolor::reset << "" << std::endl;
		break;
	case 3:
		std::cout << message;
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
