#pragma once

#ifndef PACKET_H
#define PACKET_H


class Packet
{
public:
	Packet();
	Packet(char * buffer, int size); //Will use existing allocated buffer and create packet from it
	Packet(const Packet & p); //Will allocate new buffer but copy buffer from packet argument
	int size;
	char * buffer;
};


#endif // !PACKET_H
