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

public:
	Ack(char* receivedFrame);
	Ack(char* receivedFrame, int sequence);

	char getStatus();

	char* getAck();
	void printAck();
};

#endif