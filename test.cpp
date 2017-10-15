#include <stdio.h>
#include <iostream>
#include "Packet.h"
#include "Ack.h"

int main() {
	char* data = new char(1);
	data[0] = 65;
	char* data2 = new char(1);
	data2[0] = 'B';
	char* data3 = new char(1);
	data3[0] = 'C';
	// std::cout << sizeof(data) << std::endl;
	Packet packetA(1, data);
	Packet packetB(packetA);
	Packet packetC = packetA;

	packetA.printMsg();
	packetB.printMsg();
	packetC.printMsg();

	// Ack ack1(GOOD_ACK, 1);
	// Ack ack2(ack1);
	// Ack ack3 = ack2;
	// ack1.printAck();
	// ack2.printAck();
	// ack3.printAck();
	return 0;
}