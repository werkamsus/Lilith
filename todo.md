# To-Do
---

Before you proceed to read:
---
This is a "blueprint" for what this piece of software is ultimately supposed to do. Feel free to suggest whatever comes to your mind!
It is in no way spell-checked / organised / reader-friendly, as I've just written everything down i could think of at the time.
Edit: I've tried to somehow structure it.

---

/* 		MALWARE BLUEPRINT 		*/
/*	CREATION DATE: 01.02.2016	*/
/* 			BY WERKAMSUS		*/



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Execution Flow:

* FIRST RUN
* copy file
* set startup
* melt old file
* END OF FIRST RUN
* NORMAL RUN
* delay stuff etc.
* check if files intact
* check for commands (website, server whatevs.)
* write date, information etc. to server (system profiling perhaps(?))
* if no commands wait a couple of seconds then close (no idling, come up with task)

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

OLD MLWR.txt:

* windows service
* use technique for user elevation
* inject dll into some process
* forget about svchost, wont work.
* windows service can be a process, but only on startup. 
* (!!!) AND MAKE PROCESS CRITICAL / RELOAD DLL ON TERMINATION, BACKUP INJECTION?
* (persitence with sources/KeepAlive(?))
  * then inject (polymorphic?) dll into some process (explorer?[tooObv?] 
  * csrss.exe(if possible)
  * rundll32.exe(if possible)
  * chrome
  * services.exe
  * spoolsv.exe, etc...)
* [scan for processes, pick most suitable target, change on every application startup]
* MAKE LAPTOP COMPATIBLE - backup service that checks if process is running / responding and restarts it if not (look into KeepAlive)

* PWNAT tunneling (samy.pl/pwnat)
* Shellter(?) [implement sourcecode]
* PE Injection in general, look into: hooks etc.
* powershell stuff? use trusted system tools as shell backdoor etc or to start applications

* implement native (c++) microphone, camera etc hijacking
* keylogger
* screenshot tool etc
* file manager (optional)
* console, up / download possibilites
* check for powershell possibilites
* network scan tool
* reverse proxy w/ pwnat (INCREDIBLE, LOOK INTO (!!!!!!))
* registry manager
* task manager (ofc lol)
* antivirus manual removal tool (super advanced, dont bother until 1337 af)


CONCEPT: powershell command in registry key that injects dll into process -> no .exe injection tool; ONLY DLL (.NET func. maybe)

possible injection paths:

"C:\Windows\SysWOW64\rundll32.exe"
"C:\Users\werkamsus\AppData\Local\Microsoft\OneDrive\OneDrive.exe"
chrome.exe that is NOT in a job
"C:\Windows\System32\conhost.exe" (64bit, on 64bit OS, else 32(maybe))

USEFUL FUNCTIONS:
TO HIDE WINDOWS: https://msdn.microsoft.com/en-us/library/ms633548(VS.85).aspx
