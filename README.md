#PROJECT_51
Konsep dari projek ini, berfokus pada beberapa kontrol dari sebuah robot, sebagai berikut:
1. Mendeteksi objek dengan warna tertentu, dan membawa dan memberikan interaksi pada objek tersebut.
2. Menghindari tabrakan, serta menyusuri suatu jalur.
3. Mendeteksi robot lain, serta koordinasi antar robot.


**Implementasi konsep**

Konsep diatas di-implementasikan pada sebuah robot bola (dijelaskan dalam bagian *Objek*), jadi objek yang dimaksud diatas adalah sebuah bola yang memiliki warna terentu.
Robot yang dibuat memiliki 2 jenis, pertama robot sebagai pengambil bola, kedua robot sebagai penendang bola. Robot penjaga bertugas untuk mengambil bola dengan warna biru dari kumpulan bola.
Setelah bola yang dimaksud didapat, maka robot pertama harus membawa bola dan melewati labirin dan bertemu robot kedua di tengah-tengah labirin. Lalu robot kedua bertugas untuk menerima bola dari robot pertama.
Ketika sudah diterima, robot kediua harus keluar dari labirin dan mencari gawang, kemudian menendang bolanya ke arah gawang.


**Arena**

1. Lapangan
Di lapangan terdapat bola-bola dengan warna yang berbeda-beda dan robot A diminta untuk memlilah bola dengan warna tertentu.
2. Maze 
Di maze yang berliku-liku dan terdapat suatu jalan buntu, robot A akan mencari robot B untuk kemudian menyalurkan bola tersebut kepada robot B. Di Maze terdapat sebuah gawang dan robot b akan memasukan bola tersebut ke gawang dengan bola yang telah diperoleh dari robot A.


**Objek**

1. 2 robot model penjaga dan penendang bola : (satu robot yaitu robot A memasukki maze dengan membawa suatu bola, lalu robot A memberikan bola kepada robot B yang berada di dalam maze)
*  Robot dengan bentuk badan menyerupai huruf "U". 
*  Robot memiliki kaki penendang atau *kicker* berupa ayunan di bagian tengah robot yang tegak lurus dengan badan. *Kicker* digunakan untuk memasukkan bola ke gawang di akhir maze.
*  Robot memiliki sensor untuk mendeteksi warna bola dan objek yang dicari.
*  Robot menangkap bola dengan warna yang diinginkan dan menjaga bola tersebut dengan menutup pagar di depan robot, sehingga robot seolah-olah berbentuk huruf "O".
*  Robot memiliki 4 roda, dua di bagian kiri dan dua di bagian kanan.


2. Bola-bola dengan warna yang berbeda:
*  Di awal maze, robot diberikan banyak pilihan bola dengan masing-masing bola memiliki warna yang beragam.
*  Bola-bola memiliki ukuran tidak lebih dari ukuran *gap* yang dibentuk oleh bodi robot yang berbentuk huruf "U".

3. Gawang
*  Objek yang digunakan untuk menampung bola yang ditendang oleh robot di akhir maze.
*  Gawang memiliki suatu ciri khas, e.g. warna cerah untuk membantu robot mendeteksi gawang tersebut.
*  

TUGAS 1 (20/11/2019):
*  Membuat list apa saja yang harus dilakukan
*  Tunjukkan apa yang telah dilakukan setiap asistensi

