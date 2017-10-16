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

void sendAck(int sock, char* recvData) {
	Ack reply(recvData, ++ackCount);

	 if (sendto(sock, reply.getAck(), 7, 0, (struct sockaddr*) &otherAddr, sizeof(otherAddr)) == -1)
    {
        std::cout << "Gagal mengirim ack ke-?" << std::endl;
        exit(1);
    }
}

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

		int mySocket;
		int recvlen;
		socklen_t slen = sizeof(otherAddr);

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

		while (1) {
			std::cout << "Waiting for data..." << std::endl;
			fflush(stdout);

			memset(buffer, 0, bufferSize);
			memset(recvData, 0, bufferSize);

			if ((recvfrom(mySocket, recvData, bufferSize, 0, (struct sockaddr *) &otherAddr, &slen)) == -1) {
				std::cout << "Failed to receive data" << std::endl;
				exit(1);
			}

			printf("Menerima paket dari %s:%d\n", inet_ntoa(otherAddr.sin_addr), ntohs(otherAddr.sin_port));
        	printf("Data: %x\n" , recvData);

        	CheckSum packetChecker(recvData);

        	//if (packetChecker.CheckSumValidation(recvData)) {
				sendAck(mySocket, recvData);
        	//}
		}

		fout.close();
		close(mySocket);
		return 0;
	}
}