#pragma once
#include <string> //For std::string
#include <iostream> //For std::cout, std::endl, std::cin.getline
#include <Windows.h>
#include "general.h"

class CMD
{
public: //Public functions
	CMD();
	static void cmdThread();

	std::string readCMD();
	void writeCMD(std::string command);

public:	//Public variables
	static CMD * cmdptr;
	static bool cmdOpen;

private: //Private functions
	void createChildProcess();

private:	//variables
	HANDLE g_hChildStd_IN_Rd = NULL;
	HANDLE g_hChildStd_IN_Wr = NULL;
	HANDLE g_hChildStd_OUT_Rd = NULL;
	HANDLE g_hChildStd_OUT_Wr = NULL;

	static HANDLE g_hChildProcess;
	static HANDLE g_hChildThread;


	SECURITY_ATTRIBUTES saAttr;
};