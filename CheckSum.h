#ifndef _CHECKSUM_
#define _CHECKSUM_
#include "macros.h"
#include <string.h>

class CheckSum {
	private :
		unsigned char* packet;
		unsigned char checkSum;
	public :
		CheckSum(unsigned char* inputPacket);
		void BuildCheckSum();
		bool CheckSumValidation(unsigned char* inputPacket);
		unsigned char getCheckSum();
};
#endif
