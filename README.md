# Tugas-1-Jarkom-ACK_Hunter
Tugas Jarkom untuk simulasi Sliding Windows
## Petunjuk penggunaan program
1. Masukkan command "make" pada terminal pada linux di folder Tugas-1-Jarkom-ACK.
2. Masukkan perintah "./recvfile <filename> <windowsize> <buffersize> <port>" untuk melakukan run pada receiver.
3. Buka terminal baru.
4. Masukkan perintah "./sendfile <filename> <windowsize> <buffersize> <destination_ip> <destination_port>" untuk melakukan run pada sender.
Contoh run :
Receiver  : ./recvfile recv_file.txt 5 256 8888
Sender    : ./sendfile test_file.txt 5 256 127.0.0.1 8888
## Cara kerja sliding window dalam program
- handshake(int thisSocket, const int* SWS):

Koneksi antara sender dan receiver dimulai dengan saling mengirimkan window size. Informasi window size akan digunakan untuk menentukan nilai MaxSequence = RWS + SWS + 1
  
- sendSinglePacket(int thisSocket, Packet packet) //**sender.cpp**

Fungsi ini akan mengambil array of unsigned char berukuran 9 byte dari sebuah objek Paket dan mengirimkan array tersebut ke port yang sudah diset di socket input. Dalam paket itu sudah berisi SOH (1 byte), sequence Number (4 byte), STX (1 byte), data (1 byte), ETX (1 byte), dan checksum (1 byte). Fungsi ini kemudian lanjut menunggu balasan ack dari port tujuan.

- sendData(int thisSocket, char* data, int dataLength, int windowSize, int payloadSize, int maxSequence) //**sender.cpp**

Di sini flow control dengan window size dilakukan. Fungsi menerima array char yang akan dikirim. Pada fungsi ada sebuah "pointer" yang menunjukkan mulai darimana isi array yang akan dikirim. Pointer akan bergerak sesuai dengan ukuran window dan ukuran payload tiap paket (dalam kasus ini 1-byte). 

Kemudian akan dibuat vector::future<int>. Paket sejumlah window size akan dikirimkan dalam thread terpisah dan ditrack dalam vector tersebut. Jika mendapat ack yang sesuai, maka sequence akan ditambah. Tapi jika terjadi timeout, proses akan diulangi kembali dari pembuatan thread dll.
  
\*Penghitungan timeout dilakukan dengan mengeset waktu timeout dari socket.
  
Pengiriman yang sukses akan dilanjutkan dengan menambahkan pointer buffer sebanyak windowSize * payload Size.

- sendAck(int sock, usigned char* recvdata, int maxSequence) **//receiver.cpp**

Fungsi ini akan mengirim ack berdasarkan data yang diterima dari sender (yaitu parameter recvdata). Pertama fungsi akan membuat sebuah objek Ack yang kemudian diambil array unsigned char yang merupakan ACK yang akan dikirim.

Fungsi ini dipanggil di dalam fungsi main dari receiver.cpp segera setelah receiver menerima sebuah paket yang valid. Paket yang valid disimpan dalam sebuah buffer untuk ditulis ke dalam file.
  
## Pembagian tugas
<- Sampaikan dalam list pengerjaan untuk setiap mahasiswa. Sebagai contoh: XXXX mengerjakan fungsi YYYY, ZZZZ, dan YYZZ. ->
## Pertanyaan
1.	Apa yang terjadi jika advertised window yang dikirim bernilai 0? Apa cara untuk menangani hal tersebut?
2.	Sebutkan field data yang terdapat TCP Header serta ukurannya, ilustrasikan, dan jelaskan kegunaan dari masing-masing field data tersebut!
## Jawaban
  1. Jika sender menerima kabar dari receiver bahwa buffernya habis, maka sender akan berhenti mengirimkan data dan menunggu ack dengan kabar bahwa sudah ada buffer yang tersedia. Tetapi jika ack dengan  kabar tersebut hilang, maka akan terjadi deadlock di mana sender tidak akan pernah bisa melanjutkan pengiriman data. Salah satu solusi yang bisa diterapkan adalah dengan membuat sender mengirim sebuah paket berukuran 1-byte secara periodik setelah ia menerima ack dengan advertised window = 0. Receiver bisa mengirim sebuah ack jika sudah ada buffer yang tersedia atau membuang paket 1-byte tersebut, yang akan memicu timeout di sender untuk mengirim ulang paket 1-byte tersebut.

Sumber:
  - http://cseweb.ucsd.edu/~gmporter/classes/wi15/cse124/lectures/lecture5.pdf
  - http://www.brianstorti.com/tcp-flow-control/

2. Sebuah TCP header memiliki ukuran setidaknya 5 x 32 bit = 160 bit. Field yang terdapat pada sebuah TCP header diilustrasikan dengan gambar berikut:
![TCH header picture](/img/TCPheader.jpg)
  - Field SrcPort dan DstPort (masing-masing 16-bit) mengidentifikasi port asal dan tujuan. Kombinasi nomor port dan alamat IP merupakan identitas unik dari sebuah koneksi TCP.
  - SequenceNum (32-bit) mengandung nomor urut dari byte pertama data dalam segment.
  - Acknowledgement (32-bit) dan AdvertisedWindow (16-bit) mengandung informasi tentang alur pengiriman data yang datang dan keluar.
  - Field Flags (berukuran 6 bit) digunakan untuk memberi informasi kontrol antara sesama TCP. Contoh yang bisa dimasukkan ke field ini adalah flag SYN dan FIN (untuk memulai dan menutup konesksi TCP), RESET (menandakan receiver menerima paket yang tidak sesuai dan ingin menutup koneksi), PUSH, URG (menandakan bahwa sebuah segmen memilki data penting), dan ACK.
  - Jika flag URG di set, maka UrgPtr (16-bit) akan memberitahu di bagian mana data yang **tidak** penting dimulai dalam badan data.
  - Checksum(6-bit) digunakan untuk mengecek apakah data yang dikirim tidak rusak.
  - Karena ukuran header TCP bisa bervariasi (dengan manambahkan options setelah field wajib, HdrLen (4-bit) memberitahu panjang header dalam huruf berukuran 32 bit.
  
Sumber: 
  - Computer Networks a System Approach 5th edition (page 400)

