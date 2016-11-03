#pragma once

#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>
#include <Windows.h>



class Settings
{
public:
	static std::string fileName;		//file name
	static std::string folderName;	//folder name
	static std::string startupName;		//startup name in registry / taskmgr
	static std::string installLocation; //install location (appdata, programdata etc)
	static bool installSelf;			//specifies whether the program should install itself
	static bool startOnNextBoot;		//specifies whether it should startup the installed clone of itself NOW or ON THE NEXT BOOT (ONLY IMPORTANT FOR INSTALLATION PROCESS)
	static bool meltSelf;				//specifies whether the installed clone should delete the initial file
	static bool setStartupSelf;			//specifies whether the program is to be started on system boot
	static bool logEvents;				//speciifies whether the program should log events (like errors etc)

};

#endif // !SETTINGS_H