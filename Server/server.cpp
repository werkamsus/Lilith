#include "server.h"

Server::Server(int PORT, bool BroadcastPublically)
{
	//Winsock Startup
	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2, 1);
	if (WSAStartup(DllVersion, &wsaData) != 0) //If WSAStartup returns anything other than 0, then that means an error has occured in the WinSock Startup.
	{
		MessageBoxA(NULL, "WinSock startup failed", "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}

	if (BroadcastPublically) //If server is open to public
		addr.sin_addr.s_addr = htonl(INADDR_ANY);
	else //If server is only for our router
		addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //Broadcast locally
	addr.sin_port = htons(PORT); //Port
	addr.sin_family = AF_INET; //IPv4 Socket

	sListen = socket(AF_INET, SOCK_STREAM, NULL); //Create socket to listen for new connections

	if (bind(sListen, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR) //Bind the address to the socket, if we fail to bind the address..
	{
		std::string ErrorMsg = "Failed to bind the address to our listening socket. Winsock Error:" + std::to_string(WSAGetLastError());
		MessageBoxA(NULL, ErrorMsg.c_str(), "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}

	if (listen(sListen, SOMAXCONN) == SOCKET_ERROR) //Places sListen socket in a state in which it is listening for an incoming connection. Note:SOMAXCONN = Socket Oustanding Max connections, if we fail to listen on listening socket...
	{
		std::string ErrorMsg = "Failed to listen on listening socket. Winsock Error:" + std::to_string(WSAGetLastError());
		MessageBoxA(NULL, ErrorMsg.c_str(), "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}

	serverptr = this;
	//CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)PacketSenderThread, NULL, NULL, NULL); //Create thread that will manage all outgoing packets
}

bool Server::listenForNewConnection()
{
	SOCKET newConnection = accept(sListen, (SOCKADDR*)&addr, &addrlen); //Accept a new connection
	if (newConnection == 0) //If accepting the client connection failed
	{
		std::cout << "Failed to accept the client's connection." << std::endl;
		return false;
	}
	else //If client connection properly accepted
	{
		std::cout << "Client Connected! ID:" << TotalConnections << std::endl;
		Connections[TotalConnections] = newConnection; //Set socket in array to be the newest connection before creating the thread to handle this client's socket.
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandlerThread, (LPVOID)(TotalConnections), NULL, NULL); //Create Thread to handle this client. The index in the socket array for this thread is the value (i).
		TotalConnections += 1; //Incremenent total # of clients that have connected
		return true;
	}
}

enum Server::PacketType
{
	P_Instruction,
	P_CMDCommand,
	P_Error
};


bool Server::ProcessPacket(int ID)
{
	General::outputMsg("recving packet", P_CMDCommand);
	PacketType packettype;
	recv(Connections[ID], (char*)&packettype, sizeof(PacketType), NULL);	//receive packet type
	General::outputMsg("recvd packettype", P_Instruction);

	int bufferlength;
	recv(Connections[ID], (char*)&bufferlength, sizeof(int), NULL);		//receive packet length
	General::outputMsg("recvd length: " + bufferlength, P_Instruction);
	char* buffer = new char[bufferlength + 1];	//Allocate buffer
	buffer[bufferlength] = '\0';	//Set last character of buffer to be a null terminator so we aren't printing memory that we shouldn't be looking at
	recv(Connections[ID], buffer, bufferlength, NULL);	//receive message
	General::outputMsg("recvd msg: " + std::string(buffer), P_Instruction);

	switch (packettype)
	{
	case PacketType::P_Instruction:
	{
		General::outputMsg(buffer, 1);
		break;
	}
	case PacketType::P_CMDCommand:
	{
		General::outputMsg(buffer, 3);
		break;
	}
	default:
		delete[] buffer;
		return false;
	}

	delete[] buffer;
	return true;
}

bool Server::sendPacket(int ID, std::string message, PacketType _PacketType)
{
	if (send(Connections[ID], (char*)&_PacketType, sizeof(PacketType), NULL) == SOCKET_ERROR)
	{
		General::outputMsg("Error sending Packet: Error sending Type", 1);
		return false;
	}
	int bufferlength = message.size();
	if (send(Connections[ID], (char*)&bufferlength, sizeof(int), NULL) == SOCKET_ERROR)
	{
		General::outputMsg("Error sending Packet: Error sending size", 1);
		return false;
	}
	if (send(Connections[ID], message.c_str(), bufferlength, NULL) == SOCKET_ERROR)
	{
		General::outputMsg("Error sending Packet: Error sending message", 1);
		return false;
	}
	return true;

}

void Server::ClientHandlerThread(int ID)
{
	while (true)
	{
		if (!serverptr->ProcessPacket(ID)) //Get packet type
			break; //If there is an issue getting the packet type, exit this loop
	}
	std::cout << "Lost connection to client ID: " << ID << std::endl;
	closesocket(serverptr->sListen);
}
