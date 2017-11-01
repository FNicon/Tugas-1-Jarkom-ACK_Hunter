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
bool succesSentHandshake;
bool succesReceiveHandshake;
int ackCount = 0;
socklen_t slen = sizeof(otherAddr);

void sendAck(int sock, unsigned char* recvData, int maxSequence) {
	int recvSeq = 0;
	recvSeq = (recvSeq ^ recvData[1]) << 8;
	recvSeq = (recvSeq ^ recvData[2]) << 8;
	recvSeq = (recvSeq ^ recvData[3]) << 8;
	recvSeq = (recvSeq ^ recvData[4]);
	Ack reply(recvData, recvSeq);
	reply.printAck();
	// std::cout << "sigh... " << recvSeq << std::endl;
	if (sendto(sock, reply.getAck(), sizeof(reply.getAck()), 0, (struct sockaddr*) &otherAddr, sizeof(otherAddr)) == -1){
		std::cout << "Gagal mengirim ack ke-?" << std::endl;
		exit(1);
	}
}
// void writeToFile(char* bufferToWrite, std::ofstream targetFile, ) {
// 	for (int i = 0; i < )
// }

int handshake(int thisSocket, const int* RWS) {
	int SWS = 0;

	fflush(stdout);
	if (recvfrom(thisSocket, &SWS, sizeof(SWS), 0, (struct sockaddr *) &otherAddr, &slen) == -1){
		std::cout << "Failed to receive handshake" << std::endl;
	} else {
		succesReceiveHandshake = true;
	}

	if (sendto(thisSocket, RWS, sizeof(RWS), 0 , (struct sockaddr *) &otherAddr, sizeof(otherAddr))==-1) {
		std::cout << "Failed to send handshake reply" << std::endl;
	} else {
		succesSentHandshake = true;
	}

	return (SWS + *RWS);
}
void randomSleep() {
	int randomTime;
	randomTime = rand() % 20000;
	usleep(randomTime);
}

int main(int argc, char* argv[]) {
	if (argc != 5) {
		std::cout << "Usage : ./recvfile <file name> <window size> <buffer size> <port>" << std::endl;
		return 0;
	} else {
		succesSentHandshake = false;
		succesReceiveHandshake = false;
		struct timeval timeoutVal;
		timeoutVal.tv_sec = 2;
		timeoutVal.tv_usec = 0;
		int timeoutCount = 0;
		char* fileName = argv[1];
		const int windowSize = atoi(argv[2]);
		const int bufferSize = atoi(argv[3]);
		const int myPort = atoi(argv[4]);
		std:: cout << fileName << " " << windowSize << " " << bufferSize << " " << myPort << std::endl;

		bool finished = false;

		int mySocket;
		int recvlen;
		int bufferPtr = 0;

		char bufferToWrite[bufferSize];
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

		if (setsockopt(mySocket, SOL_SOCKET, SO_RCVTIMEO, &timeoutVal, sizeof(timeoutVal))<0) {
			std::cout << "Failed to set timeout!" << std::endl;
			return 0;
		}

		std::ofstream fout;
		fout.open(fileName);
		int maxSequence = 0;
		while (!(succesSentHandshake && succesReceiveHandshake)) {
			std::cout << "Waiting for handshake..." << std::endl;
			maxSequence = handshake(mySocket, &windowSize);
		// std::cout << "RWS: " << maxSequence << std::endl;
			sleep(1);
		}
		char tempBuffer[windowSize];
		for (int initCount=0;initCount<windowSize;initCount++) {
			tempBuffer[initCount] = 0;
		}
		int counterReceived = 0;
		int tempCounter = 0;
		while (!finished) {
			std::cout << "Waiting for data..." << std::endl;
			fflush(stdout);
			memset(recvData, 0, bufferSize);
			if (timeoutCount >= 5) {
				finished = true;
			} else if ((recvfrom(mySocket, recvData, bufferSize, 0, (struct sockaddr *) &otherAddr, &slen)) == -1) {
				std::cout << "Failed to receive data" << std::endl;
				timeoutCount++;
				continue;
			} else {
				int currentSeq = 0;
				printf("=============================\n");
				printf("[main] Menerima paket dari %s:%d\n", inet_ntoa(otherAddr.sin_addr), ntohs(otherAddr.sin_port));
				printf("[main] Data (hex): %x\n" ,recvData[6]);

				CheckSum packetChecker(recvData);
				printf ("[main] received package content (hex): %x %x %x %x %x %x %x %x %x\n", recvData[0], recvData[1], recvData[2], recvData[3], recvData[4], recvData[5], recvData[6], recvData[7], recvData[8]);
				//usleep(20000);
				randomSleep();
				if (packetChecker.CheckSumValidation(recvData)) {
					currentSeq = (currentSeq ^ recvData[1]) << 8;
					currentSeq = (currentSeq ^ recvData[2]) << 8;
					currentSeq = (currentSeq ^ recvData[3]) << 8;
					currentSeq = (currentSeq ^ recvData[4]);
					if (currentSeq >= windowSize) {
						tempCounter = currentSeq - windowSize; 
					} else {
						tempCounter = currentSeq;
					}
					tempBuffer[tempCounter] = recvData[6];
					counterReceived = counterReceived + 1;
					printf("%d, Data Kirim : %c\n",currentSeq, recvData[6]);
					sendAck(mySocket, recvData, maxSequence);
					//bufferToWrite[bufferPtr] = recvData[6];
					//std::cout << "[bufferRead] buffer receiver : ";
					//for (int i = 0; i<=bufferPtr; i++) {
					//	std::cout << bufferToWrite[i] << " ";
					//}
					//std::cout<<std::endl;
					//bufferPtr++;
				} else {
					std::cout << "[main] Received Package content was corrupt." << std::endl;
				}
				if (counterReceived >= windowSize) {
					printf("pointer %d\n",bufferPtr);
					printf("=============counter : %d\n",counterReceived);
					for (int j=bufferPtr;j<bufferPtr+counterReceived;j++) {
						printf("%c ITERASI : %d\n",tempBuffer[j-bufferPtr], j-bufferPtr);
						bufferToWrite[j] = tempBuffer[j-bufferPtr];
					}
					std::cout << "[bufferRead] buffer receiver : ";
					for (int i = 0; i<bufferPtr+counterReceived; i++) {
						std::cout << bufferToWrite[i] << " ";
					}
					std::cout<<std::endl;
					bufferPtr=bufferPtr+counterReceived;
					counterReceived = 0;
				}
				if (bufferPtr >= bufferSize) {
					std::cout << "[bufferWrite] buffer to Write : ";
					for (int i = 0; i < bufferSize; i++) {
						fout << bufferToWrite[i];
						std::cout << bufferToWrite[i] << " ";
					}
					std::cout<<std::endl;
					memset(bufferToWrite, 0, bufferSize);
					bufferPtr = 0;
				}
			}
			if (finished) {
				int i = 0;
				std::cout << "[bufferWrite] buffer to Write : ";
				while (bufferToWrite[i] != 0) {
					std::cout << bufferToWrite[i] << " ";
					fout << bufferToWrite[i];
					i++;
				}
				std::cout<<std::endl;
				std::cout << "[main] No data sent. closing socket now..." << std::endl;
			}
			//std::cout << "loop entered but no condition met.." << std::endl;
		}

		fout.close();
		close(mySocket);
		return 0;
	}
}
