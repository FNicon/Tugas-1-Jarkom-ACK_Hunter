#include <stdio.h>
#include <iostream>
#include "CheckSum.h"

CheckSum::CheckSum(char* inputPacket) {
	int i;
	packet = new char[strlen(inputPacket)];
	for (i=0;i<strlen(inputPacket);i++) {
		packet[i] = inputPacket[i];
	}
}
void CheckSum::BuildCheckSum() {
	int i;
	checkSum = 0;
	for (i=0;i<strlen(packet);i++) {
		//printf("%x \n", inputString[i]);
		//printHextoBit(inputString[i]);
		checkSum = checkSum + packet[i];
	}
	if (checkSum > 255) {
		checkSum = 0;
		for (i=0;i<strlen(packet);i++) {
			//printf("%x \n", inputString[i]);
			//printHextoBit(inputString[i]);
			checkSum = checkSum + ~packet[i];
		}	
	}
}
bool CheckSum::CheckSumValidation(char* inputPacket) {
	int i;
	char tempCheckSum;
	tempCheckSum = 0;
	for (i=0;i<strlen(inputPacket) - 1;i++) {
		tempCheckSum = tempCheckSum + inputPacket[i];
	}
	if (tempCheckSum > 255) {
		tempCheckSum = 0;
		for (i=0;i<strlen(inputPacket) - 1;i++) {
			tempCheckSum = tempCheckSum + ~inputPacket[i];
		}
	}
	return (tempCheckSum == inputPacket[strlen(inputPacket)-1]);
}
char CheckSum::getCheckSum() {
	return (checkSum);
}
/*void CheckSum::printHextoBit(char a) {
	int mask = 0x80;
	while (mask>0) {
		printf("%d", (a & mask) > 0 );
		mask >>= 1;
	}
	printf("\n");
}*/