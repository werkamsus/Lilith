#pragma once

#ifndef GENERAL_H
#define GENERAL_H
#define WIN32_LEAN_AND_MEAN

#include <string>
#include <iostream>
#include <arpa/inet.h>
#include <netdb.h>
#include <memory>
#include <thread>
#include <string.h> // For the memcpy stuff
#include "termcolor.hpp" //Simple header only libary for colored text in console
#include "server.h"

class General
{
public:	//functions
	static void outputMsg(std::string message, int msgType);
	static bool processParameter(std::string &command, std::string compCommand);
public:	//variables
	static bool cmdMode;
};


#endif // !GENERAL_H