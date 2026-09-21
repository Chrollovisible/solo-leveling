#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <map>
#include <thread>
#include <chrono>
#include <iomanip>
#include <limits>

using namespace std;

void efekKetik(const string& teks, int penundaan = 25) {
    for (char karakter : teks) {
        cout << karakter << flush;
        this_thread::sleep_for(chrono::milliseconds(penundaan));
    }
}

void bersihkanLayar() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void jedaLayar() {
    cout << "\nTekan Enter untuk melanjutkan...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}


class Skill {
public:
    string nama;
    string deskripsi;
    int levelDibutuhkan;
    int biayaMana;
    int pengaliKerusakan;
    bool terbuka;
    
    Skill(string namaSkill, string deskripsiSkill, int reqLevel, int mana, int dmgMult)
        : nama(namaSkill), deskripsi(deskripsiSkill), levelDibutuhkan(reqLevel), 
          biayaMana(mana), pengaliKerusakan(dmgMult), terbuka(false) {}
};

class Monster {
public:
    string nama;
    int level;
    int kesehatan;
    int kesehatanMaks;
    int serangan;
    int pertahanan;
    int hadiahExp;
    
    Monster(string namaMonster, int lvl, int hp, int atk, int def, int exp)
        : nama(namaMonster), level(lvl), kesehatan(hp), kesehatanMaks(hp), 
          serangan(atk), pertahanan(def), hadiahExp(exp) {}
    
    bool masihHidup() const {
        return kesehatan > 0;
    }
    
    void terimaKerusakan(int kerusakan) {
        int kerusakanAktual = kerusakan - pertahanan;
        if (kerusakanAktual < 1) kerusakanAktual = 1;
        kesehatan -= kerusakanAktual;
        if (kesehatan < 0) kesehatan = 0;
    }
    
    int hitungSerangan() const {
        return serangan + (rand() % 5);
    }
};


class Item {
public:
    string nama;
    string deskripsi;
    int pulihkanKesehatan;
    int pulihkanMana;
    int jumlah;
    
    Item(string namaItem, string deskripsiItem, int hp, int mp, int qty = 0)
        : nama(namaItem), deskripsi(deskripsiItem), pulihkanKesehatan(hp), 
          pulihkanMana(mp), jumlah(qty) {}
};


class Pemain {
public:
    string nama;
    string gelar;
    int level;
    int exp;
    int expKeLevelBerikut;
    int kesehatan;
    int kesehatanMaks;
    int mana;
    int manaMaks;
    int serangan;
    int pertahanan;
    int poinStat;
    int poinSkill;
    
    // Inven
    vector<Item> inventori;
    
    // Skill
    vector<Skill> daftarSkill;
    
    // Statistik
    int monsterDikalahkan;
    int dungeonDiselesaikan;
    int totalKerusakanDiberikan;
    
    Pemain(string namaPemain) 
        : nama(namaPemain), gelar("Pemula"), level(1), exp(0), expKeLevelBerikut(100),
          kesehatan(100), kesehatanMaks(100), mana(50), manaMaks(50), serangan(15), pertahanan(5),
          poinStat(0), poinSkill(0), monsterDikalahkan(0), dungeonDiselesaikan(0), totalKerusakanDiberikan(0) {
        

        inisialisasiSkill();
        

        inventori.push_back(Item("Potion Kecil", "Mulihin 30 HP", 30, 0, 3));
        inventori.push_back(Item("Mana Potion", "Mulihin 20 MP", 0, 20, 2));
    }
    
    void inisialisasiSkill() {
        daftarSkill.clear();
        daftarSkill.push_back(Skill("Slash", "Nyerang dengan pedang", 1, 5, 1));
        daftarSkill.push_back(Skill("Fireball", "Nglempar bola api", 3, 15, 2));
        daftarSkill.push_back(Skill("Heal", "Nambah HP", 5, 20, 0));
        daftarSkill.push_back(Skill("Double Strike", "Nyerang dua kali berturut-turut", 7, 25, 1));
        daftarSkill.push_back(Skill("Shadow Step", "Nge-flick dan serang dari belakang", 10, 30, 3));
        daftarSkill.push_back(Skill("Rage", "Tingkatin serangan sementara", 12, 40, 0));
        daftarSkill.push_back(Skill("Meteor Strike", "Manggil meteor buat ngancurin musuh", 15, 50, 5));
        
 
        daftarSkill[0].terbuka = true;
    }
    
    bool masihHidup() const {
        return kesehatan > 0;
    }
    
    void terimaKerusakan(int kerusakan) {
        int kerusakanAktual = kerusakan - pertahanan;
        if (kerusakanAktual < 1) kerusakanAktual = 1;
        kesehatan -= kerusakanAktual;
        if (kesehatan < 0) kesehatan = 0;
    }
    
    void pulihkanKesehatan(int jumlah) {
        kesehatan += jumlah;
        if (kesehatan > kesehatanMaks) kesehatan = kesehatanMaks;
    }
    
    void pulihkanMana(int jumlah) {
        mana += jumlah;
        if (mana > manaMaks) mana = manaMaks;
    }
    
    int hitungSerangan() const {
        return serangan + (rand() % 10);
    }
    
    void dapatkanExp(int jumlah) {
        exp += jumlah;
        efekKetik("Lu dapet " + to_string(jumlah) + " EXP!\n", 20);
        
        while (exp >= expKeLevelBerikut) {
            naikLevel();
        }
    }
    
    void naikLevel() {
        level++;
        exp -= expKeLevelBerikut;
        expKeLevelBerikut = 100 * level;
        
        kesehatanMaks += 20 + (rand() % 10);
        manaMaks += 10 + (rand() % 5);
        kesehatan = kesehatanMaks;
        mana = manaMaks;
        
        poinStat += 3;
        poinSkill += 1;
        
        // Perbarui gelar berdasarkan level
        if (level < 5) gelar = "Newbie/Cupu Njir";
        else if (level < 10) gelar = "Pemburu E-rank";
        else if (level < 15) gelar = "Pemburu D-rank";
        else if (level < 20) gelar = "Pemburu C-rank";
        else if (level < 25) gelar = "Pemburu B-rank";
        else if (level < 30) gelar = "Pemburu A-rank";
        else if (level < 40) gelar = "Pemburu S-rank";
        else gelar = "Shadow Monarch";
        
        efekKetik("\n================================\n", 10);
        efekKetik("LEVEL UP! Sekarang Level " + to_string(level) + "!\n", 30);
        efekKetik("Gelar: " + gelar + "\n", 30);
        efekKetik("================================\n\n", 10);
        
        this_thread::sleep_for(chrono::milliseconds(1000));
    }
    
    void tampilkanStatus() const {
        cout << "===============================================\n";
        cout << "           STATUS PEMAIN\n";
        cout << "===============================================\n";
        cout << "Nama:   " << nama << " [" << gelar << "]\n";
        cout << "Level:  " << level << "\n";
        cout << "EXP:    " << exp << "/" << expKeLevelBerikut << "\n";
        cout << "HP:     " << kesehatan << "/" << kesehatanMaks << "\n";
        cout << "MP:     " << mana << "/" << manaMaks << "\n";
        cout << "Serangan: " << serangan << "\n";
        cout << "Pertahanan:" << pertahanan << "\n";
        cout << "Poin Stat: " << poinStat << "\n";
        cout << "Poin Skill: " << poinSkill << "\n";
        cout << "===============================================\n";
        cout << "Monster yang udah lu kalahin: " << monsterDikalahkan << "\n";
        cout << "Dungeon yang udah lu selesain: " << dungeonDiselesaikan << "\n";
        cout << "Lu kena serangan sebanyak:    " << totalKerusakanDiberikan << "\n";
        cout << "===============================================\n";
    }
    
    void tampilkanSkill() const {
        cout << "===============================================\n";
        cout << "              SKILLS\n";
        cout << "===============================================\n";
        for (int i = 0; i < daftarSkill.size(); i++) {
            cout << i+1 << ". " << daftarSkill[i].nama;
            if (daftarSkill[i].terbuka) {
                cout << " [UDAH KEBUKA NIH]";
            } else {
                cout << " [Masih Kekunci Njir - Level " << daftarSkill[i].levelDibutuhkan << "]";
            }
            cout << "\n   " << daftarSkill[i].deskripsi << "\n";
            cout << "   Biaya MP: " << daftarSkill[i].biayaMana;
            if (daftarSkill[i].pengaliKerusakan > 0) {
                cout << " | Kerusakan: " << daftarSkill[i].pengaliKerusakan << "x Serangan";
            }
            cout << "\n\n";
        }
    }
    
    void tampilkanInventori() const {
        cout << "===============================================\n";
        cout << "              INVENTORY\n";
        cout << "===============================================\n";
        if (inventori.empty()) {
            cout << "Yahh, Punya Lu Masih Kosong!\n";
        } else {
            for (int i = 0; i < inventori.size(); i++) {
                cout << i+1 << ". " << inventori[i].nama 
                     << " (" << inventori[i].jumlah << ")\n";
                cout << "   " << inventori[i].deskripsi << "\n\n";
            }
        }
        cout << "===============================================\n";
    }
    
    bool gunakanSkill(int indeksSkill, Monster& target) {
        if (indeksSkill < 0 || indeksSkill >= daftarSkill.size()) return false;
        
        Skill& skill = daftarSkill[indeksSkill];
        
        if (!skill.terbuka) {
            efekKetik("Skillnya belum kebuka njir!\n", 20);
            return false;
        }
        
        if (mana < skill.biayaMana) {
            efekKetik("MP-mu nggak cukup!\n", 20);
            return false;
        }
        
        mana -= skill.biayaMana;
        
        if (skill.nama == "Heal") {
            int jumlahPulih = kesehatanMaks / 3;
            pulihkanKesehatan(jumlahPulih);
            efekKetik("Lu udah mulihin " + to_string(jumlahPulih) + " HP!\n", 20);
        } 
        else if (skill.nama == "Rage") {
            serangan += 10;
            efekKetik("Seranganmu udah meningkat 10 poin buat pertarungan ini!\n", 20);
        }
        else {
            int kerusakan = (serangan * skill.pengaliKerusakan) + (rand() % 15);
            efekKetik("Lu udah ngegunain " + skill.nama + "!\n", 20);
            target.terimaKerusakan(kerusakan);
            totalKerusakanDiberikan += kerusakan;
            
            // Efek khusus untuk skill tertentu
            if (skill.nama == "Double Strike") {
                efekKetik("Serangan kedua!\n", 20);
                int kerusakanKedua = serangan + (rand() % 10);
                target.terimaKerusakan(kerusakanKedua);
                totalKerusakanDiberikan += kerusakanKedua;
            }
        }
        
        return true;
    }
    
    void gunakanItem(int indeksItem) {
        if (indeksItem < 0 || indeksItem >= inventori.size()) {
            efekKetik("Itemmu nggak valid!\n", 20);
            return;
        }
        
        Item& item = inventori[indeksItem];
        
        if (item.jumlah <= 0) {
            efekKetik("Itemmu udah habis!\n", 20);
            return;
        }
        
        item.jumlah--;
        pulihkanKesehatan(item.pulihkanKesehatan);
        pulihkanMana(item.pulihkanMana);
        
        efekKetik("Lu udah ngegunain " + item.nama + "!\n", 20);
        if (item.pulihkanKesehatan > 0) {
            efekKetik("HP mu udah  dipulihin " + to_string(item.pulihkanKesehatan) + " poin.\n", 20);
        }
        if (item.pulihkanMana > 0) {
            efekKetik("MP mu udah dipulihin " + to_string(item.pulihkanMana) + " poin.\n", 20);
        }
        
        // Hapus item jika habis
        if (item.jumlah <= 0) {
            inventori.erase(inventori.begin() + indeksItem);
        }
    }
    
    void tambahItem(Item item) {
        // Cek apakah item sudah ada di inventori
        for (auto& itemInventori : inventori) {
            if (itemInventori.nama == item.nama) {
                itemInventori.jumlah += item.jumlah;
                return;
            }
        }
        
        // Jika tidak ada, tambahkan item baru
        inventori.push_back(item);
    }
    
    void bukaSkill(int indeksSkill) {
        if (indeksSkill < 0 || indeksSkill >= daftarSkill.size()) {
            efekKetik("Skillmu nggak valid!\n", 20);
            return;
        }
        
        Skill& skill = daftarSkill[indeksSkill];
        
        if (skill.terbuka) {
            efekKetik("Skillmu udah kebuka!\n", 20);
            return;
        }
        
        if (level < skill.levelDibutuhkan) {
            efekKetik("Levelmu belum cukup njir! Skill ini memerlukan level " + 
                      to_string(skill.levelDibutuhkan) + "\n", 20);
            return;
        }
        
        if (poinSkill <= 0) {
            efekKetik("Poin skillmu tidak cukup!\n", 20);
            return;
        }
        
        poinSkill--;
        skill.terbuka = true;
        efekKetik("Skill " + skill.nama + " berhasil dibuka!\n", 20);
    }
    
    void alokasikanPoinStat() {
        if (poinStat <= 0) {
            efekKetik("Nggak ada poin stat untuk dialokasikan!\n", 20);
            return;
        }
        
        while (poinStat > 0) {
            bersihkanLayar();
            tampilkanStatus();
            
            cout << "\n" << poinStat << " poin stat tersedia.\n";
            cout << "1. Mau Nambah Serangan? (+5)\n";
            cout << "2. Mau Nambah Pertahanan? (+3)\n";
            cout << "3. Mau Nambah Max HP? (+20)\n";
            cout << "4. Mau Nambah Max MP? (+10)\n";
            cout << "5. Keluar dari menu\n";
            cout << "Pilihan: ";
            
            int pilihan;
            cin >> pilihan;
            
            if (pilihan == 5) break;
            
            if (pilihan < 1 || pilihan > 4) {
                efekKetik("Pilihanmu nggak valid!\n", 20);
                continue;
            }
            
            switch (pilihan) {
                case 1:
                    serangan += 5;
                    break;
                case 2:
                    pertahanan += 3;
                    break;
                case 3:
                    kesehatanMaks += 20;
                    kesehatan += 20;
                    break;
                case 4:
                    manaMaks += 10;
                    mana += 10;
                    break;
            }
            
            poinStat--;
            efekKetik("Statmu udah berhasil ditingkatkan!\n", 20);
        }
    }
};

// Fungsi untuk membuat monster acak berdasarkan level pemain
Monster buatMonsterAcak(int levelPemain) {
    vector<string> namaNamaMonster = {
        "Goblin", "Slime", "Skeleton", "Wolf", "Orc", 
        "Dark Elf", "Minotaur", "Harpy", "Golem", "Dragon"
    };
    
    string nama = namaNamaMonster[rand() % namaNamaMonster.size()];
    
    // Level monster berdasarkan level pemain dengan sedikit variasi
    int levelMonster = levelPemain + (rand() % 3) - 1;
    if (levelMonster < 1) levelMonster = 1;
    
    // Stat monster berdasarkan level
    int hpDasar = 30 + (levelMonster * 10);
    int seranganDasar = 5 + (levelMonster * 3);
    int pertahananDasar = 2 + (levelMonster * 2);
    int expDasar = 20 + (levelMonster * 8);
    
    return Monster(nama, levelMonster, hpDasar, seranganDasar, pertahananDasar, expDasar);
}

// Fungsi untuk pertarungan
void pertarungan(Pemain& pemain) {
    Monster monster = buatMonsterAcak(pemain.level);
    
    efekKetik("\n================================\n", 10);
    efekKetik("MONSTERNYA MUNCUL!\n", 30);
    efekKetik(monster.nama + " Level " + to_string(monster.level) + "\n", 30);
    efekKetik("================================\n\n", 10);
    
    bool giliranPemain = true;
    
    while (pemain.masihHidup() && monster.masihHidup()) {
        if (giliranPemain) {
            // Giliran pemain
            int pilihan;
            
            while (true) {
                cout << "\n================================\n";
                cout << pemain.nama << " [HP: " << pemain.kesehatan << "/" << pemain.kesehatanMaks 
                     << " | MP: " << pemain.mana << "/" << pemain.manaMaks << "]\n";
                cout << monster.nama << " [HP: " << monster.kesehatan << "/" << monster.kesehatanMaks << "]\n";
                cout << "================================\n";
                cout << "1. Serang\n";
                cout << "2. Ngegunain Skillmu\n";
                cout << "3. Ngegunain Itemmu\n";
                cout << "4. Melarikan Diri\n";
                cout << "Pilihan: ";
                
                cin >> pilihan;
                
                if (pilihan == 1) {
                    // Serangan biasa
                    int kerusakan = pemain.hitungSerangan();
                    efekKetik("Lu udah nyerang " + monster.nama + "!\n", 20);
                    monster.terimaKerusakan(kerusakan);
                    pemain.totalKerusakanDiberikan += kerusakan;
                    efekKetik("Kerusakan: " + to_string(kerusakan) + "\n", 20);
                    break;
                } 
                else if (pilihan == 2) {
                    // Gunakan skill
                    bersihkanLayar();
                    pemain.tampilkanSkill();
                    
                    cout << "Pilih skill (0 untuk kembali): ";
                    int pilihanSkill;
                    cin >> pilihanSkill;
                    
                    if (pilihanSkill == 0) continue;
                    
                    if (pemain.gunakanSkill(pilihanSkill - 1, monster)) {
                        break;
                    }
                }
                else if (pilihan == 3) {
                    // Gunakan item
                    bersihkanLayar();
                    pemain.tampilkanInventori();
                    
                    if (pemain.inventori.empty()) {
                        efekKetik("Inventorimu masih kosong!\n", 20);
                        continue;
                    }
                    
                    cout << "Pilih item (0 untuk kembali): ";
                    int pilihanItem;
                    cin >> pilihanItem;
                    
                    if (pilihanItem == 0) continue;
                    
                    pemain.gunakanItem(pilihanItem - 1);
                    
                    // Setelah menggunakan item, giliran tetap pemain
                    continue;
                }
                else if (pilihan == 4) {
                    // Coba melarikan diri
                    if (rand() % 100 < 50) { // 50% chance untuk berhasil
                        efekKetik("Anda berhasil melarikan diri, awowakokwakwaokwok!\n", 20);
                        return;
                    } else {
                        efekKetik("Anda gagal melarikan diri,aowakwaokowakowawok!\n", 20);
                        break;
                    }
                }
                else {
                    efekKetik("Pilihanmu nggak valid!\n", 20);
                }
            }
        } 
        else {
            // Giliran monster
            this_thread::sleep_for(chrono::milliseconds(800));
            
            if (monster.masihHidup()) {
                int kerusakan = monster.hitungSerangan();
                efekKetik(monster.nama + " udah nyerang Lu!\n", 20);
                pemain.terimaKerusakan(kerusakan);
                efekKetik("Kerusakan: " + to_string(kerusakan) + "\n", 20);
            }
        }
        
        giliranPemain = !giliranPemain;
        
        // Beri jeda sebelum giliran berikutnya
        this_thread::sleep_for(chrono::milliseconds(500));
    }
    
    // Cek hasil pertarungan
    if (pemain.masihHidup()) {
        efekKetik("\n================================\n", 10);
        efekKetik("LU UDAH MENANG!\n", 30);
        efekKetik(monster.nama + " udah lu kalahin!\n", 30);
        efekKetik("================================\n", 10);
        
        pemain.dapatkanExp(monster.hadiahExp);
        pemain.monsterDikalahkan++;
        
        // Drop item acak
        if (rand() % 100 < 40) { // 40% chance dapat item
            Item itemJatuh("Potion Kecil", "Mulihin 30 HP", 30, 0, 1);
            
            if (rand() % 100 < 20) { // 20% chance dapat potion yang lebih baik
                itemJatuh = Item("Potion Besar", "Mulihin 60 HP", 60, 0, 1);
            }
            else if (rand() % 100 < 10) { // 10% chance dapat elixir
                itemJatuh = Item("Elixir", "Mulihin 40 HP dan 30 MP", 40, 30, 1);
            }
            
            efekKetik("Lu dapet " + itemJatuh.nama + "!\n", 20);
            pemain.tambahItem(itemJatuh);
        }
    } 
    else {
        efekKetik("\n================================\n", 10);
        efekKetik("WAH!!! LU KALAH NJIRR!!\n", 30);
        efekKetik("================================\n", 10);
        
        // Kehilangan exp saat kalah
        pemain.exp -= pemain.exp / 10;
        if (pemain.exp < 0) pemain.exp = 0;
        
        efekKetik("Lu udah kehilangan 10% EXP.\n", 20);
    }
    
    // Reset stat sementara (jika ada)
    pemain.serangan = 15 + (pemain.level * 3); // Reset ke serangan dasar
}

// Fungsi untuk dungeon
void masukDungeon(Pemain& pemain) {
    bersihkanLayar();
    
    efekKetik("================================\n", 10);
    efekKetik("        MASUK DUNGEON\n", 30);
    efekKetik("================================\n\n", 10);
    
    cout << "Pilih tingkat kesulitan dungeon dulu:\n";
    cout << "1. EZ (1-3 monster)\n";
    cout << "2. Normal (3-5 monster)\n";
    cout << "3. Sulit, Susah, Ripuh, Busung Lapar, Rumah Kardus (5-7 monster)\n";
    cout << "4. Kembali\n";
    cout << "Pilihan: ";
    
    int pilihan;
    cin >> pilihan;
    
    if (pilihan == 4) return;
    
    int monsterMin, monsterMaks;
    
    switch (pilihan) {
        case 1:
            monsterMin = 1; monsterMaks = 3;
            break;
        case 2:
            monsterMin = 3; monsterMaks = 5;
            break;
        case 3:
            monsterMin = 5; monsterMaks = 7;
            break;
        default:
            efekKetik("Pilihanmu nggak valid!\n", 20);
            return;
    }
    
    int jumlahMonster = monsterMin + (rand() % (monsterMaks - monsterMin + 1));
    
    efekKetik("\nMemasuki dungeon...\n", 20);
    efekKetik("Lu sekarang akan ngelawan " + to_string(jumlahMonster) + " monster!\n\n", 20);
    
    jedaLayar();
    
    for (int i = 0; i < jumlahMonster; i++) {
        bersihkanLayar();
        
        efekKetik("================================\n", 10);
        efekKetik("   Monster " + to_string(i+1) + "/" + to_string(jumlahMonster) + "\n", 30);
        efekKetik("================================\n\n", 10);
        
        pertarungan(pemain);
        
        if (!pemain.masihHidup()) {
            efekKetik("Lu dikeluarin dari dungeon!\n", 20);
            jedaLayar();
            return;
        }
        
        // Pulihkan sedikit HP dan MP setelah setiap monster
        if (i < jumlahMonster - 1) {
            pemain.pulihkanKesehatan(pemain.kesehatanMaks / 10);
            pemain.pulihkanMana(pemain.manaMaks / 10);
            
            efekKetik("\nIstirahat bentar lah yaa...\n", 20);
            efekKetik("HP dan MP udah dipulihin sedikit.\n", 20);
            
            jedaLayar();
        }
    }
    
    // Hadiah menyelesaikan dungeon
    efekKetik("\n================================\n", 10);
    efekKetik("SELAMAT! Lu udah nyelesain dungeon! HOKI NJIR\n", 30);
    efekKetik("================================\n\n", 10);
    
    pemain.dungeonDiselesaikan++;
    
    // Bonus EXP berdasarkan kesulitan
    int bonusExp = 50 * pilihan * jumlahMonster;
    pemain.dapatkanExp(bonusExp);
    
    // Bonus item
    Item hadiah("Potion Besar", "Mulihin 60 HP", 60, 0, pilihan); // Lebih banyak untuk dungeon yang lebih sulit
    pemain.tambahItem(hadiah);
    
    efekKetik("Lu ngedapetin " + to_string(pilihan) + " " + hadiah.nama + "!\n", 20);
    
    jedaLayar();
}

// Fungsi untuk toko
void kunjungiToko(Pemain& pemain) {
    bersihkanLayar();
    
    efekKetik("================================\n", 10);
    efekKetik("        TOKO PEMBURU\n", 30);
    efekKetik("================================\n\n", 10);
    
    vector<Item> barangToko = {
        Item("Potion Kecil", "Mulihin 30 HP", 30, 0, 1),
        Item("Potion Besar", "Mulihin 60 HP", 60, 0, 1),
        Item("Mana Potion", "Mulihin 30 MP", 0, 30, 1),
        Item("Elixir", "Mulihin 40 HP dan 30 MP", 40, 30, 1),
        Item("Attack Boost", "Ningkatin serangan 10 poin buat 1 pertarungan", 0, 0, 1)
    };
    
    vector<int> harga = {20, 40, 30, 60, 50};
    
    // Harga khusus untuk level tinggi
    if (pemain.level > 10) {
        barangToko.push_back(Item("Potion Elite", "Mulihin 100 HP", 100, 0, 1));
        harga.push_back(80);
    }
    
    while (true) {
        bersihkanLayar();
        
        cout << "================================\n";
        cout << "        TOKO PEMBURU\n";
        cout << "================================\n\n";
        
        cout << "Uang yang Lu punya: " << pemain.exp << " Gold (EXP)\n\n";
        
        for (int i = 0; i < barangToko.size(); i++) {
            cout << i+1 << ". " << barangToko[i].nama 
                 << " - " << harga[i] << " Gold\n";
            cout << "   " << barangToko[i].deskripsi << "\n\n";
        }
        
        cout << barangToko.size() + 1 << ". Kembali\n";
        cout << "Pilihan: ";
        
        int pilihan;
        cin >> pilihan;
        
        if (pilihan == barangToko.size() + 1) {
            break;
        }
        
        if (pilihan < 1 || pilihan > barangToko.size()) {
            efekKetik("Pilihanmu nggak valid!\n", 20);
            jedaLayar();
            continue;
        }
        
        int indeksBarang = pilihan - 1;
        
        if (pemain.exp < harga[indeksBarang]) {
            efekKetik("Goldmu nggak cukup!\n", 20);
            jedaLayar();
            continue;
        }
        
        pemain.exp -= harga[indeksBarang];
        pemain.tambahItem(barangToko[indeksBarang]);
        
        efekKetik("Lu udah beli " + barangToko[indeksBarang].nama + "!\n", 20);
        
        // Efek khusus untuk Attack Boost
        if (barangToko[indeksBarang].nama == "Attack Boost") {
            pemain.serangan += 10;
            efekKetik("Seranganmu meningkat 10 poin buat pertarungan berikutnya!\n", 20);
        }
        
        jedaLayar();
    }
}

// Fungsi utama game
int main() {
    srand(time(0));
    
    bersihkanLayar();
    
    efekKetik("================================\n", 10);
    efekKetik("   ARISE: SHADOW HUNTER\n", 40);
    efekKetik("================================\n\n", 10);
    
    efekKetik("Selamat datang di dunia pemburu monster!\n", 30);
    efekKetik("Sistem misterius udah milih elu nih...\n\n", 30);
    
    efekKetik("Masukin nama Lu: ", 30);
    string namaPemain;
    getline(cin, namaPemain);
    
    Pemain pemain(namaPemain);
    
    efekKetik("\nSistem udah diaktifin...\n", 30);
    efekKetik("Selamat datang, " + namaPemain + "!\n", 30);
    efekKetik("Lu sekarang adalah seorang pemburu E-rank.\n\n", 30);
    
    jedaLayar();
    
    bool gameBerjalan = true;
    
    while (gameBerjalan && pemain.masihHidup()) {
        bersihkanLayar();
        
        pemain.tampilkanStatus();
        
        cout << "\n================================\n";
        cout << "           MENU UTAMA\n";
        cout << "================================\n";
        cout << "1. Berburu Monster\n";
        cout << "2. Masuk Dungeon\n";
        cout << "3. Buka Skill Tree\n";
        cout << "4. Ngalokasiin Poin Stat\n";
        cout << "5. Ngeliat Inventory\n";
        cout << "6. Ngunjungin Toko\n";
        cout << "7. Simpan & Keluar\n";
        cout << "Pilihan: ";
        
        int pilihan;
        cin >> pilihan;
        
        switch (pilihan) {
            case 1:
                bersihkanLayar();
                pertarungan(pemain);
                jedaLayar();
                break;
                
            case 2:
                masukDungeon(pemain);
                break;
                
            case 3:
                bersihkanLayar();
                pemain.tampilkanSkill();
                
                if (pemain.poinSkill > 0) {
                    cout << "\nLu udah punya " << pemain.poinSkill << " poin skill.\n";
                    cout << "Lu mau ngebuka skillnya? (y/n): ";
                    
                    char pilihanBuka;
                    cin >> pilihanBuka;
                    
                    if (pilihanBuka == 'y' || pilihanBuka == 'Y') {
                        cout << "Pilih skill buat dibuka (0 untuk kembali): ";
                        int pilihanSkill;
                        cin >> pilihanSkill;
                        
                        if (pilihanSkill != 0) {
                            pemain.bukaSkill(pilihanSkill - 1);
                        }
                    }
                }
                
                jedaLayar();
                break;
                
            case 4:
                pemain.alokasikanPoinStat();
                break;
                
            case 5:
                bersihkanLayar();
                pemain.tampilkanInventori();
                
                if (!pemain.inventori.empty()) {
                    cout << "Mau ngegunain itemnya? (y/n): ";
                    char pilihanGunakan;
                    cin >> pilihanGunakan;
                    
                    if (pilihanGunakan == 'y' || pilihanGunakan == 'Y') {
                        cout << "Pilih item (0 untuk kembali): ";
                        int pilihanItem;
                        cin >> pilihanItem;
                        
                        if (pilihanItem != 0) {
                            pemain.gunakanItem(pilihanItem - 1);
                        }
                    }
                }
                
                jedaLayar();
                break;
                
            case 6:
                kunjungiToko(pemain);
                break;
                
            case 7:
                gameBerjalan = false;
                efekKetik("Nyimpen progres...\n", 20);
                efekKetik("Sampai jumpa, " + pemain.nama + "!\n", 30);
                break;
                
            default:
                efekKetik("Pilihanmu nggak valid!\n", 20);
                jedaLayar();
                break;
        }
    }
    
    if (!pemain.masihHidup()) {
        efekKetik("\n================================\n", 10);
        efekKetik("      GAME OVER\n", 40);
        efekKetik("================================\n\n", 10);
        efekKetik("Lu udah gugur dalam pertempuran, awaokwaowaokwawok...\n", 30);
        efekKetik("Level akhir: " + to_string(pemain.level) + "\n", 30);
        efekKetik("Monster yang udah lu kalahin: " + to_string(pemain.monsterDikalahkan) + "\n", 30);
        efekKetik("Total kerusakan: " + to_string(pemain.totalKerusakanDiberikan) + "\n", 30);
    }
    
    return 0;
}