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

#define PORT 8888

struct sockaddr_in otherAddress;
socklen_t slen = sizeof(otherAddress);

int sendSinglePacket(int thisSocket, Packet packet) {
	char ack[7];

	if (sendto(thisSocket, packet.getMsg(), packet.getMsgSize(), 0 , (struct sockaddr *) &otherAddress, sizeof(otherAddress))==-1) {
		std::cout << "Failed to send packet number-" << packet.getFrameNumber() << std::endl;
		std::cout << "Package: " << packet.getMsg() << std::endl;
	}

	if (recvfrom(thisSocket, ack, 7, 0, (struct sockaddr *) &otherAddress, &slen) == -1){
		std::cout << "Failed to receive ack-" << std::endl;
	}

	std::cout << "Next sequence: " << ack << std::endl << std::endl;
}

void sendData(int thisSocket, char* data, int dataLength, int windowSize, int payloadSize) {
	bool acked = false;

	int windowPtr = 0;
	int bufferPtr = 0;
	int sequence = 0;
	int expectedAck = 0;
	int maxSequence = windowSize * 2;
	int lastAckRecv;
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
		sleep(1);

		windowPtr += payloadSize;
		sequence++;
	}

}

int main(int argc, char* argv[]) {
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

		memset((char*) &otherAddress, 0, sizeof(otherAddress));
		otherAddress.sin_family = AF_INET;
		otherAddress.sin_port = htons(destPort);
		if (inet_aton(address , &otherAddress.sin_addr) == 0) {
	        std::cout << "Failed to write destination address" << std::endl;
	        exit(1);
	    }

		int iterasi = 1;
		while (!fin.eof()) {
			int dataLength = 0;
			std::cout << "Iterasi " << iterasi << std::endl;
			int i = 0;
			while (i < bufferSize && !fin.eof()) {
				fin >> std::noskipws >> buffer[i];
				dataLength++;
				i++;
			}

			sendData(thisSocket, buffer, dataLength, windowSize, 1);
			memset(buffer, 0, bufferSize);
			iterasi++;
		}

		fin.close();
		std::cout << "Selesai!" << std::endl;
		close(thisSocket);
		return 0;
	}
}