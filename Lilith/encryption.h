#pragma once

#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <iostream>
#include <string>

class Encryption
{
public:
	static std::string encWeakKeyXOR(std::string input, std::string key);
	static std::string decWeakKeyXOR(std::string input, std::string key);
};

#endif