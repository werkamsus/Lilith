#include "PacketManager.h"

bool PacketManager::HasPendingPackets()
{
	return (queue_packets.size() > 0); //returns true if size > 0 false otherwise
}

void PacketManager::Append(Packet p)
{
	std::lock_guard<std::mutex> lock(mutex_packets); //lock mutex so two threads dont access data at same time
	queue_packets.push(p); //Add packet to queue
}

Packet PacketManager::Retrieve()
{
	std::lock_guard<std::mutex> lock(mutex_packets); //lock mutex so two threads dont access data at same time
	Packet p = queue_packets.front(); //Get packet from front of queue
	queue_packets.pop(); //Remove packet from front of queue
	return p; //Return packet that was removed from the queue
}