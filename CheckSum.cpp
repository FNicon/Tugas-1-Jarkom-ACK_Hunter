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
bool CheckSum::CheckSumValidation() {
	return (true);
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