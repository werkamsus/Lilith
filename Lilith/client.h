#pragma once

#ifndef CLIENT_H
#define CLIENT_H

#pragma comment(lib,"ws2_32.lib") //Required for WinSock#include "client.h"
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>

#include "general.h"
#include "cmdRedirect.h"

enum Packet
{
	P_Instruction,
	P_CMDCommand,
	P_Error
};

class Client
{
public: //Public functions
	Client(std::string IP, int PORT);
	bool Connect();

	//bool SendString(std::string & _string, bool IncludePacketType = true);
	bool CloseConnection();
	//bool RequestFile(std::string FileName);
	static Client * clientptr;
private: //Private functions
	//bool ProcessPacketType(PacketType _PacketType);
	static void ClientThread();

	static void ClientThread();
private:
	//Sending Funcs
	bool SendInt(int _int);
	bool SendPacketType(Packet _packettype);


	//Getting Funcs
	bool GetInt(int & _int);
	bool GetPacketType(Packet & _packettype);
	bool GetString(std::string & _string);

private:
	//FileTransferData file; //Object that contains information about our file that is being received from the server.
	SOCKET sConnection;//This client's connection to the server
	SOCKADDR_IN addr; //Address to be binded to our Connection socket
	static bool connected;
};

	//This client ptr is necessary so that the ClientThread method can access the Client instance/methods. 
							//Since the ClientThread method is static, this is the simplest workaround I could think of since there will only be one instance of the client.

#endif // !CLIENT_H