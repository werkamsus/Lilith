#pragma once

#ifndef GENERAL_H
#define GENERAL_H
#define WIN32_LEAN_AND_MEAN

#include <string>
#include <iostream>
#include <Windows.h>
#include "server.h"

class General
{
public:	//functions
	static void outputMsg(std::string message, int msgType);
	static bool General::processParameter(std::string &command, std::string compCommand);
public:	//variables
	static bool cmdMode;
};


#endif // !GENERAL_H