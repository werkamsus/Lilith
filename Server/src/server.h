#pragma once
#include <string> //For std::string
#include <iostream> //For std::cout, std::endl
#include <vector> //for std::vector

#include "FileTransferData.h"
#include "PacketManager.h"
#include "PacketStructs.h"
#include "general.h"

class Connection
{
public:
	Connection(int socket_)
	{
		socket = socket_;
		ActiveConnection = true; //Default to active connection 
	}
	bool ActiveConnection; //True if connection is active, false if inactive(due to a disconnect)
	int socket;
	//file transfer data
	FileTransferData file; //Object that contains information about our file that is being sent to the client from this server
	PacketManager pm; //Packet Manager for outgoing data for this connection
};

class Server
{
public:
	Server(int PORT, bool BroadcastPublically = false);
	void ListenForNewConnection();
	void HandleInput();
	static Server * serverptr; //Serverptr is necessary so the static ClientHandler method can access the server instance/functions.

private:

	void handleScript(std::string script);

	bool sendall(int ID, char * data, int totalbytes);
	bool recvall(int ID, char * data, int totalbytes);

	bool Sendint32_t(int ID, int32_t _int32_t);
	bool Getint32_t(int ID, int32_t & _int32_t);

	bool SendPacketType(int ID, PacketType _packettype);
	bool GetPacketType(int ID, PacketType & _packettype);

	void SendString(int ID, std::string const & _string, PacketType _packettype);
	bool GetString(int ID, std::string & _string);

	bool ProcessPacket(int ID, PacketType _packettype);
	bool HandleSendFile(int ID);

	static void ClientHandlerThread(int ID);
	static void PacketSenderThread(void);
	static void ListenerThread();

	void DisconnectClient(int ID); //Called to properly disconnect and clean up a client (if possible)
private:
	std::vector<std::shared_ptr<Connection>> connections;
	std::mutex connectionMgr_mutex; //mutex for managing connections (used when a client disconnects)
	int UnusedConnections = 0; //# of Inactive Connection Objects that can be reused

	sockaddr_in addr; //Address that we will bind our listening socket to
	socklen_t addrlen = sizeof(addr);
	int sListen;
	int currentSessionID;
};

