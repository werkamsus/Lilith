#pragma once

#ifndef SERVER_H
#define SERVER_H



#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib,"ws2_32.lib") //Required for WinSock
#include <WinSock2.h> //For win sockets
#include <string>

#include "general.h"

class Server
{
public:
	Server(int PORT, bool BroadcastPublically);
	bool listenForNewConnection();

private:

	enum PacketType;
	bool ProcessPacket(int ID);
	bool sendPacket(int ID, std::string message, PacketType _PacketType);



	static void ClientHandlerThread(int ID);
	//static void PacketSenderThread();

	//void DisconnectClient(int ID); //Called to properly disconnect and clean up a client (if possible)
private:
	SOCKET Connections[100];
	int TotalConnections = 0;
	/*
	std::vector<std::shared_ptr<Connection>> connections;
	std::mutex connectionMgr_mutex; //mutex for managing connections (used when a client disconnects)
	int UnusedConnections = 0; //# of Inactive Connection Objects that can be reused
	*/

	SOCKADDR_IN addr; //Address that we will bind our listening socket to
	int addrlen = sizeof(addr);
	SOCKET sListen;

};

static Server * serverptr; //Serverptr is necessary so the static ClientHandler method can access the server instance/functions.


#endif // !SERVER_H