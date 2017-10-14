#include <stdio.h>
#include <iostream>
#include "Packet.h"

int main() {
	char* data = new char(1);
	data[0] = 'B';
	// std::cout << sizeof(data) << std::endl;
	Packet packetA(1234, data);

	std::cout << packetA.getPacket() << std::endl;
	return 0;
}