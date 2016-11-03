#pragma once

#ifndef CONVERSION_H
#define CONVERSION_H

#include <string>
#include <Windows.h>



extern LPTSTR convStringToLPTSTR(std::string s);
extern std::wstring convStringToWidestring(const std::string& s);


#endif