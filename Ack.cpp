#include <stdio.h>
#include <iostream>
#include "Ack.h"

Ack::Ack(char* receivedFrame) {
	// Set status berdasarkan package yang didapat
	// Set nextSeq berdasarkan package yang didapatkan

	nextSeq = 9999;

	advWindow = 'X';

	checkSum = 'Y';
}

Ack::Ack(char* receivedFrame, int sequence) {
	// Set status berdasarkan package yang didapat
	// Set nextSeq berdasarkan package yang didapatkan

	nextSeq = sequence;

	advWindow = 'X';

	checkSum = 'Y';
}

char Ack::getStatus() {
	return status;
}

char* Ack::getAck() {
	char* ack = new char[1 + 4 + 1 + 1];

	if (nextSeq > 999) {
		sprintf(ack,"%c%d", GOOD_ACK, nextSeq);
	} else if (nextSeq > 99) {
		sprintf(ack,"%c%s%d", GOOD_ACK, "0", nextSeq);
	} else if (nextSeq > 9) {
		sprintf(ack,"%c%s%d", GOOD_ACK, "00", nextSeq);
	} else {
		sprintf(ack,"%c%s%d", GOOD_ACK, "000", nextSeq);
	}

	sprintf(ack, "%s%c%c", ack, advWindow, checkSum);

	return ack;
}

void Ack::printAck() {
	std::cout << this->getAck() << std::endl;
}