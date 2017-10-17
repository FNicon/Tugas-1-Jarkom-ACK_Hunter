#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <fstream>
#include <vector>
#include <unistd.h>
#include "CheckSum.h"

struct sockaddr_in myAddr, otherAddr;
int ackCount = 0;

// void sendAck(int sock, char* recvData) {
// 	Ack reply(recvData, ++ackCount);

// 	 if (sendto(sock, reply.getAck(), 7, 0, (struct sockaddr*) &otherAddr, sizeof(otherAddr)) == -1)
//     {
//         std::cout << "Gagal mengirim ack ke-?" << std::endl;
//         exit(1);
//     }
// }

void sendAck(int sock, char* recvData) {
	ack reply;
	int sequence = 0;
	sequence = (sequence ^ recvData[1]) << 24;
	sequence = (sequence ^ recvData[2]) << 16;
	sequence = (sequence ^ recvData[3]) << 8;
	sequence = (sequence ^ recvData[4]);
	sequence++;

	reply.verdict = GOOD_ACK;
	reply.nextSeq[0] = (sequence & 0xFF000000) >> 24;
	reply.nextSeq[1] = (sequence & 0xFF0000) >> 16;
	reply.nextSeq[2] = (sequence & 0xFF00) >> 8;
	reply.nextSeq[3] = (sequence & 0xFF);
	reply.advWindow = '0';
	CheckSum check(&reply);
	reply.checksum = check.getCheckSum();

	if (sendto(sock, &reply, sizeof(reply), 0, (struct sockaddr*) &otherAddr, sizeof(otherAddr)) == -1) {
        std::cout << "Gagal mengirim ack ke-?" << std::endl;
        exit(1);
    }
}

// void writeToFile(char* bufferToWrite, std::ofstream targetFile, ) {
// 	for (int i = 0; i < )
// }

int main(int argc, char* argv[]) {
	if (argc != 5) {
		std::cout << "Usage : ./recvfile <file name> <window size> <buffer size> <port>" << std::endl;
		return 0;
	} else {
		char* fileName = argv[1];
		const int windowSize = atoi(argv[2]);
		const int bufferSize = atoi(argv[3]);
		const int myPort = atoi(argv[4]);
		std:: cout << fileName << " " << windowSize << " " << bufferSize << " " << myPort << std::endl;

		bool finished = false;

		int mySocket;
		int recvlen;
		socklen_t slen = sizeof(otherAddr);
		int bufferPtr = 0;

		char buffer[bufferSize];
		char recvData[bufferSize];

		if ((mySocket=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
	        std::cout << "Failed to create socket" << std::endl;
			exit(1);
	    }

		memset((char*) &myAddr, 0, sizeof(myAddr));
		myAddr.sin_family = AF_INET;
		myAddr.sin_port = htons(myPort);
		myAddr.sin_addr.s_addr = htonl(INADDR_ANY);

		if (bind(mySocket, (struct sockaddr*) &myAddr, sizeof(myAddr)) == -1) {
			std::cout << "Failed to bind socket" << std::endl;
			exit(1);
		}

		std::ofstream fout;
		fout.open(fileName);

		while (!finished) {
			std::cout << "Waiting for data..." << std::endl;
			fflush(stdout);

			memset(recvData, 0, bufferSize);

			if ((recvfrom(mySocket, recvData, bufferSize, 0, (struct sockaddr *) &otherAddr, &slen)) == -1) {
				std::cout << "Failed to receive data" << std::endl;
				exit(1);
			}

			printf("[main] Menerima paket dari %s:%d\n", inet_ntoa(otherAddr.sin_addr), ntohs(otherAddr.sin_port));
        	printf("[main] Data: %s\n" , recvData);

        	CheckSum packetChecker(recvData);

        	std::cout << "[main] Buffer ptr before: " << bufferPtr << std::endl;
        	if (packetChecker.CheckSumValidation()) {
        		std::cout << "[main] Menambah buffPtr" << std::endl;
        		std::cout << "[main] TESS: " << strlen(recvData) << std::endl;
        		sendAck(mySocket, recvData);
        		buffer[bufferPtr] = recvData[6];
        		bufferPtr++;
        	}

        	std::cout << "[main] Buffer ptr after: " << bufferPtr << std::endl;
        	std::cout << "[main] bufferSize: " << bufferSize << std::endl;

        	if (recvData[4] == '-') {
        		finished = true;
        		std::cout << "[main] Finished! closing socket now..." << std::endl;
        	}
		}

		fout.close();
		close(mySocket);
		return 0;
	}
}