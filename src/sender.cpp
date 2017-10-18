#include <future>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <thread>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <fstream>
#include <vector>
#include <unistd.h>
#include "Packet.h"
#include "CheckSum.h"

#define PORT 8888
struct sockaddr_in otherAddress;

socklen_t slen = sizeof(otherAddress);
int LAR;
int LFS;
int offeredWindow;


int sendSinglePacket(int thisSocket, Packet packet) {
	unsigned char ack[7];
	unsigned char* packetToSend = new unsigned char(9);
	int nextSeq = 0;
	packet.getMsg();
	for (int i=0;i<=sizeof(packet.getPacketMessage());i++) {
		packetToSend[i] = packet.getPacketMessage()[i];
	}
	if (sendto(thisSocket, packetToSend, sizeof(packetToSend)+1, 0 , (struct sockaddr *) &otherAddress, sizeof(otherAddress))==-1) {
		std::cout << "Failed to send packet number-" << packet.getFrameNumber() << std::endl;
		return 1;
	}

	fflush(stdout);
	if (recvfrom(thisSocket, ack, 7, 0, (struct sockaddr *) &otherAddress, &slen) == -1){
		std::cout << "Failed to receive ack-" << std::endl;
		return -1;
	}
	packet.printMsg();
	printf ("[sendSinglePacket] ack content (hex): %x %x %x %x %x %x %x\n", ack[0], ack[1], ack[2], ack[3], ack[4], ack[5], ack[6]);
	nextSeq = (nextSeq ^ ack[1]) << 8;
	nextSeq = (nextSeq ^ ack[2]) << 8;
	nextSeq = (nextSeq ^ ack[3]) << 8;
	nextSeq = (nextSeq ^ ack[4]);
	if (nextSeq == LAR + 1 /*--> nextSeq == LAR + 1*/) {
		std::cout << "[sendSinglePacket] Next sequence: " << nextSeq << std::endl << std::endl;
		offeredWindow = ack[5];
		LAR = nextSeq;
	} else {
		std::cout << "Ack Number false!" << nextSeq << std::endl << std::endl;
		return -2;
	}
	return 0;
}


int handshake(int thisSocket, const int* SWS) {
	if (sendto(thisSocket, SWS, sizeof(SWS), 0 , (struct sockaddr *) &otherAddress, sizeof(otherAddress))==-1) {
		std::cout << "Failed to send handshake" << std::endl;
	}
	int RWS = 0;
	if (recvfrom(thisSocket, &RWS, sizeof(RWS), 0, (struct sockaddr *) &otherAddress, &slen) == -1){
		std::cout << "Failed to receive handshake reply" << std::endl;
	}
	return (*SWS + RWS + 1);
}
void sendData(int thisSocket, char* data, int dataLength, int windowSize, int payloadSize, int maxSequence) {
	bool acked = false;
	int windowPtr = 0;
	int bufferPtr = 0;
	uint32_t sequence = 0;
	int expectedAck = 0;
	int lastAckRecv;
	int packageCount = 0;
	int errorCode = 0;
	char payload[payloadSize];


	//Tambahan Variabel
	offeredWindow = windowSize;
	LAR = -1; //Last Acknowledgement received
	LFS = -1; //Last Frame Sent

	while (windowPtr < windowPtr + offeredWindow /*--> offeredWindow*/ * payloadSize && windowPtr < dataLength) {
		std::cout << "[moveWindow] Window Pointer: " << windowPtr << " - " << windowPtr + windowSize * payloadSize << std::endl;
		std::vector<std::future<int>> threadRes;
		int tempWindowPtr = windowPtr;
		int tempSequence = sequence;
		int tempPackageCount = packageCount;
		while (tempWindowPtr < windowPtr + offeredWindow /**--> offeredWindow*/ * payloadSize && tempWindowPtr /*Harusnya tempwindow?*/ < dataLength) {
			if (tempSequence >= maxSequence) {
				tempSequence = 0;
			}
			std::cout << "[sendData] Mengirim paket ke-" << tempPackageCount << std::endl;
			std::cout << "[sendData] Sequence paket: " << tempSequence << std::endl;

			for (int j = 0; j < payloadSize && j < dataLength; j++) {
				payload[j] = data[bufferPtr];
				bufferPtr++;
			}
			std::cout << "[sendData] Isi paket: " << payload << std::endl;
			threadRes.push_back(std::async(std::launch::async,sendSinglePacket,thisSocket, *(new Packet(tempSequence, payload))));
			LFS = tempWindowPtr;
			tempWindowPtr += payloadSize;
			tempSequence++;
			tempPackageCount++;
		}
		int checkCounter = 0;
		for (auto& fut : threadRes) {
			++sequence;
			++tempPackageCount;
			++checkCounter;
			if (fut.get()!=0)
				break;
		}
		windowPtr = windowPtr + payloadSize * checkCounter;
		/*
		if (sequence >= maxSequence) {
			sequence = 0;
		}
		std::cout << "[sendData] Mengirim paket ke-" << packageCount << std::endl;
		std::cout << "[sendData] Sequence paket: " << sequence << std::endl;
		if (errorCode == 0) {
			for (int j = 0; j < payloadSize && j < dataLength; j++) {
				payload[j] = data[bufferPtr];
				bufferPtr++;
			}
		}
		std::cout << "[sendData] Isi paket: " << payload << std::endl;
		//errorCode = sendSinglePacket(thisSocket, *(new Packet(sequence,payload)));
		std::future<int> fut = std::async(std::launch::async,sendSinglePacket,thisSocket, *(new Packet(sequence, payload)));
		//std::future<int> fut2 = std::async(std::launch::async,sendSinglePacket,thisSocket, *(new Packet(sequence, payload)));
		errorCode = fut.get();
		//int errorDummy = fut2.get();

		if (errorCode == 0) {
			windowPtr += payloadSize;
			sequence++;
			packageCount++;
		}*/
	}

}
int main(int argc, char* argv[]) {
	struct timeval timeoutVal;
	timeoutVal.tv_sec = 2;
	timeoutVal.tv_usec = 0;

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
		sleep(1);
		int iterasi = 1;
		int i;
		int dataLength;
		while (!fin.eof()){
			dataLength = 0;
			std::cout << "[main] Iterasi " << iterasi << std::endl;
			i = 0;
			std::cout << "[bufferRead] buffer sender : ";
			while (i < bufferSize && !fin.eof()) {
				fin >> std::noskipws >> buffer[i];
				dataLength++;
				i++;
				std::cout << buffer[i] << " ";
			}
			std::cout<<std::endl;
			sendData(thisSocket, buffer, dataLength, windowSize, 1, maxSequence);
			memset(buffer, 0, bufferSize);
			iterasi++;
		}
		//if (fin.eof()) {
			//buffer[i] = 255;
			//dataLength++;
			//i++;
			//sendData(thisSocket, buffer, dataLength, windowSize, 1, maxSequence);
			//memset(buffer, 0, bufferSize);
			//printf("%x\n",buffer[0]);
		//}
		fin.close();
		std::cout << "Selesai!" << std::endl;
		close(thisSocket);
		return 0;
	}
}
