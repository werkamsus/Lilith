#include "PacketStructs.h"
#include <Windows.h>

namespace PS
{
	Message::Message(std::string msg)
	{
		message = msg;
	}

	Packet Message::toPacket(PacketType _packettype)
	{
		const int packetsize = sizeof(int32_t) * 2 + message.size() * sizeof(char); //Calculate total size of buffer for packet contents
		char * buffer = new char[packetsize]; //Create buffer big enough to hold all info for message
		int32_t packettype = htonl((int32_t)_packettype); //Convert packet type (int32_t) to network byte order
		int32_t messagesize = htonl(message.size()); //Convert message size (int32_t) to network byte order
		memcpy(buffer, &packettype, sizeof(int32_t)); //Copy Packet Type to first 4 bytes of buffer
		memcpy(buffer + sizeof(int32_t), &messagesize, sizeof(int32_t)); //Copy size to next 4 bytes of buffer
		memcpy(buffer + sizeof(int32_t) * 2, message.c_str(), message.size() * sizeof(char)); //Copy message to fill the rest of the buffer
		Packet p(buffer, packetsize); //Create packet to be returned
		return p;
	}
}