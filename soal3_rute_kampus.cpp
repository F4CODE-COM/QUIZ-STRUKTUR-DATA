#include <iomanip>
#include <string>
#include <queue>
#include <vector>
#include <algorithm>
using namespace std;

const int N = 6;
const int INF = 1000000;
const char NAMA[N] = {'A', 'B', 'C', 'D', 'E', 'F'};

int graf[N][N]; 

void tambahSisi(int u, int v, int w) { graf[u][v] = graf[v][u] = w; }

void isiGraf() {
    for (int i = 0; i < N; i++) for (int j = 0; j < N; j++) graf[i][j] = 0;
    tambahSisi(0, 1, 4);  // A-B
    tambahSisi(0, 2, 2);  // A-C
    tambahSisi(1, 2, 1);  // B-C
    tambahSisi(1, 3, 5);  // B-D
    tambahSisi(1, 4, 3);  // B-E
    tambahSisi(2, 3, 8);  // C-D
    tambahSisi(2, 4, 6);  // C-E
    tambahSisi(3, 4, 2);  // D-E
    tambahSisi(3, 5, 4);  // D-F
    tambahSisi(4, 5, 3);  // E-F
}

void tampilMatrix() {
    cout << "\nAdjacency Matrix:\n    ";
    for (int j = 0; j < N; j++) cout << setw(3) << NAMA[j];
    cout << "\n";
    for (int i = 0; i < N; i++) {
        cout << setw(3) << NAMA[i] << " ";
        for (int j = 0; j < N; j++) cout << setw(3) << graf[i][j];
        cout << "\n";
    }
}

int bacaVertex(const string& prompt) {
    char c;
    while (true) {
        cout << prompt;
        cin >> c;
        c = toupper(c);
        if (c >= 'A' && c <= 'F') return c - 'A';
        cout << "Vertex tidak valid! Pilih A-F.\n";
    }
}

void bfs(int mulai) {
    bool dikunjungi[N] = {false};
    queue<int> q;
    dikunjungi[mulai] = true;
    q.push(mulai);
    cout << "BFS dari " << NAMA[mulai] << ": ";
    while (!q.empty()) {
        int u = q.front(); q.pop();
        cout << NAMA[u] << " ";
        for (int v = 0; v < N; v++)
            if (graf[u][v] && !dikunjungi[v]) {
                dikunjungi[v] = true;
                q.push(v);
            }
    }
    cout << "\n";
}

void dfsRek(int u, bool dikunjungi[]) {
    dikunjungi[u] = true;
    cout << NAMA[u] << " ";
    for (int v = 0; v < N; v++)
        if (graf[u][v] && !dikunjungi[v]) dfsRek(v, dikunjungi);
}

void dfs(int mulai) {
    bool dikunjungi[N] = {false};
    cout << "DFS dari " << NAMA[mulai] << ": ";
    dfsRek(mulai, dikunjungi);
    cout << "\n";
}

void dijkstra(int asal, int tujuan) {
    int jarak[N], induk[N];
    bool selesai[N] = {false};
    for (int i = 0; i < N; i++) { jarak[i] = INF; induk[i] = -1; }
    jarak[asal] = 0;

    for (int k = 0; k < N; k++) {
        int u = -1;
        for (int i = 0; i < N; i++)
            if (!selesai[i] && (u == -1 || jarak[i] < jarak[u])) u = i;
        if (u == -1 || jarak[u] == INF) break;
        selesai[u] = true;
        for (int v = 0; v < N; v++)
            if (graf[u][v] && jarak[u] + graf[u][v] < jarak[v]) {
                jarak[v] = jarak[u] + graf[u][v];
                induk[v] = u;
            }
    }

    cout << "\nJarak terpendek dari " << NAMA[asal] << " ke semua vertex:\n";
    for (int i = 0; i < N; i++)
        cout << "  " << NAMA[asal] << " -> " << NAMA[i] << " = " << jarak[i] << "\n";

    if (jarak[tujuan] == INF) {
        cout << "\nTidak ada rute dari " << NAMA[asal] << " ke " << NAMA[tujuan] << ".\n";
        return;
    }
    vector<int> rute;
    for (int v = tujuan; v != -1; v = induk[v]) rute.push_back(v);
    reverse(rute.begin(), rute.end());

    cout << "\nRute " << NAMA[asal] << " ke " << NAMA[tujuan] << ": ";
    for (size_t i = 0; i < rute.size(); i++) {
        cout << NAMA[rute[i]];
        if (i + 1 < rute.size()) cout << " -> ";
    }
    cout << "\nTotal jarak: " << jarak[tujuan] << "\n";
}

int main() {
    isiGraf();
    int pilih;
    do {
        cout << "\n=== SISTEM PENCARIAN RUTE KAMPUS ===\n"
             << "1. Tampilkan Adjacency Matrix\n"
             << "2. BFS\n"
             << "3. DFS\n"
             << "4. Rute terpendek (Dijkstra)\n"
             << "0. Keluar\n"
             << "Pilih: ";
        cin >> pilih;
        if (pilih == 1) tampilMatrix();
        else if (pilih == 2) bfs(bacaVertex("Vertex awal (A-F): "));
        else if (pilih == 3) dfs(bacaVertex("Vertex awal (A-F): "));
        else if (pilih == 4) {
            int a = bacaVertex("Vertex awal   (A-F): ");
            int t = bacaVertex("Vertex tujuan (A-F): ");
            dijkstra(a, t);
        }
    } while (pilih != 0);
    cout << "Program selesai.\n";
    return 0;
}
