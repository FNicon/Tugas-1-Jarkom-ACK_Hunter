#include <stdio.h>
#include <iostream>
#include "CheckSum.h"

CheckSum::CheckSum(segment* seg) {
	packet = new char[sizeof(seg)];
	packet[0] = seg->soh;
	packet[1] = seg->stx;
	packet[2] = seg->seqNum[0];
	packet[3] = seg->seqNum[1];
	packet[4] = seg->seqNum[2];
	packet[5] = seg->seqNum[3];
	packet[6] = seg->data;
	packet[7] = seg->etx;
	packet[8] = seg->checksum;
}

CheckSum::CheckSum(ack* ackd) {
	packet = new char[sizeof(ackd)];
	packet[0] = ackd->verdict;
	packet[1] = ackd->nextSeq[0];
	packet[2] = ackd->nextSeq[1];
	packet[3] = ackd->nextSeq[2];
	packet[4] = ackd->nextSeq[3];
	packet[5] = ackd->advWindow;
	packet[6] = ackd->checksum;
}

CheckSum::CheckSum(char* pack) {
	packet = new char[sizeof(pack)];
	for (int i = 0; i < sizeof(pack); i++) {
		packet[i] = pack[i];
	}
}

CheckSum::CheckSum(unsigned char* pack) {
	packet_2 = new unsigned char[sizeof(pack)];
	for (int i = 0; i < sizeof(pack); i++) {
		packet_2[i] = pack[i];
	}
}

// void CheckSum::BuildCheckSum() {
// 	int i;
// 	checkSum = 0;
// 	for (i=0;i<strlen(packet) - 1;i++) {
// 		//printf("%x \n", inputString[i]);
// 		//printHextoBit(inputString[i]);
// 		checkSum = checkSum + packet[i];
// 	}
// 	if (checkSum > 255) {
// 		checkSum = 0;
// 		for (i=0;i<strlen(packet) - 1;i++) {
// 			//printf("%x \n", inputString[i]);
// 			//printHextoBit(inputString[i]);
// 			checkSum = checkSum + ~packet[i];
// 		}	
// 	}
// }

void CheckSum::BuildCheckSum_2() {
	int i;
	checkSum = 0;
	for (i=0;i<sizeof(packet_2) - 1;i++) {
		//printf("%x \n", inputString[i]);
		//printHextoBit(inputString[i]);
		checkSum = checkSum + packet_2[i];
	}
	if (checkSum > 255) {
		checkSum = 0;
		for (i=0;i<sizeof(packet_2) - 1;i++) {
			//printf("%x \n", inputString[i]);
			//printHextoBit(inputString[i]);
			checkSum = checkSum + ~packet_2[i];
		}	
	}
}

bool CheckSum::CheckSumValidation() {
	return (true);
}

// char CheckSum::getCheckSum() {
// 	return (checkSum);
// }

unsigned char CheckSum::getCheckSum_2() {
	return (checkSum_2);
}
/*void CheckSum::printHextoBit(char a) {
	int mask = 0x80;
	while (mask>0) {
		printf("%d", (a & mask) > 0 );
		mask >>= 1;
	}
	printf("\n");
}*/