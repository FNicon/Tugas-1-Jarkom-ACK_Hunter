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

	ack = new unsigned char[1 + 4 + 1 + 1];
	nextSeq = 9999;
	advWindow = 'X';

	// std::cout << "[Ack.cpp] nextSeq: "<< nextSeq << std::endl;
	ack[0] = GOOD_ACK;
	ack[1] = (nextSeq & 0xff000000) >> 24;
	// printf("[Ack.cpp] one: %x\n" , ack[1]);
	ack[2] = (nextSeq & 0xff0000) >> 16;
	// printf("[Ack.cpp] two: %x\n" , ack[2]);
	ack[3] = (nextSeq & 0xff00) >> 8;
	// printf("[Ack.cpp] three: %x\n", ack[3]);
	ack[4] = (nextSeq & 0xff) >> 0;
	// printf("[Ack.cpp] four: %x\n", ack[4]);
	ack[5] = advWindow;
	ack[6] = 0;
	CheckSum check(ack);
	check.BuildCheckSum_2();
	ack[6] = check.getCheckSum_2();
}

Ack::Ack(unsigned char* receivedFrame, int sequence) {
	// Set status berdasarkan package yang didapat
	// Set nextSeq berdasarkan package yang didapatkan
	// receivedPacket = new char[strlen(receivedFrame)];
	// for (int i = 0; i < strlen(receivedFrame); i++) {
	// 	receivedPacket[i] = receivedFrame[i];
	// } 

	ack = new unsigned char[1 + 4 + 1 + 1];
	nextSeq = sequence + 1;

	advWindow = 'X';

	// std::cout << "[Ack.cpp] nextSeq: "<< nextSeq << std::endl;
	ack[0] = GOOD_ACK;
	ack[1] = (nextSeq & 0xff000000) >> 24;
	// printf("[Ack.cpp] one: %x\n" , ack[1]);
	ack[2] = (nextSeq & 0xff0000) >> 16;
	// printf("[Ack.cpp] two: %x\n" , ack[2]);
	ack[3] = (nextSeq & 0xff00) >> 8;
	// printf("[Ack.cpp] three: %x\n", ack[3]);
	ack[4] = (nextSeq & 0xff) >> 0;
	// printf("[Ack.cpp] four: %x\n", ack[4]);
	ack[5] = advWindow;
	ack[6] = 0;
	CheckSum check(ack);
	check.BuildCheckSum_2();
	ack[6] = check.getCheckSum_2();
}

char Ack::getStatus() {
	return status;
}

unsigned char* Ack::getAck() {
	

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
	printf("[Ack.cpp] ack content (hex): %x %x %x %x %x %x %x\n", ack[0], ack[1], ack[2], ack[3], ack[4], ack[5], ack[6]);
}