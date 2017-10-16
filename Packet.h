#ifndef _PACKET_
#define _PACKET_
#include "macros.h"
#include "CheckSum.h"
#include <string.h>

class Packet {
private:
	int frameNumber;
	int dataSize;
	char* data;
	char checkSum;

public:
	Packet(int frameNumber, char* data);
	Packet(const Packet& packet);
	Packet& operator=(const Packet& packet);
	~Packet();

	void setFrameNumber(int number);
	int getFrameNumber();

	void setData(char* data);
	char* getData();

	int getSize();

	void setChecksum(char checkSum);
	char getChecksum();

	char* getMsg();
	int getMsgSize();
	void printMsg();
};

#endif