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
// struct sockaddr_in thisAddress;

// void fillBuffer(char* buffer, int& offset, int bufferSize) {
	
// }

// void checkTimeOut() {
// 	while (true) {
		
// 	}
// }

int sendSinglePacket(int thisSocket, Packet packet) {
	if (sendto(thisSocket, packet.getMsg(), packet.getMsgSize(), 0 , (struct sockaddr *) &otherAddress, sizeof(otherAddress))==-1) {
		std::cout << "Failed to send packet number-" << packet.getFrameNumber() << std::endl;
		std::cout << "Package: " << packet.getMsg() << std::endl;
	}
}

void sendData(int thisSocket, char* data, int windowSize, int payloadSize) {
	bool acked = false;

	int windowPtr = 0;
	int bufferPtr = 0;
	int sequence = 0;
	int expectedAck = 0;
	int maxSequence = windowSize * 2;
	int lastAckRecv;

	while (windowPtr < windowPtr + windowSize * payloadSize && windowPtr < strlen(data)) {
		if (sequence >= maxSequence) {
			sequence = 0;
		}
		std::cout << "panjang data: " << strlen(data) << std::endl;
		// std::cout << "Curent buffer pointer: " << bufferPtr << std::endl;

		char* payload = new char(payloadSize);
		for (int j = 0; j < payloadSize && j < strlen(data); j++) {
			payload[j] = data[bufferPtr];
			bufferPtr++;
		}
		// std::cout << "Current data: " << payload << std::endl;
		// std::cout << "Current window pointer: " << windowPtr << std::endl << std::endl;
		sendSinglePacket(thisSocket, *(new Packet(sequence, payload)));
		// sleep(1);
		delete [] payload;

		windowPtr += payloadSize;
		sequence++;
	}

}

void checkAck(int thisSocket) {

}

int main(int argc, char* argv[]) {
	if (argc != 6) {
		std::cout << "Usage : ./sender <file name> <window size> <buffer size> <destination ip> <destination port>" << std::endl;
		return 0;
	} else {
		char ch;
		char* fileName = argv[1];
		const int windowSize = atoi(argv[2]);
		const int bufferSize = atoi(argv[3]);
		const char* address = (argv[4]);
		const int destPort = atoi(argv[5]);
		std:: cout << fileName << " " << windowSize << " " << bufferSize << " " << address << " " << destPort << std::endl;

		char* buffer = new char(bufferSize);

		int thisSocket;
		int socketSize = sizeof(otherAddress);
		int bufferOffset = 0;

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


		// memset((char*) &thisAddress, 0, sizeof(thisAddress));
		// thisAddress.sin_family = AF_INET;
		// thisAddress.sin_port = htons(PORT);
		// if (inet_aton(address , &thisAddress.sin_addr) == 0) {
	 //        std::cout << "Failed to write destination address" << std::endl;
	 //        exit(1);
	 //    }

		// if (bind(thisSocket, (struct sockaddr*) &thisAddress, sizeof(thisAddress)) == -1) {
		// 	std::cout << "Failed to bind socket to a port" << std::endl;
		// 	return 0;
		// }

	    if (sendto(thisSocket, "abcde", 5, 0 , (struct sockaddr *) &otherAddress, sizeof(otherAddress))==-1) {
			std::cout << "Failed" << std::endl;
		}
		int offset = 0;
		int iterasi = 1;
		while (!fin.eof()) {
			std::cout << "Iterasi " << iterasi << std::endl;
			int i = 0;
			while (i < bufferSize && !fin.eof()) {
				fin >> std::noskipws >> buffer[i];
				offset++;
				i++;
			}

			// std::cout << "buffer:" << std::endl;
			// std::cout << buffer << std::endl;
			// std::cout << "enter:" << std::endl;
			// std:: cin >> ch;
			sendData(thisSocket, buffer, windowSize, 1);
			memset(buffer, 0, strlen(buffer));
			iterasi++;
		}

		fin.close();
		std::cout << "Di sini!" << std::endl;
		close(thisSocket);
		return 0;
	}
}