#include <stdio.h>
#include <iostream>
#include "Packet.h"

Packet::Packet(uint32_t frameNumber, char* data) {
	this->frameNumber = frameNumber;
	this->dataSize = strlen(data);
	this->data = new char[dataSize];
	for (int i = 0; i < dataSize; i++) {
		this->data[i] = data[i];
	}
}

Packet::Packet(const Packet& packet) {
	this->frameNumber = packet.frameNumber;
	this->dataSize = strlen(packet.data);//sizeof(packet.data)/sizeof(packet.data[0]);
	this->data = new char(dataSize);
	for (int i = 0; i < dataSize; i++) {
		this->data[i] = packet.data[i];
	}
}

Packet& Packet::operator=(const Packet& packet) {
	this->frameNumber = packet.frameNumber;
	this->dataSize = strlen(packet.data);
	delete [] data;
	this->data = new char(dataSize);
	for (int i = 0; i < dataSize; i++) {
		data[i] = packet.data[i];
	}

	return *this;
}

Packet::~Packet() {
	delete [] data;
}

void Packet::setFrameNumber(uint32_t number) {
	this->frameNumber = number;
}
uint32_t Packet::getFrameNumber() {
	return frameNumber;
}

void Packet::setData(char* newData) {
	delete [] data;
	dataSize = sizeof(newData)/sizeof(newData[0]);
	data = new char(dataSize);

	for (int i = 0; i < dataSize; i++) {
		data[i] = newData[i];
	}
}
char* Packet::getData() {
	return data;
}

int Packet::getSize() {
	return dataSize;
}

unsigned char* Packet::getMsg() {
	unsigned char* message = new unsigned char[1 + 4 + 1+ dataSize + 1 + 1];
		message[0] = SOH;
		message[1] = frameNumber >> 24;
		message[2] = frameNumber >> 16;
		message[3] = frameNumber >> 8;
		message[4] = frameNumber >> 0;
		message[5] = STX;
		message[6] = data[0];
		message[7] = ETX;
		message[8] = 0;
		CheckSum check(message);
		message[8] = check.getCheckSum_2();


		// sprintf (message, "%c%c%c%c%c", SOH, (frameNumber & 0xff000000) >> 24, (frameNumber & 0xff0000) >> 16, (frameNumber & 0xff00) >> 8, (frameNumber & 0xff));
		// // if (frameNumber > 999) {
		// // 	sprintf(message,"%c%d", SOH, frameNumber);
		// // } else if (frameNumber > 99) {
		// // 	sprintf(message,"%c%s%d", SOH, "0", frameNumber);
		// // } else if (frameNumber > 9) {
		// // 	sprintf(message,"%c%s%d", SOH, "00", frameNumber);
		// // } else {
		// // 	sprintf(message,"%c%s%d", SOH, "000", frameNumber);
		// // }

		// sprintf(message, "%s%c%c%c", message, STX, data[0], ETX);

		// check.BuildCheckSum();
		// sprintf(message,"%s%c",message,check.getCheckSum());
		return message;
}

int Packet::getMsgSize() {
	return (1 + 4 + 1 + dataSize + 1 + 1);
}

void Packet::printMsg() {
	std::cout << this->getMsg() << std::endl;
}