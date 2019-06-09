#pragma once

#ifndef PACKETMANAGER_H
#define PACKETMANAGER_H

#include "Packet.h"
#include <queue>
#include <mutex>

class PacketManager
{
private:
	std::queue<Packet> queue_packets;
	std::mutex mutex_packets;
public:
	void Clear();
	bool HasPendingPackets();
	void Append(Packet p);
	Packet Retrieve();
};

#endif