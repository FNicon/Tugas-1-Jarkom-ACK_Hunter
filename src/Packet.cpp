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
void Packet::getMsg() {
	unsigned char* message = new unsigned char[1 + 4 + 1+ dataSize + 1 + 1];
	packetMessage = new unsigned char(9);
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
	check.BuildCheckSum();
	message[8] = check.getCheckSum();
	for (int i = 0;i<=sizeof(message);i++) {
		packetMessage[i] = message[i];
	}
}
unsigned char* Packet::getPacketMessage() {
	return (packetMessage);
}

int Packet::getMsgSize() {
	return (1 + 4 + 1 + dataSize + 1 + 1);
}
void Packet::printMsg() {
	printf ("[Packet.cpp] package content (hex): %x %x %x %x %x %x %x %x %x\n", packetMessage[0], packetMessage[1], packetMessage[2], packetMessage[3], packetMessage[4], packetMessage[5], packetMessage[6], packetMessage[7], packetMessage[8]);
}
