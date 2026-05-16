#ifndef BOOKSTORE_H
#define BOOKSTORE_H

#define MAX_BUKU 100
#define MAX_HISTORY 100

// Struktur untuk Buku
typedef struct {
    char kodeBuku[20];
    char namaBuku[100];
    char jenisBuku[50];
    double hargaBuku;
} Buku;

// Struktur untuk History
typedef struct {
    char kodeBuku[20];
    int jumlahTerjual;
    double totalHarga;
} History;

// Global variables (extern agar bisa diakses di file lain)
extern Buku daftarBuku[MAX_BUKU];
extern int jumlahBuku;
extern History daftarHistory[MAX_HISTORY];
extern int jumlahHistory;

// Deklarasi Fungsi
void loadDataBuku();
void loadDataHistory();
void saveData();
void inputDataBuku();
void viewHistory();
void viewBuku();
void deleteHistory();
void deleteBuku();
void inputTransaksi();

#endif
