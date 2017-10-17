#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <fstream>
#include <vector>
#include <unistd.h>
#include "Packet.h"
#include "CheckSum.h"
#include <time.h>

#define PORT 8888

struct sockaddr_in otherAddress;
socklen_t slen = sizeof(otherAddress);



int sendSinglePacket(int thisSocket, Packet packet) {
	unsigned char ack[7];
	int nextSeq = -1;
	if (sendto(thisSocket, packet.getMsg(), sizeof(packet.getMsg()), 0 , (struct sockaddr *) &otherAddress, sizeof(otherAddress))==-1) {
		std::cout << "Failed to send packet number-" << packet.getFrameNumber() << std::endl;
	}

	// fflush(stdout);
	// if (recvfrom(thisSocket, ack, 7, 0, (struct sockaddr *) &otherAddress, &slen) == -1){
	// 	std::cout << "Failed to receive ack-" << std::endl;
	// }

	packet.printMsg();
	nextSeq = (nextSeq ^ ack[1]) << 8;
	// printf("[sendSinglePacket] Satu: %x\n", ack[1]);

	nextSeq = (nextSeq ^ ack[2]) << 8;
	// printf("[sendSinglePacket] Dua: %x\n", ack[2]);

	nextSeq = (nextSeq ^ ack[3]) << 8;
	// printf("[sendSinglePacket] Tiga: %x\n", ack[3]);

	nextSeq = (nextSeq ^ ack[4]);
	// printf("[sendSinglePacket] Empat: %x\n", ack[4]);

	return nextSeq;
}


int handshake(int thisSocket, const int* SWS) {
	if (sendto(thisSocket, SWS, sizeof(SWS), 0 , (struct sockaddr *) &otherAddress, sizeof(otherAddress))==-1) {
		std::cout << "Failed to send handshake" << std::endl;
		exit(0);
	}

	int RWS = 0;
	if (recvfrom(thisSocket, &RWS, sizeof(RWS), 0, (struct sockaddr *) &otherAddress, &slen) == -1){
		std::cout << "Failed to receive handshake reply" << std::endl;
		exit(0);
	}

	return (*SWS + RWS + 1);
}


// int sendSingleSegment(int thisSocket, segment* seg) {
// 	char ack[7];
// 	printf ("[sendSingleSegment] readable format: %c%s%c%c%c%c\n", seg->soh, seg->seqNum, seg->stx, seg->data, seg->etx, seg->checksum);

// 	if (sendto(thisSocket, seg, 9, 0 , (struct sockaddr *) &otherAddress, sizeof(otherAddress))==-1) {
// 		std::cout << "Failed to send packet number-" << seg->seqNum << std::endl;
// 	}

// 	if (recvfrom(thisSocket, ack, 7, 0, (struct sockaddr *) &otherAddress, &slen) == -1){
// 		std::cout << "Failed to receive ack-" << std::endl;
// 	}

// 	std::cout << "[sendSingleSegment] Next sequence: " << ack << std::endl << std::endl;
// }

void sendData(int thisSocket, char* data, int dataLength, int windowSize, int payloadSize, int maxSequence) {
	bool acked = false;
	unsigned char ack[7];

	int windowPtr = 0;
	int bufferPtr = 0;
	uint32_t sequence = 0;
	int expectedAck = -1;

	int lastAckRecv = 0;
	int packageCount = 0;

	while (windowPtr < windowPtr + windowSize * payloadSize && windowPtr < dataLength) {
		if (sequence >= maxSequence) {
			sequence = 0;
		}
		std::cout << "Mengirim paket ke-" << ++packageCount << std::endl;
		std::cout << "Sequence paket: " << sequence << std::endl;

		char payload[payloadSize];
		for (int j = 0; j < payloadSize && j < dataLength; j++) {
			payload[j] = data[bufferPtr];
			bufferPtr++;
		}

		std::cout << "Isi paket: " << payload << std::endl;
		sendSinglePacket(thisSocket, *(new Packet(sequence, payload)));
		usleep(800);

		windowPtr += payloadSize;
		sequence++;
	}
}

// void sendAllSegment(int thisSocket, char* data, int dataLength, int windowSize, int payloadSize) {
// 	bool acked = false;

// 	int windowPtr = 0;
// 	int bufferPtr = 0;
// 	int sequence = 0;
// 	int expectedAck = 0;
// 	int maxSequence = windowSize * 2;
// 	int lastAckRecv;
// 	int packageCount = 0;


