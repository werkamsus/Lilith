#include "Packet.h"
#include <Windows.h>

Packet::Packet()
{

}

Packet::Packet(char * _buffer, int _size)
{
	buffer = _buffer;
	size = _size;
}

Packet::Packet(const Packet & p) //Allocate new block for buffer
{
	size = p.size;
	buffer = new char[size];
	memcpy(buffer, p.buffer, size);
}