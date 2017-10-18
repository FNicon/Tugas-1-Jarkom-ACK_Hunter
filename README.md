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
<- Jelaskan juga fungsi yang terkait dengan sliding window pada program anda dan apa yang dilakukan oleh fungsi tersebut.->
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

