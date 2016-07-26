#pragma once


#include <string>

#ifndef SETTINGS_H
#define SETTINGS_H

extern std::string fileName;		//file name
extern std::string folderName;	//folder name
extern std::string startupName;		//startup name in registry / taskmgr
extern std::string installLocation; //install location (appdata, programdata etc)
extern bool installSelf;			//specifies whether the program should install itself
extern bool startOnNextBoot;		//specifies whether it should startup the installed clone of itself NOW or ON THE NEXT BOOT (ONLY IMPORTANT FOR INSTALLATION PROCESS)
extern bool meltSelf;				//specifies whether the installed clone should delete the initial file
extern bool setStartupSelf;			//specifies whether the program is to be started on system boot


#endif // !SETTINGS_H