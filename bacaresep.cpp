#include <iostream>

using namespace std;

struct NodeLangkah {
    int nomorLangkah;
    string deskripsiLangkah;

    NodeLangkah* next;
    NodeLangkah* prev;
};

struct NodeResep {
    // informasi resep
    string namaResep;
    int estimasiWaktu;
    
    // untuk ngehubungin ke DLL Langkah
    NodeLangkah* headLangkah;
    NodeLangkah* tailLangkah;

    NodeResep* next;
};

// global pointer buat ntyimpen head sama tail katalog
NodeResep* headKatalog = NULL;
NodeResep* tailKatalog = NULL;

// deklarasi fungsi
void simpanKeFile();
void bacaDariFile();
bool cekDuplikatNama(string nama);
void tambahResep();
void lihatKatalog();

// --- BAGIAN FILE ---
// fungsi buat nyimpen semua linked list ke txt
void simpanKeFile() {
    FILE* file = fopen("resep.txt", "w");
    if (file == NULL) {
        cout << "Gagal nyimpen ke file!" << endl;
        return;
    }

    NodeResep* tempResep = headKatalog;
    while (tempResep != NULL) {
        // tanda awal resep
        fprintf(file, "#RESEP\n");
        fprintf(file, "%s\n", tempResep->namaResep.c_str());
        fprintf(file, "%d\n", tempResep->estimasiWaktu);

        NodeLangkah* tempLangkah = tempResep->headLangkah;
        while (tempLangkah != NULL) {
            fprintf(file, "#LANGKAH\n");
            fprintf(file, "%d\n", tempLangkah->nomorLangkah);
            fprintf(file, "%s\n", tempLangkah->deskripsiLangkah.c_str());
            tempLangkah = tempLangkah->next;
        }
        
        // kalau resep abis
        fprintf(file, "#END_RESEP\n");
        tempResep = tempResep->next;
    }
    
    fclose(file);
}

// fungsi buat ngebangun ulang linked list dari file txt pas app dibuka
void bacaDariFile() {
    FILE* file = fopen("resep.txt", "r");
    if (file == NULL) {
        // kalo file belum ada, otomatis kebuat kosong pas nanti save
        return; 
    }

    char buffer[256];
    NodeResep* resepBaru = NULL;

    // baca file baris per baris
    while (fgets(buffer, sizeof(buffer), file)) {
        string baris(buffer);
        if (!baris.empty() && baris.back() == '\n') baris.pop_back(); // hapus newline (\n) di akhir

        if (baris == "#RESEP") {
            resepBaru = new NodeResep();
            resepBaru->headLangkah = NULL;
            resepBaru->tailLangkah = NULL;
            resepBaru->next = NULL;

            // baca nama
            fgets(buffer, sizeof(buffer), file);
            string nama(buffer);
            if (!nama.empty() && nama.back() == '\n') nama.pop_back();
            resepBaru->namaResep = nama;

            // baca estimasi waktu
            fgets(buffer, sizeof(buffer), file);
            string waktu(buffer);
            if (!waktu.empty() && waktu.back() == '\n') waktu.pop_back();
            resepBaru->estimasiWaktu = stoi(waktu);

        } else if (baris == "#LANGKAH" && resepBaru != NULL) {
            NodeLangkah* langkahBaru = new NodeLangkah();
            
            // baca nomor langkah
            fgets(buffer, sizeof(buffer), file);
            string noLangkah(buffer);
            if (!noLangkah.empty() && noLangkah.back() == '\n') noLangkah.pop_back();
            langkahBaru->nomorLangkah = stoi(noLangkah);

            // baca deskripsi
            fgets(buffer, sizeof(buffer), file);
            string deskripsi(buffer);
            if (!deskripsi.empty() && deskripsi.back() == '\n') deskripsi.pop_back();
            langkahBaru->deskripsiLangkah = deskripsi;

            langkahBaru->next = NULL;
            langkahBaru->prev = NULL;

            // sambungin ke DLL Langkah
            if (resepBaru->headLangkah == NULL) {
                resepBaru->headLangkah = langkahBaru;
                resepBaru->tailLangkah = langkahBaru;
            } else {
                resepBaru->tailLangkah->next = langkahBaru;
                langkahBaru->prev = resepBaru->tailLangkah;
                resepBaru->tailLangkah = langkahBaru;
            }
            
        } else if (baris == "#END_RESEP" && resepBaru != NULL) {
            // masukin resep yang udah utuh ke SLL Katalog Utama
            if (headKatalog == NULL) {
                headKatalog = resepBaru;
                tailKatalog = resepBaru;
            } else {
                tailKatalog->next = resepBaru;
                tailKatalog = resepBaru;
            }
            resepBaru = NULL;
        }
    }
    fclose(file);
}

