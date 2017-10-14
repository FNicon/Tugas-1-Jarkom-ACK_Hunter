#include <stdio.h>
#include <iostream>
#include "Ack.h"

Ack::Ack(char* receivedFrame) {
	// Set status berdasarkan package yang didapat
	// Set nextSeq berdasarkan package yang didapatkan

	advWindow = 'X';

	checkSum = 'Y';
}

char Ack::getStatus() {
	return status;
}

char* Ack::getAck() {
	char* ack = new char(1 + 4 + 1 + 1);

	sprintf(ack, "%c%d%c%c", status, nextSeq, advWindow, checkSum);

	return ack;
}

void Ack::printAck() {
	std::cout << this->getAck() << std::endl;
}