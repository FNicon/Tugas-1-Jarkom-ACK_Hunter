#ifndef _ACK_
#define _ACK_
#include "macros.h"
#include <string.h>

class Ack {
private:
	char status;
	int nextSeq;
	char advWindow;
	char checkSum;

public:
	Ack(char* receivedFrame);
	Ack(const Ack& copy);
	Ack& operator=(const Ack& copy);
	~Ack();

	char getStatus();

	char* getAck();
	void printAck();
};

#endif