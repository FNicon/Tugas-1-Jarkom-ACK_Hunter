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
  1. Jika sender menerima kabar bahwa dari receiver bahwa buffernya habis, maka sender akan berhenti mengirimkan data yang masih ada dan menunggu ack yang memberi kabar bahwa sudah ada buffer yang tersedia. Tetapi jika ack yang mempunyai kabar tersebut hilang, maka akan terjadi deadlock di mana sender tidak akan pernah bisa mengirim sisa datanya. Salah satu solusi yang bisa diterapkan adalah dengan membuat sender mengirim sebuah paket berukuran 1-byte secara periodik setelah ia menerima ack dengan advertised window = 0. Receiver bisa mengirim sebuah ack jika sudah ada buffer yang tersedia atau membuang paket 1-byte tersebut, yang akan memicu timeout di sender untuk mengirim ulang paket 1-byte tersebut.

2.
