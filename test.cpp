#include <stdio.h>
#include <iostream>
#include "Packet.h"

int main() {
	char* data = new char(1);
	data[0] = 'A';
	char* data2 = new char(1);
	data2[0] = 'B';
	char* data3 = new char(1);
	data3[0] = 'C';
	// std::cout << sizeof(data) << std::endl;
	Packet packetA(1, data);
	Packet packetB(packetA);
	Packet packetC = packetA;

	std::cout << packetA.getPacket() << std::endl;
	std::cout << packetB.getPacket() << std::endl;
	std::cout << packetC.getPacket() << std::endl;
	return 0;
}