
#include <string>
#include <Windows.h>
#include "settings.h"

extern std::string fileName = "latestProjectTest.exe";					//file name
extern std::string folderName = "latestProjectTestDIR";					//name of folder where file is located
extern std::string startupName = "latest proj test startup name";		//startup name in registry / taskmgr
extern std::string installLocation = "APPDATA"; //install location (appdata, programdata etc)
extern bool installSelf = true;				//specifies whether the program should install itself
extern bool startOnNextBoot = false;		//specifies whether it should startup the installed clone of itself NOW or ON THE NEXT BOOT (ONLY IMPORTANT FOR INSTALLATION PROCESS)
extern bool meltSelf = false;				//specifies whether the installed clone should delete the initial file
extern bool setStartupSelf = false;			//specifies whether the program is to be started on system boot