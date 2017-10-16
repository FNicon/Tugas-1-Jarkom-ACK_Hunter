#ifndef _CHECKSUM_
#define _CHECKSUM_
#include "macros.h"
#include <string.h>

class CheckSum {
	private :
		char* packet;
		char checkSum;
	public :
		CheckSum(char* inputPacket);
		void BuildCheckSum();
		bool CheckSumValidation();
		char getCheckSum();
		//void printHextoBit(char a);
};
#endif