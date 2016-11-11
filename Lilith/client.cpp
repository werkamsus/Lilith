#include "client.h"

Client* Client::clientptr;
bool Client::connected = false;

void Client::ClientThread()
{
	while (clientptr->connected)
	{
		if (!clientptr->ReceivePacket())
			General::handleError(3, false);

		//process packets
	}

	if (clientptr->CloseConnection()) //Try to close socket connection..., If connection socket was closed properly
	{

	}
	else //If connection socket was not closed properly for some reason from our function
	{
		
	}
}

//test

Client::Client(std::string IP, int PORT)
{
	//Winsock Startup
	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2, 2);
	if (WSAStartup(DllVersion, &wsaData) != 0)
	{
		General::handleError(3, true);
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
	connected = true;
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientThread, NULL, NULL, NULL); //Create the client thread that will receive any data that the server sends.
	return true;
}

bool Client::CloseConnection()
{
	connected = false;
	if (closesocket(sConnection) == SOCKET_ERROR)
	{
		if (WSAGetLastError() == WSAENOTSOCK) //If socket error is that operation is not performed on a socket (This happens when the socket has already been closed)
		{
			WSACleanup();
			return true; //return true since connection has already been closed
		}
		return false;
	}
	WSACleanup();
	return true;
}