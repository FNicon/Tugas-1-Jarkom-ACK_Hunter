int main() {
	unsigned char orderedRcvPacket[bufferSize][9]; //Buat buffer untuk menerima paket berukuran 9 byte

	int rcvWindowPtr = 0;	//Pointer orderedRcvPacket
	int midWindowPtr = 0; //Pointer penulisan di tengah2 window
	int writerPtr = 0; //Pointer orderedCharToWrite
	int RWS = windowSize;	//Receiver Window Size
	int LFR = -1;	//Last Frame Received
	int LAF = windowSize;	//Last Acceptable Frame

	int maxSequence;
	int iteration = 0;	// Sudah berapa kali balik ke 0

	//Terima paket recvData


// Ini di thread sendiri
	while (rcvWindowPtr < bufferSize) {
		if (checkSumValid(recvData)) {
			int packetPosition = getSequence(recvData) + iteration * maxSequence; //getSequence pake bagian program yang pake bitwise XOR ('^')
			if (packetPosition > LFR && packetPosition <= LAF) {

				if (getSequence(recvData) == maxSequence - 1) {
					iteration++;
				}

				if (packetPosition != LFR + 1) {
					//Kirim Ack dengan nextSequence = packetPoistion + 1
					//dan advWindow = (recvWindowPtr + windowSize) - writerPtr
					LFR = packetPosition
				} else {
					//Kirim Ack dengan nextSequence = LFR + 1
					//dan advWindow = (recvWindowPtr + windowSize) - writerPtr
					//LFR tetap
				}

				//Pasang mutex di sini
				orderedRcvPacket[packetPosition] = getPayload(recvData);
				//Lepas mutex di sini

				if (writerPtr > rcvWindowPtr) {
					rcvWindowPtr = writerPtr;
				}

				LFR = rcvWindowPtr - 1;
				LAF = rcvWindowPtr + windowSize;

			} else {
				//Kirim Ack dengan nextSequence = LFR + 1
			}
		}
	}

// Ini di thread berbeda
	while (writerPtr < bufferSize) {
		//Sleep random di sini
		
		if (isNotNull(orderedRcvPacket[writerPtr])) {
			//pasang mutex di sini
			fout << getPayload(orderedRcvPacket[writerPtr]);
			//lepas mutex di sini

			writerPtr++;
		}
	}
}