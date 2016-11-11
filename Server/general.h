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
public:
	static void outputMsg(std::string message, int msgType);
};


#endif // !GENERAL_H