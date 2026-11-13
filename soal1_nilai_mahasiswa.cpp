#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

struct Mahasiswa {
    long long nim;
    string nama;
    double nilai;
    Mahasiswa* next;
};

// DYNAMCI ARRAY
class DynamicArray {
    double* data;
    int ukuran, kapasitas;
    void perbesar() {
        kapasitas *= 2;
        double* baru = new double[kapasitas];
        for (int i = 0; i < ukuran; i++) baru[i] = data[i];
        delete[] data;
        data = baru;
    }
public:
    DynamicArray() : ukuran(0), kapasitas(2) { data = new double[kapasitas]; }
    ~DynamicArray() { delete[] data; }
    void tambah(double v) {
        if (ukuran == kapasitas) perbesar();
        data[ukuran++] = v;
    }
    int size() const { return ukuran; }
    double& operator[](int i) { return data[i]; }

    // Insertion Sort
    void insertionSort() {
        for (int i = 1; i < ukuran; i++) {
            double kunci = data[i];
            int j = i - 1;
            while (j >= 0 && data[j] > kunci) {
                data[j + 1] = data[j];
                j--;
            }
            data[j + 1] = kunci;
        }
    }

    // Binary Search 
    int binarySearch(double target) {
        int kiri = 0, kanan = ukuran - 1;
        while (kiri <= kanan) {
            int tengah = kiri + (kanan - kiri) / 2;
            if (data[tengah] == target) return tengah;
            if (data[tengah] < target) kiri = tengah + 1;
            else kanan = tengah - 1;
        }
        return -1;
    }
};

//LINKED LIST (SINGLE)
class DaftarMahasiswa {
    Mahasiswa* head;
public:
    DaftarMahasiswa() : head(nullptr) {}
    ~DaftarMahasiswa() {
        while (head) { Mahasiswa* t = head; head = head->next; delete t; }
    }
    bool nimAda(long long nim) {
        for (Mahasiswa* p = head; p; p = p->next) if (p->nim == nim) return true;
        return false;
    }
    void tambah(long long nim, const string& nama, double nilai) {
        Mahasiswa* baru = new Mahasiswa{nim, nama, nilai, nullptr};
        if (!head) head = baru;
        else {
            Mahasiswa* p = head;
            while (p->next) p = p->next;
            p->next = baru;
        }
    }
    // Linear Search berdasarkan NIM
    Mahasiswa* linearSearch(long long nim) {
        for (Mahasiswa* p = head; p; p = p->next)
            if (p->nim == nim) return p;
        return nullptr;
    }
    void tampil() {
        if (!head) { cout << "Belum ada data.\n"; return; }
        cout << left << setw(5) << "No" << setw(15) << "NIM"
             << setw(25) << "Nama" << "Nilai\n";
        cout << string(52, '-') << "\n";
        int no = 1;
        for (Mahasiswa* p = head; p; p = p->next)
            cout << left << setw(5) << no++ << setw(15) << p->nim
                 << setw(25) << p->nama << fixed << setprecision(2) << p->nilai << "\n";
    }
    void tampilByNilai(double nilai) {
        for (Mahasiswa* p = head; p; p = p->next)
            if (p->nilai == nilai)
                cout << "   - " << p->nim << " | " << p->nama << "\n";
    }
};

int main() {
    DaftarMahasiswa daftar;
    DynamicArray nilai;
    bool terurut = false;
    int pilih;

    do {
        cout << "\n=== SISTEM PENGELOLAAN NILAI MAHASISWA ===\n"
             << "1. Tambah data mahasiswa\n"
             << "2. Tampilkan data mahasiswa\n"
             << "3. Urutkan nilai (Insertion Sort)\n"
             << "4. Cari mahasiswa berdasarkan NIM (Linear Search)\n"
             << "5. Cari nilai tertentu (Binary Search)\n"
             << "0. Keluar\n"
             << "Pilih: ";
        cin >> pilih;

        if (pilih == 1) {
            long long nim; string nama; double n;
            cout << "NIM   : "; cin >> nim;
            if (daftar.nimAda(nim)) { cout << "NIM sudah terdaftar!\n"; continue; }
            cout << "Nama  : "; cin.ignore(); getline(cin, nama);
            cout << "Nilai : "; cin >> n;
            daftar.tambah(nim, nama, n);
            nilai.tambah(n);
            terurut = false;
            cout << "Data berhasil ditambahkan.\n";
        }
        else if (pilih == 2) {
            daftar.tampil();
            cout << "\nArray nilai" << (terurut ? " (terurut)" : " (belum terurut)") << ": ";
            for (int i = 0; i < nilai.size(); i++) cout << nilai[i] << " ";
            cout << "\n";
        }
        else if (pilih == 3) {
            if (nilai.size() == 0) { cout << "Belum ada data.\n"; continue; }
            nilai.insertionSort();
            terurut = true;
            cout << "Nilai setelah diurutkan (kecil -> besar): ";
            for (int i = 0; i < nilai.size(); i++) cout << nilai[i] << " ";
            cout << "\n";
        }
        else if (pilih == 4) {
            long long nim;
            cout << "Masukkan NIM yang dicari: "; cin >> nim;
            Mahasiswa* m = daftar.linearSearch(nim);
            if (m) cout << "Ditemukan -> " << m->nim << " | " << m->nama
                        << " | Nilai: " << m->nilai << "\n";
            else cout << "Mahasiswa dengan NIM " << nim << " tidak ditemukan.\n";
        }
        else if (pilih == 5) {
            if (!terurut) { cout << "Urutkan nilai dulu (menu 3) sebelum Binary Search.\n"; continue; }
            double target;
            cout << "Masukkan nilai yang dicari: "; cin >> target;
            int idx = nilai.binarySearch(target);
            if (idx != -1) {
                cout << "Nilai " << target << " ditemukan pada indeks " << idx << " (array terurut).\n";
                cout << "Mahasiswa dengan nilai tersebut:\n";
                daftar.tampilByNilai(target);
            } else cout << "Nilai " << target << " tidak ditemukan.\n";
        }
    } while (pilih != 0);

    cout << "Program selesai.\n";
    return 0;
}
