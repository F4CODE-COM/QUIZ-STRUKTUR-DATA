#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

const int UKURAN = 20;

enum Status { KOSONG, TERISI, DIHAPUS };

struct Slot {
    long long nim;
    string nama;
    Status status;
};

class HashTable {
    Slot tabel[UKURAN];
    static int hash1(long long key) { return key % 20; }
    static int hash2(long long key) { return 19 - (key % 19); }
    static int indeks(long long key, int i) { return (hash1(key) + (long long)i * hash2(key)) % UKURAN; }
public:
    HashTable() { for (auto& s : tabel) { s.nim = 0; s.nama = ""; s.status = KOSONG; } }

    bool insert(long long nim, const string& nama) {
        if (search(nim) != -1) { cout << "  NIM " << nim << " sudah ada.\n"; return false; }
        int slotHapus = -1;
        for (int i = 0; i < UKURAN; i++) {
            int idx = indeks(nim, i);
            if (tabel[idx].status == TERISI) {
                cout << "  [collision] NIM " << nim << " bentrok di indeks " << idx
                     << " (probe ke-" << i << "), coba lagi dengan Hash2 = " << hash2(nim) << "\n";
                continue;
            }
            if (tabel[idx].status == DIHAPUS && slotHapus == -1) slotHapus = idx;
            if (tabel[idx].status == KOSONG) {
                if (slotHapus != -1) idx = slotHapus;
                tabel[idx] = {nim, nama, TERISI};
                cout << "  NIM " << nim << " (" << nama << ") disimpan di indeks " << idx << "\n";
                return true;
            }
        }
        if (slotHapus != -1) {
            tabel[slotHapus] = {nim, nama, TERISI};
            cout << "  NIM " << nim << " (" << nama << ") disimpan di indeks " << slotHapus << "\n";
            return true;
        }
        cout << "  Gagal insert NIM " << nim << ": tabel penuh / tidak ada slot terjangkau.\n";
        return false;
    }

    
    int search(long long nim) {
        for (int i = 0; i < UKURAN; i++) {
            int idx = indeks(nim, i);
            if (tabel[idx].status == KOSONG) return -1;
            if (tabel[idx].status == TERISI && tabel[idx].nim == nim) return idx;
        }
        return -1;
    }

    void cari(long long nim) {
        int idx = search(nim);
        if (idx == -1) cout << "  NIM " << nim << " tidak ditemukan.\n";
        else cout << "  Ditemukan di indeks " << idx << " -> " << tabel[idx].nim
                  << " | " << tabel[idx].nama << "\n";
    }

    bool remove(long long nim) {
        int idx = search(nim);
        if (idx == -1) { cout << "  NIM " << nim << " tidak ditemukan, gagal dihapus.\n"; return false; }
        tabel[idx].status = DIHAPUS;   // tombstone agar rantai probing tidak putus
        cout << "  NIM " << nim << " (" << tabel[idx].nama << ") dihapus dari indeks " << idx << "\n";
        return true;
    }

    void display() {
        cout << left << setw(8) << "Indeks" << setw(14) << "NIM" << setw(22) << "Nama" << "Status\n";
        cout << string(55, '-') << "\n";
        for (int i = 0; i < UKURAN; i++) {
            cout << left << setw(8) << i;
            if (tabel[i].status == TERISI)
                cout << setw(14) << tabel[i].nim << setw(22) << tabel[i].nama << "Terisi\n";
            else if (tabel[i].status == DIHAPUS)
                cout << setw(14) << "-" << setw(22) << "-" << "Dihapus\n";
            else
                cout << setw(14) << "-" << setw(22) << "-" << "Kosong\n";
        }
    }
};

void ujiOtomatis(HashTable& ht) {
    cout << "\n--- Insert 15 data (ada collision) ---\n";
    struct D { long long nim; const char* nama; };
    D data[15] = {
        {2024001, "Andi"},   {2024002, "Budi"},    {2024003, "Citra"},
        {2024004, "Dewi"},   {2024005, "Eko"},     {2024006, "Fajar"},
        {2024007, "Gita"},   {2024008, "Hadi"},    {2024009, "Indah"},
        {2024010, "Joko"},   {2024021, "Kiki"},    {2024023, "Lina"},
        {2024041, "Maya"},   {2024061, "Nanda"},   {2024081, "Oscar"}
    };
    for (auto& d : data) ht.insert(d.nim, d.nama);

    cout << "\n--- Display ---\n";
    ht.display();

    cout << "\n--- Search ---\n";
    ht.cari(2024021);
    ht.cari(2024999);

    cout << "\n--- Remove NIM 2024001 lalu 2024021 (collision) ---\n";
    ht.remove(2024001);
    ht.remove(2024021);
    ht.remove(2024999);

    cout << "\n--- Search setelah remove (data lain tetap terjangkau) ---\n";
    ht.cari(2024021);
    ht.cari(2024041);

    cout << "\n--- Insert NIM 2024101 (memakai ulang slot bekas hapus) ---\n";
    ht.insert(2024101, "Putri");

    cout << "\n--- Display akhir ---\n";
    ht.display();
}

int main() {
    HashTable ht;
    int pilih;
    do {
        cout << "\n=== HASH TABLE MAHASISWA (Double Hashing) ===\n"
             << "1. Insert\n"
             << "2. Search\n"
             << "3. Remove\n"
             << "4. Display\n"
             << "5. Jalankan pengujian otomatis (15 data)\n"
             << "0. Keluar\n"
             << "Pilih: ";
        cin >> pilih;
        if (pilih == 1) {
            long long nim; string nama;
            cout << "NIM  : "; cin >> nim;
            cout << "Nama : "; cin.ignore(); getline(cin, nama);
            ht.insert(nim, nama);
        }
        else if (pilih == 2) { long long nim; cout << "NIM: "; cin >> nim; ht.cari(nim); }
        else if (pilih == 3) { long long nim; cout << "NIM: "; cin >> nim; ht.remove(nim); }
        else if (pilih == 4) ht.display();
        else if (pilih == 5) ujiOtomatis(ht);
    } while (pilih != 0);
    cout << "Program selesai.\n";
    return 0;
}
