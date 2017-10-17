all: Packet.cpp sender.cpp receiver.cpp Ack.cpp
	#g++ sender.cpp CheckSum.cpp -o sendfile
	g++ sender.cpp Packet.cpp CheckSum.cpp -o sendfile -std=c++11
	#g++ receiver_v2.cpp CheckSum.cpp -o recvfile
	g++ receiver.cpp Ack.cpp CheckSum.cpp -o recvfile -std=c++11

sendfile: Packet.cpp sender.cpp
	g++ Packet.cpp sender.cpp CheckSum.cpp -o sendfile
	./sendfile test_file.txt 5 256 127.0.0.1 8888

recvfile: receiver.cpp Ack.cpp
	g++ receiver.cpp Ack.cpp CheckSum.cpp -o recvfile
	./recvfile recv_file.txt 5 256 8888
