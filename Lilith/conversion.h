#pragma once

#ifndef CONVERSION_H
#define CONVERSION_H

#include <string>
#include <Windows.h>


class Conversion
{
public:
	static LPTSTR convStringToLPTSTR(std::string s);
	static std::wstring convStringToWidestring(const std::string& s);
};

#endif