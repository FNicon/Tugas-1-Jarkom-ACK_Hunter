/*
    Simple udp server
*/
#include<stdio.h> //printf
#include<string.h> //memset
#include<stdlib.h> //exit(0);
#include<arpa/inet.h>
#include<sys/socket.h>
#include "CheckSum.h"
#define BUFFER_LENGTH 512//Max length of buffer
#define PORT 8888 //The port on which to listen for incoming data
void die(char *errorMessage) {
	perror(errorMessage);
	exit(1);
}
int main(void) {
	struct sockaddr_in serverAddress;
	struct sockaddr_in clientAddress;
	int socketConnector;
	int socketSize = sizeof(clientAddress);
	int receivedLength;
	char buffer[BUFFER_LENGTH];
	//create a UDP socket
	socketConnector = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (socketConnector == -1) {
		die("socket");
	}
	// zero out the structure
	memset((char *) &serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(PORT);
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	//bind socket to port
	if(bind(socketConnector, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
		die("bind");
	}
	//keep listening for data
	while(1) {
		memset(buffer,'\0', BUFFER_LENGTH);
		printf("Waiting for data...");
		fflush(stdout);
		//try to receive some data, this is a blocking call
		receivedLength = recvfrom(socketConnector, buffer, BUFFER_LENGTH, 0, (struct sockaddr *)&clientAddress, &socketSize);
		if (receivedLength == -1) {
			die("recvfrom()");
		}
		//print details of the client/peer and the data received
		printf("Received packet from %s:%d\n", inet_ntoa(clientAddress.sin_addr), ntohs(clientAddress.sin_port));
		printf("Data: %s\n" , buffer);
		//now reply the client with the same data
		if (sendto(socketConnector, buffer, receivedLength, 0, (struct sockaddr*) &clientAddress, socketSize) == -1) {
			die("sendto()");
		}
	}
	close(socketConnector);
	return 0;
}