/*
Simple udp client
Taken from: http://www.binarytides.com/programming-udp-sockets-c-linux/
*/
#include<stdio.h> //printf
#include<string.h> //memset
#include<stdlib.h> //exit(0);
#include<arpa/inet.h>
#include<sys/socket.h>
#include "CheckSum.h"
#define SERVER "127.0.0.1"
#define BUFFER_LENGTH 512//Max length of buffer
#define PORT 8888 //The port on which to send data
void die(char *errorMessage) {
	perror(errorMessage);
	exit(1);
}
int main(void) {
	struct sockaddr_in clientAddress;
	char testSum;
	int socketConnector;
	int socketSize=sizeof(clientAddress);
	char buffer[BUFFER_LENGTH];
	char message[BUFFER_LENGTH];
	CheckSum check;
	socketConnector = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (socketConnector == -1) {
		die("socket");
	}
	memset((char *) &clientAddress, 0, sizeof(clientAddress));
	clientAddress.sin_family = AF_INET;
	clientAddress.sin_port = htons(PORT);
	if (inet_aton(SERVER , &clientAddress.sin_addr) == 0) {
		fprintf(stderr, "inet_aton() failed\n");
		exit(1);
	}
	while(1) {
		printf("Enter message : ");
		gets(message);
		check = new CheckSum(message);
		testSum = check.buildCheckSum(message);
		printf("CheckSum : %x \n",testSum);
		//send the message
		if (sendto(socketConnector, message, strlen(message) , 0 , (struct sockaddr *) &clientAddress, socketSize)==-1) {
			die("sendto()");
		}
		//receive a reply and print it
		//clear the buffer by filling null, it might have previously received data
		memset(buffer,'\0', BUFFER_LENGTH);
		//try to receive some data, this is a blocking call
		if (recvfrom(socketConnector, buffer, BUFFER_LENGTH, 0, (struct sockaddr *) &clientAddress, &socketSize) == -1) {
			die("recvfrom()");
		}
		puts(buffer);
	}
	close(socketConnector);
	return 0;
}
