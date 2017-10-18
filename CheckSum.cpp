#include <stdio.h>
#include <iostream>
#include "CheckSum.h"
CheckSum::CheckSum(unsigned char* inputPacket) {
	packet = new unsigned char[sizeof(inputPacket)];
	for (int i = 0; i < sizeof(inputPacket); i++) {
		packet[i] = inputPacket[i];
	}
}
void CheckSum::BuildCheckSum() {
	int i;
	checkSum = 0;
	for (i=0;i<sizeof(packet);i++) {
		checkSum = checkSum + packet[i];
	}
}

bool CheckSum::CheckSumValidation(unsigned char* inputPacket) {
	int i;
	unsigned char tempCheckSum;
	tempCheckSum = 0;
	for (i=0;i<sizeof(inputPacket);i++) {
		tempCheckSum = tempCheckSum + inputPacket[i];
	}
	return (tempCheckSum == inputPacket[sizeof(inputPacket)]);
}
unsigned char CheckSum::getCheckSum() {
	return (checkSum);
}
