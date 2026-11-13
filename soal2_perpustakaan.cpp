// Soal 2: Sistem Antrean dan Pengelolaan Buku Perpustakaan
// Queue (FIFO) antrean peminjam, Stack (LIFO) riwayat pengembalian, BST ID buku
#include <iostream>
#include <string>
using namespace std;

// ---------- Queue (Linked List, FIFO) ----------
struct NodeQ { string nama; NodeQ* next; };
class Queue {
    NodeQ *depan, *belakang;
public:
    Queue() : depan(nullptr), belakang(nullptr) {}
    ~Queue() { while (depan) dequeue(); }
    bool kosong() const { return depan == nullptr; }
    void enqueue(const string& nama) {
        NodeQ* baru = new NodeQ{nama, nullptr};
        if (kosong()) depan = belakang = baru;
        else { belakang->next = baru; belakang = baru; }
    }
    string dequeue() {
        NodeQ* t = depan;
        string nama = t->nama;
        depan = depan->next;
        if (!depan) belakang = nullptr;
        delete t;
        return nama;
    }
    void tampil() const {
        if (kosong()) { cout << "(antrean kosong)\n"; return; }
        cout << "DEPAN -> ";
        for (NodeQ* p = depan; p; p = p->next) cout << p->nama << " -> ";
        cout << "BELAKANG\n";
    }
};

// ---------- Stack (Linked List, LIFO) ----------
struct NodeS { int idBuku; string judul; NodeS* next; };
class Stack {
    NodeS* top;
public:
    Stack() : top(nullptr) {}
    ~Stack() { while (top) pop(); }
    bool kosong() const { return top == nullptr; }
    void push(int id, const string& judul) { top = new NodeS{id, judul, top}; }
    void pop() { NodeS* t = top; top = top->next; delete t; }
    void tampil() const {
        if (kosong()) { cout << "(riwayat kosong)\n"; return; }
        cout << "TOP (terbaru)\n";
        for (NodeS* p = top; p; p = p->next)
            cout << "  | ID " << p->idBuku << " - " << p->judul << "\n";
        cout << "BOTTOM (terlama)\n";
    }
};

// ---------- Binary Search Tree ID Buku ----------
struct NodeT { int id; NodeT *kiri, *kanan; };
class BST {
    NodeT* root;
    NodeT* sisip(NodeT* n, int id, bool& ok) {
        if (!n) { ok = true; return new NodeT{id, nullptr, nullptr}; }
        if (id < n->id) n->kiri = sisip(n->kiri, id, ok);
        else if (id > n->id) n->kanan = sisip(n->kanan, id, ok);
        return n;
    }
    void preOrder(NodeT* n) const { if (n) { cout << n->id << " "; preOrder(n->kiri); preOrder(n->kanan); } }
    void inOrder(NodeT* n) const { if (n) { inOrder(n->kiri); cout << n->id << " "; inOrder(n->kanan); } }
    void postOrder(NodeT* n) const { if (n) { postOrder(n->kiri); postOrder(n->kanan); cout << n->id << " "; } }
    void hapus(NodeT* n) { if (n) { hapus(n->kiri); hapus(n->kanan); delete n; } }
public:
    BST() : root(nullptr) {}
    ~BST() { hapus(root); }
    bool insert(int id) { bool ok = false; root = sisip(root, id, ok); return ok; }
    bool cari(int id) const {
        NodeT* p = root;
        while (p) {
            if (id == p->id) return true;
            p = (id < p->id) ? p->kiri : p->kanan;
        }
        return false;
    }
    bool kosong() const { return root == nullptr; }
    void pre() const { cout << "Pre-Order  : "; preOrder(root); cout << "\n"; }
    void in() const { cout << "In-Order   : "; inOrder(root); cout << "\n"; }
    void post() const { cout << "Post-Order : "; postOrder(root); cout << "\n"; }
};

int main() {
    Queue antrean;
    Stack riwayat;
    BST bst;
    int pilih;

    do {
        cout << "\n=== SISTEM PERPUSTAKAAN ===\n"
             << "1. Tambah ID buku (BST)\n"
             << "2. Cari ID buku (BST)\n"
             << "3. Tampilkan isi BST (Pre/In/Post-Order)\n"
             << "4. Mahasiswa masuk antrean peminjaman (Enqueue)\n"
             << "5. Layani peminjam terdepan (Dequeue)\n"
             << "6. Tampilkan antrean\n"
             << "7. Kembalikan buku (Push ke riwayat)\n"
             << "8. Batalkan pengembalian terakhir (Pop)\n"
             << "9. Tampilkan riwayat pengembalian\n"
             << "0. Keluar\n"
             << "Pilih: ";
        cin >> pilih;

        if (pilih == 1) {
            int id; cout << "ID buku: "; cin >> id;
            if (bst.insert(id)) cout << "ID " << id << " ditambahkan ke BST.\n";
            else cout << "ID sudah ada.\n";
        }
        else if (pilih == 2) {
            int id; cout << "ID buku yang dicari: "; cin >> id;
            cout << "ID " << id << (bst.cari(id) ? " ditemukan.\n" : " tidak ditemukan.\n");
        }
        else if (pilih == 3) {
            if (bst.kosong()) cout << "BST kosong.\n";
            else { bst.pre(); bst.in(); bst.post(); }
        }
        else if (pilih == 4) {
            string nama; cout << "Nama mahasiswa: "; cin.ignore(); getline(cin, nama);
            antrean.enqueue(nama);
            cout << nama << " masuk antrean.\n";
            antrean.tampil();
        }
        else if (pilih == 5) {
            if (antrean.kosong()) cout << "Antrean kosong.\n";
            else {
                cout << antrean.dequeue() << " sedang dilayani (keluar dari antrean).\n";
                antrean.tampil();
            }
        }
        else if (pilih == 6) antrean.tampil();
        else if (pilih == 7) {
            int id; string judul;
            cout << "ID buku   : "; cin >> id;
            cout << "Judul buku: "; cin.ignore(); getline(cin, judul);
            riwayat.push(id, judul);
            cout << "Pengembalian dicatat.\n";
            riwayat.tampil();
        }
        else if (pilih == 8) {
            if (riwayat.kosong()) cout << "Riwayat kosong.\n";
            else { riwayat.pop(); cout << "Pengembalian terakhir dibatalkan.\n"; riwayat.tampil(); }
        }
        else if (pilih == 9) riwayat.tampil();
    } while (pilih != 0);

    cout << "Program selesai.\n";
    return 0;
}
