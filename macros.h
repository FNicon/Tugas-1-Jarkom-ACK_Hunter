#ifndef _MACROS_
#define _MACROS_

#define SOH 1
#define STX 2
#define ETX 3
#define PACKET_SIZE 9
#define GOOD_ACK 6
#define NACK 21

typedef struct segment {
	char soh;
	char seqNum[4];
	char stx;
	char data;
	char etx;
	char checksum;
} segment;

typedef struct ack {
	char verdict;
	char nextSeq[4];
	char advWindow;
	char checksum;
} ack;



#endif