// 	while (windowPtr < windowPtr + windowSize * payloadSize && windowPtr < dataLength) {
// 		if (sequence >= maxSequence) {
// 			sequence = 0;
// 		}
// 		std::cout << "[sendAllSegment] Mengirim segment ke-" << ++packageCount << std::endl;
// 		std::cout << "[sendAllSegment] Sequence segment: " << sequence << std::endl;

// 		char payload[payloadSize];
// 		for (int j = 0; j < payloadSize && j < dataLength; j++) {
// 			payload[j] = data[bufferPtr];
// 			bufferPtr++;
// 		}

// 		std::cout << "[sendAllSegment] Isi segment: " << payload << std::endl;

// 		segment segmentToSend;
// 		segmentToSend.soh = SOH;
// 		segmentToSend.seqNum[0] = (sequence & 0xFF000000) >> 24;
// 		segmentToSend.seqNum[1] = (sequence & 0xFF0000) >> 16;
// 		segmentToSend.seqNum[2] = (sequence & 0xFF00) >> 8;
// 		segmentToSend.seqNum[3] = (sequence & 0xFF);
// 		segmentToSend.stx = STX;
// 		segmentToSend.data = payload[0];
// 		segmentToSend.etx = ETX;
// 		std::cout << "[sendAllSegment] Sizeof segment: " << sizeof(segmentToSend) << std::endl;
// 		CheckSum check(&segmentToSend);
// 		segmentToSend.checksum = (check.getCheckSum());

// 		sendSingleSegment(thisSocket, &segmentToSend);
// 		usleep(1000);

// 		windowPtr += payloadSize;
// 		sequence++;
// 	}

// }

int main(int argc, char* argv[]) {
	struct timeval timeoutVal;
	timeoutVal.tv_sec = 0;                            //0 secs
	timeoutVal.tv_usec = 100000;                       //100 ms

	if (argc != 6) {
		std::cout << "Usage : ./sendfile <file name> <window size> <buffer size> <destination ip> <destination port>" << std::endl;
		return 0;
	} else {

		char* fileName = argv[1];
		const int windowSize = atoi(argv[2]);
		const int bufferSize = atoi(argv[3]);
		const char* address = (argv[4]);
		const int destPort = atoi(argv[5]);
		std:: cout << fileName << " " << windowSize << " " << bufferSize << " " << address << " " << destPort << std::endl;

		char buffer[bufferSize];

		int thisSocket;
		int socketSize = sizeof(otherAddress);

		std::ifstream fin;
		fin.open(fileName);

		if ((thisSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
			std::cout << "Failed to create socket!" << std::endl;
			return 0;
		}

		if (setsockopt(thisSocket, SOL_SOCKET, SO_RCVTIMEO, &timeoutVal, sizeof(timeoutVal))<0) {
			std::cout << "Failed to set timeout!" << std::endl;
			return 0;
		}

		memset((char*) &otherAddress, 0, sizeof(otherAddress));
		otherAddress.sin_family = AF_INET;
		otherAddress.sin_port = htons(destPort);
		if (inet_aton(address , &otherAddress.sin_addr) == 0) {
	        std::cout << "Failed to write destination address" << std::endl;
	        exit(1);
	    }

	    std::cout << "Sending handshake, waiting for reply..." << std::endl;
	    int maxSequence = handshake(thisSocket, &windowSize);
		// std::cout << "SWS: " << maxSequence << std::endl;
	    sleep(1);
		// {	
		// 	char ch;
		// 	std::cin >> ch;
		// }

		int iterasi = 1;
		while (!fin.eof()) {
			int dataLength = 0;
			std::cout << "[main] Iterasi " << iterasi << std::endl;
			int i = 0;
			while (i < bufferSize && !fin.eof()) {
				fin >> std::noskipws >> buffer[i];
				printf("buffer %d: %c\n", i, buffer[i]);
				dataLength++;
				i++;
			}

			sendData(thisSocket, buffer, dataLength, windowSize, 1, maxSequence);
			// sendAllSegment(thisSocket, buffer, dataLength, windowSize, 1);
			memset(buffer, 0, bufferSize);
			printf("datalength: %d\n", dataLength);
			iterasi++;
		}

		unsigned char EOFmesg[3];
		 EOFmesg[0] = 'E';
		 EOFmesg[1] = 'O';
		 EOFmesg[2] = 'F';

		if (sendto(thisSocket, EOFmesg, sizeof(EOFmesg), 0 , (struct sockaddr *) &otherAddress, sizeof(otherAddress))==-1) {
			std::cout << "Failed to send EOF" << std::endl;
		}

		fin.close();
		std::cout << "Selesai!" << std::endl;
		close(thisSocket);
		return 0;
	}
}