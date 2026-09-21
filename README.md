ARISE: Shadow Hunter

game RPG text-based yang dibuat pake C++, vibes-nya Solo Leveling banget 

Dari pemburu E-rank receh sampe jadi Shadow Monarch yang disegani — semua progress-nya ada di tangan lo. Gaskeun berburu monster, tembus dungeon, buka skill, dan naik level pelan-pelan tapi pasti.

Yang Bisa Lo Lakuin
Naik level & dapet gelar otomatis makin lo push rank
Berburu monster satu-satu, turn-based, biar strategis dikit
Masuk dungeon buat tantangan yang lebih spicy + hadiah lebih gede
Skill tree isi 7 skill beda-beda vibe (Slash, Fireball, Heal, sampe Meteor Strike )
Atur sendiri poin stat lo mau condong ke mana
Inventory buat nyimpen & pake potion pas kepepet
Toko buat belanja pake Gold (dari EXP lo sendiri, jadi hemat-hemat ya)
Efek ngetik animasi ala terminal biar berasa immersive dikit
Yang Lo Butuhin
Compiler C++ yang support C++11 ke atas (soalnya pake <thread> sama <chrono>)
Linux/macOS: g++ atau clang++
Windows: g++ (via MinGW) atau Visual Studio
Cara Jalanin (gampang njir)
1. Clone dulu repo-nya
bash
git clone https://github.com/USERNAME/NAMA-REPO.git
cd NAMA-REPO
2. Compile

Linux/macOS:

bash
g++ solev.cpp -o solev -pthread

Windows (MinGW):

bash
g++ solev.cpp -o solev.exe
3. Jalanin

Linux/macOS:

bash
./solev

Windows:

bash
solev.exe
Cara Main (biar gak bingung pas awal)
Masukin nama karakter lo pas game mulai
Pilih menu tiap giliran:
1. Berburu Monster — grinding EXP santai
2. Masuk Dungeon — level kesulitan naik, hadiah juga naik
3. Buka Skill Tree — pake poin skill buat unlock jurus baru
4. Alokasikan Poin Stat — bikin karakter lo makin OP
5. Lihat Inventory — pake potion pas HP/MP tipis
6. Kunjungi Toko — belanja pake Gold hasil grinding
7. Simpan & Keluar — udahan dulu, lanjut nanti
Terus push level sampe titel lo jadi Shadow Monarch 
Isi Folder
.
├── solev.cpp
└── README.md
Lisensi

Bebas dipake, bebas dioprek. Mau nambahin lisensi formal (misal MIT) juga gapapa banget.

made with hate & banyak kopi
