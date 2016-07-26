#pragma once

#include <string>
#include <Windows.h>


#ifndef CONVERSION_H
#define CONVERSION_H

extern LPTSTR convStringToLPTSTR(std::string s);
extern std::wstring convStringToWidestring(const std::string& s);


#endif