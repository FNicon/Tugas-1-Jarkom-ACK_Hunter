#include <stdio.h>
#include <iostream>
#include "Packet.h"

Packet::Packet(int frameNumber, char* data) {
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

void Packet::setFrameNumber(int number) {
	this->frameNumber = number;
}
int Packet::getFrameNumber() {
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

char* Packet::getMsg() {
	char* message = new char[1 + 4 + 1+ dataSize + 1 + 1];
	if (frameNumber > 999) {
		sprintf(message,"%c%d", SOH, frameNumber);
	} else if (frameNumber > 99) {
		sprintf(message,"%c%s%d", SOH, "0", frameNumber);
	} else if (frameNumber > 9) {
		sprintf(message,"%c%s%d", SOH, "00", frameNumber);
	} else {
		sprintf(message,"%c%s%d", SOH, "000", frameNumber);
	}

	if (data[0] != 0) {
		sprintf(message, "%s%c%c%c", message, STX, data[0], ETX);
	} else {
		sprintf(message, "%s%c%s%c", message, STX, "EFFF", ETX);
	}
	CheckSum check = *(new CheckSum(message));
	check.BuildCheckSum();
	sprintf(message,"%s%c",message,check.getCheckSum());
	return message;
}

int Packet::getMsgSize() {
	return (1 + 4 + 1 + dataSize + 1 + 1);
}

void Packet::printMsg() {
	std::cout << this->getMsg() << std::endl;
}