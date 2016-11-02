#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#pragma comment(lib,"ws2_32.lib") //Required for WinSock
#include <WinSock2.h> //For win sockets
#include <string> //For std::string
#include <iostream> //For std::cout, std::endl, std::cin.getline

class Client
{
public: //Public functions
	Client(std::string IP, int PORT);
	bool Connect();

	//bool SendString(std::string & _string, bool IncludePacketType = true);
	bool CloseConnection();
	//bool RequestFile(std::string FileName);
private: //Private functions
	//bool ProcessPacketType(PacketType _PacketType);
	static void ClientThread();

	enum PacketType;
	bool ReceivePacket();
	bool ProcessPacket(PacketType _packettype);
	bool Client::sendPacket(std::string message, PacketType _PacketType);
	//Sending Funcs
	//bool sendall(char * data, int totalbytes);
	//bool Sendint32_t(int32_t _int32_t);
	//bool SendPacketType(PacketType _PacketType);


	//Getting Funcs
	//bool recvall(char * data, int totalbytes);
	//bool Getint32_t(int32_t & _int32_t);
	//bool GetPacketType(PacketType & _PacketType);
	//bool GetString(std::string & _string);

private:
	//FileTransferData file; //Object that contains information about our file that is being received from the server.
	SOCKET sConnection;//This client's connection to the server
	SOCKADDR_IN addr; //Address to be binded to our Connection socket
	bool connected;
};


static Client * clientptr;	//This client ptr is necessary so that the ClientThread method can access the Client instance/methods. 
							//Since the ClientThread method is static, this is the simplest workaround I could think of since there will only be one instance of the client.