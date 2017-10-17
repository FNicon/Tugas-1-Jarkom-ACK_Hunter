#ifndef _PACKET_
#define _PACKET_
#include "macros.h"
#include "CheckSum.h"
#include <string.h>
#include <stdint.h>

class Packet {
private:
	uint32_t frameNumber;
	int dataSize;
	char* data;
	unsigned char* packetMessage;

public:
	Packet(uint32_t frameNumber, char* data);
	Packet(const Packet& packet);
	Packet& operator=(const Packet& packet);
	~Packet();

	void setFrameNumber(uint32_t number);
	uint32_t getFrameNumber();

	void setData(char* data);
	char* getData();

	int getSize();

	void setChecksum(char checkSum);
	char getChecksum();

	void getMsg();
	unsigned char* getPacketMessage();
	int getMsgSize();
	void printMsg();
};

#endif