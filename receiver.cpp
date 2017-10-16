#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <fstream>
#include <vector>
#include <unistd.h>
#include "Ack.h"
#include "CheckSum.h"

struct sockaddr_in myAddr, otherAddr;
int ackCount = 0;

void sendAck(int sock, unsigned char* recvData) {
	Ack reply(recvData, ++ackCount);
std::cout << "Here" << std::endl;
	 if (sendto(sock, reply.getAck(), sizeof(reply.getAck()), 0, (struct sockaddr*) &otherAddr, sizeof(otherAddr)) == -1)
    {
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
		unsigned char recvData[bufferSize];

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
        	printf("[main] Data: %c\n" , (char)recvData[6]);

        	CheckSum packetChecker(recvData);

        	std::cout << "[main] Buffer ptr before: " << bufferPtr << std::endl;
        	if (packetChecker.CheckSumValidation()) {
        		std::cout << "[main] sizeof recvData: " << sizeof(recvData) << std::endl;
        		printf ("[sendSinglePacket] readable format: %x %x %x %x %x %x %x %x %x\n", recvData[0], recvData[1], recvData[2], recvData[3], recvData[4], recvData[5], recvData[6], recvData[7], recvData[8]);
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