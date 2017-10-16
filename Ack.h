#ifndef _ACK_
#define _ACK_
#include "macros.h"
#include "CheckSum.h"
#include <string.h>

class Ack {
private:
	char status;
	int nextSeq;
	char advWindow;
	// char* receivedPacket;

public:
	Ack(unsigned char* receivedFrame);
	Ack(unsigned char* receivedFrame, int sequence);

	char getStatus();

	// char* getRecvPacket();
	char extractData();
	unsigned char* getAck();
	void printAck();
};

#endif