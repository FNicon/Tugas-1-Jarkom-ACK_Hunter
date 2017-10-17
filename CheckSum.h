#ifndef _CHECKSUM_
#define _CHECKSUM_
#include "macros.h"
#include <string.h>

class CheckSum {
	private :
		char* packet;
		unsigned char* packet_2;
		char checkSum;
		unsigned char checkSum_2;
	public :
		CheckSum(segment* seg);
		CheckSum(ack* ackd);
		CheckSum(char*);
		CheckSum(unsigned char*);
		void BuildCheckSum();
		void BuildCheckSum_2();
		bool CheckSumValidation();
		char getCheckSum();
		unsigned char getCheckSum_2();
		//void printHextoBit(char a);
};
#endif
