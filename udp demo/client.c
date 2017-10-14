/*
Simple udp client
Taken from: http://www.binarytides.com/programming-udp-sockets-c-linux/
*/
#include<stdio.h> //printf
#include<string.h> //memset
#include<stdlib.h> //exit(0);
#include<arpa/inet.h>
#include<sys/socket.h>
 
#define SERVER "127.0.0.1"
#define BUFFER_LENGTH 512//Max length of buffer
#define PORT 8888 //The port on which to send data
void printHextoBit(char a) {
	int mask = 0x80; /* 10000000 */
	while (mask>0) {
		printf("%d", (a & mask) > 0 );
		mask >>= 1; /* move the bit down */
	}
	printf("\n");
}
char buildCheckSum(char* inputString) {
	char checkSum;
	checkSum = 0;
	int i;
	for (i=0;i<strlen(inputString);i++) {
		printf("%x \n", inputString[i]);
		printHextoBit(inputString[i]);
		checkSum = checkSum + inputString[i];
	}
	return(checkSum);
}
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
		testSum = buildCheckSum(message);
		printf("CheckSum : %x \n",testSum);
		printHextoBit(testSum);
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
