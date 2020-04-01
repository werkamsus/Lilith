#pragma once
#ifndef PACKETSTRUCTS_H
#define PACKETSTRUCTS_H

#include "PacketType.h"
#include "Packet.h"
#include <string>

namespace PS //Packet Structures Namespace
{
	class Message
	{
	public:
		Message(std::string);
		Packet toPacket(PacketType _packettype); //Converts ChatMessage to packet
	private:
		std::string message;
	};

}

#endif