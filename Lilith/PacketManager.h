#pragma once
#include "Packet.h"
#include <queue>
#include <mutex>

class PacketManager
{
private:
	std::queue<Packet> queue_packets;
	std::mutex mutex_packets;
public:
	bool HasPendingPackets();
	void Append(Packet p);
	Packet Retrieve();
};