bool cekDuplikatNama(string nama) {
    NodeResep* temp = headKatalog;
    while (temp != NULL) {
        if (temp->namaResep == nama) return true;
        temp = temp->next;
    }
    return false;
}

void tambahResep() {
    string nama;
    int waktu;
    
    cout << "\n=== TAMBAH RESEP BARU ===\n";
    cout << "Masukkan nama resep: ";
    getline(cin >> ws, nama);

    // Error handling: gaboleh duplikat
    if (cekDuplikatNama(nama)) {
        cout << "Gagal! Resep dengan nama '" << nama << "' udah ada.\n";
        return;
    }

    cout << "Estimasi waktu masak (menit): ";
    cin >> waktu;

    // Bikin node resep baru
    NodeResep* resepBaru = new NodeResep();
    resepBaru->namaResep = nama;
    resepBaru->estimasiWaktu = waktu;
    resepBaru->headLangkah = NULL;
    resepBaru->tailLangkah = NULL;
    resepBaru->next = NULL;

    cout << "\n-- Masukkan Langkah-langkah Memasak --\n";
    cout << "(Ketik 'selesai' di deskripsi kalau udah beres)\n";
    
    int noLangkah = 1;
    while (true) {
        string deskripsi;
        cout << "Langkah " << noLangkah << ": ";
        getline(cin >> ws, deskripsi);

        if (deskripsi == "selesai" || deskripsi == "Selesai") break;

        // Bikin node langkah baru (DLL)
        NodeLangkah* langkahBaru = new NodeLangkah();
        langkahBaru->nomorLangkah = noLangkah;
        langkahBaru->deskripsiLangkah = deskripsi;
        langkahBaru->next = NULL;
        langkahBaru->prev = NULL;

        // Proses insert Tail buat langkah
        if (resepBaru->headLangkah == NULL) {
            resepBaru->headLangkah = langkahBaru;
            resepBaru->tailLangkah = langkahBaru;
        } else {
            resepBaru->tailLangkah->next = langkahBaru;
            langkahBaru->prev = resepBaru->tailLangkah;
            resepBaru->tailLangkah = langkahBaru;
        }
        noLangkah++;
    }

    // Sambungin resep baru ke Katalog (SLL)
    if (headKatalog == NULL) {
        headKatalog = resepBaru;
        tailKatalog = resepBaru;
    } else {
        tailKatalog->next = resepBaru;
        tailKatalog = resepBaru;
    }

    cout << "Resep berhasil ditambah!\n";
    simpanKeFile(); // update txt
}

void lihatKatalog() {
    cout << "\n=== KATALOG RESEP ===\n";
    if (headKatalog == NULL) {
        cout << "Katalog masih kosong, belum ada resep.\n";
        return;
    }

    NodeResep* temp = headKatalog;
    int i = 1;
    while (temp != NULL) {
        cout << i << ". " << temp->namaResep << " (" << temp->estimasiWaktu << " menit)\n";
        temp = temp->next;
        i++;
    }
}

int main() {
    // pas aplikasi buka, langsung narik dari memory/file
    bacaDariFile(); 

    int pilihan;
    do {
        // ini sesuai rancangan di docs sebelumnya tha
        cout << "\n===================================\n";
        cout << "   SISTEM PEMBACAAN RESEP MAKANAN  \n";
        cout << "===================================\n";
        cout << "1. Lihat Katalog Resep\n";
        cout << "2. Cari Resep (dan Mulai Memasak)\n";
        cout << "3. Urutkan Resep\n";
        cout << "4. Tambah Resep Baru\n";
        cout << "5. Edit Resep\n";
        cout << "6. Hapus Resep\n";
        cout << "0. Keluar\n";
        cout << "===================================\n";
        cout << "Pilih menu: ";
        
        // error handling biar ga infinite loop kalau inputnya string
        if (!(cin >> pilihan)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Input tidak valid! Harap masukkan angka.\n";
            continue;
        }

        switch (pilihan) {
            // fungsi2nya ini
            case 1: lihatKatalog(); break; //done
            case 2: cariResep(); break;
            case 3: urutkanResep(); break;
            case 4: tambahResep(); break; //done
            case 5: editResep(); break;
            case 6: hapusResep(); break;
            case 0: cout << "Selamat tinggal!\n"; break;
            default: cout << "Pilihan menu ga ada, ulangi.\n";
        }
    } while (pilihan != 0);

    return 0;
}