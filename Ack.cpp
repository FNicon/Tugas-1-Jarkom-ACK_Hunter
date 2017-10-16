#include <stdio.h>
#include <iostream>
#include "Ack.h"

Ack::Ack(unsigned char* receivedFrame) {
	// Set status berdasarkan package yang didapat
	// Set nextSeq berdasarkan package yang didapatkan

	// receivedPacket = new char[strlen(receivedFrame)];
	// for (int i = 0; i < strlen(receivedFrame); i++) {
	// 	receivedPacket[i] = receivedFrame[i];
	// } 

	nextSeq = 9999;
	advWindow = 'X';
}

Ack::Ack(unsigned char* receivedFrame, int sequence) {
	// Set status berdasarkan package yang didapat
	// Set nextSeq berdasarkan package yang didapatkan
	// receivedPacket = new char[strlen(receivedFrame)];
	// for (int i = 0; i < strlen(receivedFrame); i++) {
	// 	receivedPacket[i] = receivedFrame[i];
	// } 

	nextSeq = sequence;

	advWindow = 'X';
}

char Ack::getStatus() {
	return status;
}

unsigned char* Ack::getAck() {
	unsigned char* ack = new unsigned char[1 + 4 + 1 + 1];

	ack[0] = SOH;
	ack[1] = nextSeq >> 24;
	ack[2] = nextSeq >> 16;
	ack[3] = nextSeq >> 8;
	ack[4] = nextSeq >> 0;
	ack[5] = advWindow;
	ack[6] = 0;
	CheckSum check(ack);
	check.BuildCheckSum();
	std::cout << "Here" << std::endl;
	ack[6] = check.getCheckSum();

	// sprintf (ack, "%c%d%d%d%c", SOH, (nextSeq & 0xff000000) >> 24, (nextSeq & 0xff0000) >> 16, (nextSeq & 0xff00) >> 8, (nextSeq & 0xff));
	// // if (nextSeq > 999) {
	// // 	sprintf(ack,"%c%d", GOOD_ACK, nextSeq);
	// // } else if (nextSeq > 99) {
	// // 	sprintf(ack,"%c%s%d", GOOD_ACK, "0", nextSeq);
	// // } else if (nextSeq > 9) {
	// // 	sprintf(ack,"%c%s%d", GOOD_ACK, "00", nextSeq);
	// // } else {
	// // 	sprintf(ack,"%c%s%d", GOOD_ACK, "000", nextSeq);
	// // }

	// sprintf(ack, "%s%c", ack, advWindow);
	// CheckSum check = *(new CheckSum(ack));
	// check.BuildCheckSum();
	// sprintf(ack, "%s%c",ack,check.getCheckSum());
	return ack;
}

// char* Ack::getRecvPacket() {
// 	return (receivedPacket);
// }

// char Ack::extractData() {
// 	return (receivedPacket[6]);
// }

void Ack::printAck() {
	std::cout << this->getAck() << std::endl;
}