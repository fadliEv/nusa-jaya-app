#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bookstore.h"

// Definisi variabel global
Buku daftarBuku[MAX_BUKU];
int jumlahBuku = 0;
History daftarHistory[MAX_HISTORY];
int jumlahHistory = 0;

// Fungsi helper untuk memformat angka ke Rupiah dengan pemisah ribuan
void formatRupiah(double amount, char *output) {
    char temp[50];
    sprintf(temp, "%.2f", amount);
    
    char *dot = strchr(temp, '.');
    int len = (int)(dot - temp);
    
    int outIdx = 0;
    for (int i = 0; i < len; i++) {
        if (i > 0 && (len - i) % 3 == 0) {
            output[outIdx++] = ',';
        }
        output[outIdx++] = temp[i];
    }
    strcpy(output + outIdx, dot);
}

void loadDataBuku() {
    FILE *file = fopen("databuku.txt", "r");
    
    if (file == NULL) {
        // Jika tidak ada, buat file baru dengan 15 data dummy
        file = fopen("databuku.txt", "w");
        if (file) {
            char *jenis[] = {"Pendidikan", "Novel", "Komik", "Teknologi", "Sejarah"};
            for (int i = 1; i <= 15; i++) {
                fprintf(file, "BK%03d|Buku Literasi %d|%s|%.2f\n", i, i, jenis[i % 5], (double)(50000 + (i * 2000)));
            }
            fclose(file);
        }
        printf("[System] databuku.txt baru dibuat dengan 15 data dummy.\n");
        // Re-open to read the dummy data
        file = fopen("databuku.txt", "r");
    }

    char buffer[256];
    jumlahBuku = 0;
    while (fgets(buffer, sizeof(buffer), file)) {
        buffer[strcspn(buffer, "\n")] = 0;
        char *token = strtok(buffer, "|");
        if (token) strcpy(daftarBuku[jumlahBuku].kodeBuku, token);
        
        token = strtok(NULL, "|");
        if (token) strcpy(daftarBuku[jumlahBuku].namaBuku, token);
        
        token = strtok(NULL, "|");
        if (token) strcpy(daftarBuku[jumlahBuku].jenisBuku, token);
        
        token = strtok(NULL, "|");
        if (token) daftarBuku[jumlahBuku].hargaBuku = atof(token);
        
        jumlahBuku++;
    }
    fclose(file);
    printf("[System] Berhasil memuat %d data buku.\n", jumlahBuku);
}

void loadDataHistory() {
    FILE *file = fopen("history.txt", "r");
    
    if (file == NULL) {
        // Jika tidak ada, buat file baru dengan 5 data dummy
        file = fopen("history.txt", "w");
        if (file) {
            for (int i = 1; i <= 5; i++) {
                double harga = (double)(50000 + (i * 2000));
                fprintf(file, "BK%03d|%d|%.2f\n", i, (i % 3) + 1, harga * ((i % 3) + 1));
            }
            fclose(file);
        }
        printf("[System] history.txt baru dibuat dengan 5 data dummy.\n");
        file = fopen("history.txt", "r");
    }

    char buffer[256];
    jumlahHistory = 0;
    while (fgets(buffer, sizeof(buffer), file)) {
        buffer[strcspn(buffer, "\n")] = 0;
        char *token = strtok(buffer, "|");
        if (token) strcpy(daftarHistory[jumlahHistory].kodeBuku, token);
        
        token = strtok(NULL, "|");
        if (token) daftarHistory[jumlahHistory].jumlahTerjual = atoi(token);
        
        token = strtok(NULL, "|");
        if (token) daftarHistory[jumlahHistory].totalHarga = atof(token);
        
        jumlahHistory++;
    }
    fclose(file);
    printf("[System] Berhasil memuat %d data histori.\n", jumlahHistory);
}

void saveData() {
    FILE *fileBuku = fopen("databuku.txt", "w");
    if (fileBuku != NULL) {
        for (int i = 0; i < jumlahBuku; i++) {
            fprintf(fileBuku, "%s|%s|%s|%.2f\n", daftarBuku[i].kodeBuku, daftarBuku[i].namaBuku, daftarBuku[i].jenisBuku, daftarBuku[i].hargaBuku);
        }
        fclose(fileBuku);
    }

    FILE *fileHistory = fopen("history.txt", "w");
    if (fileHistory != NULL) {
        for (int i = 0; i < jumlahHistory; i++) {
            fprintf(fileHistory, "%s|%d|%.2f\n", daftarHistory[i].kodeBuku, daftarHistory[i].jumlahTerjual, daftarHistory[i].totalHarga);
        }
        fclose(fileHistory);
    }
}

void inputDataBuku() {
    if (jumlahBuku >= MAX_BUKU) {
        printf("Kapasitas penyimpanan buku penuh!\n");
        return;
    }

    Buku bukuBaru;
    int nextId = 1;
    if (jumlahBuku > 0) {
        // Mencari ID tertinggi agar auto-increment tetap rapi
        int maxId = 0;
        for(int i = 0; i < jumlahBuku; i++) {
            int currentId;
            sscanf(daftarBuku[i].kodeBuku, "BK%d", &currentId);
            if(currentId > maxId) maxId = currentId;
        }
        nextId = maxId + 1;
    }
    sprintf(bukuBaru.kodeBuku, "BK%03d", nextId);

    printf("\n=== INPUT BUKU BARU ===\n");
    printf("Kode Buku (Otomatis): %s\n", bukuBaru.kodeBuku);
    
    printf("Masukkan Nama Buku: ");
    getchar(); 
    fgets(bukuBaru.namaBuku, sizeof(bukuBaru.namaBuku), stdin);
    bukuBaru.namaBuku[strcspn(bukuBaru.namaBuku, "\n")] = 0;

    // Sistem Kategori/Jenis Buku
    int pilihanJenis;
    char *listJenis[] = {"Sejarah", "Konspirasi", "Edukasi", "Novel", "Komik", "Teknologi"};
    while(1) {
        printf("\nPilih Jenis Buku:\n");
        for(int i = 0; i < 6; i++) {
            printf("%d. %s\n", i + 1, listJenis[i]);
        }
        printf("Masukkan pilihan (1-6): ");
        if (scanf("%d", &pilihanJenis) != 1 || pilihanJenis < 1 || pilihanJenis > 6) {
            printf("\n[Warning] Jenis Buku tidak ditemukan! Silakan pilih angka 1 sampai 6.\n");
            while (getchar() != '\n'); // clear buffer
        } else {
            strcpy(bukuBaru.jenisBuku, listJenis[pilihanJenis - 1]);
            break;
        }
    }

    printf("Masukkan Harga Buku: ");
    scanf("%lf", &bukuBaru.hargaBuku);

    daftarBuku[jumlahBuku] = bukuBaru;
    jumlahBuku++;

    char hargaFormatted[50];
    formatRupiah(bukuBaru.hargaBuku, hargaFormatted);

    printf("\n[Sukses] Data Buku Berhasil Ditambahkan!\n");
    printf("Preview Data Baru:\n");
    printf("----------------------------------\n");
    printf("Kode Buku  : %s\n", bukuBaru.kodeBuku);
    printf("Nama Buku  : %s\n", bukuBaru.namaBuku);
    printf("Jenis Buku : %s\n", bukuBaru.jenisBuku);
    printf("Harga Buku : Rp %s\n", hargaFormatted);
    printf("----------------------------------\n");
}

void viewHistory() {
    printf("\n=== HISTORI TRANSAKSI PENJUALAN ===\n");
    if (jumlahHistory == 0) {
        printf("Belum ada data histori transaksi.\n");
        return;
    }
    printf("No | Kode Buku | Jumlah Terjual | Total Harga\n");
    printf("---------------------------------------------\n");
    for (int i = 0; i < jumlahHistory; i++) {
        char totalFormatted[50];
        formatRupiah(daftarHistory[i].totalHarga, totalFormatted);
        printf("%-2d | %-9s | %-14d | Rp %s\n", i + 1, daftarHistory[i].kodeBuku, daftarHistory[i].jumlahTerjual, totalFormatted);
    }
}

void viewBuku() {
    printf("\n=== DAFTAR BUKU (Terbaru di Atas) ===\n");
    if (jumlahBuku == 0) {
        printf("Belum ada data buku tersedia.\n");
        return;
    }
    printf("No | Kode Buku | Nama Buku                     | Jenis Buku      | Harga\n");
    printf("-----------------------------------------------------------------------\n");
    // Looping terbalik (Descending) agar data terbaru ada di paling atas
    for (int i = jumlahBuku - 1; i >= 0; i--) {
        char hargaFormatted[50];
        formatRupiah(daftarBuku[i].hargaBuku, hargaFormatted);
        printf("%-2d | %-9s | %-29s | %-15s | Rp %s\n", jumlahBuku - i, daftarBuku[i].kodeBuku, daftarBuku[i].namaBuku, daftarBuku[i].jenisBuku, hargaFormatted);
    }
}

void deleteHistory() {
    if (jumlahHistory == 0) {
        printf("\nBelum ada histori transaksi untuk dihapus.\n");
        return;
    }
    viewHistory();
    int index;
    printf("Masukkan index histori yang ingin dihapus (1 - %d): ", jumlahHistory);
    scanf("%d", &index);

    if (index >= 1 && index <= jumlahHistory) {
        for (int i = index - 1; i < jumlahHistory - 1; i++) {
            daftarHistory[i] = daftarHistory[i + 1];
        }
        jumlahHistory--;
        printf("Data Successfully delete..\n");
    } else {
        printf("Index tidak valid!\n");
    }
}

void deleteBuku() {
    if (jumlahBuku == 0) {
        printf("\nBelum ada buku untuk dihapus.\n");
        return;
    }
    viewBuku();
    int index;
    printf("Masukkan index buku yang ingin dihapus (1 - %d): ", jumlahBuku);
    scanf("%d", &index);

    if (index >= 1 && index <= jumlahBuku) {
        for (int i = index - 1; i < jumlahBuku - 1; i++) {
            daftarBuku[i] = daftarBuku[i + 1];
        }
        jumlahBuku--;
        printf("Data Successfully delete..\n");
    } else {
        printf("Index tidak valid!\n");
    }
}

void inputTransaksi() {
    if (jumlahBuku == 0) {
        printf("\nBelum ada data buku. Silakan input buku terlebih dahulu.\n");
        return;
    }

    // Menampilkan daftar buku terlebih dahulu agar user tidak lupa kode bukunya
    viewBuku();

    if (jumlahHistory >= MAX_HISTORY) {
        printf("\nKapasitas histori penuh!\n");
        return;
    }

    char kodeInput[20];
    int jumlahTerjual;
    printf("\n=== INPUT TRANSAKSI PENJUALAN ===\n");
    printf("Masukkan Kode Buku: ");
    scanf("%s", kodeInput);

    int indexBuku = -1;
    for (int i = 0; i < jumlahBuku; i++) {
        if (strcmp(daftarBuku[i].kodeBuku, kodeInput) == 0) {
            indexBuku = i;
            break;
        }
    }

    if (indexBuku == -1) {
        printf("Buku dengan kode %s tidak ditemukan.\n", kodeInput);
        return;
    }

    printf("Masukkan Jumlah Terjual: ");
    scanf("%d", &jumlahTerjual);

    if (jumlahTerjual <= 0) {
        printf("Jumlah tidak valid!\n");
        return;
    }

    double total = daftarBuku[indexBuku].hargaBuku * jumlahTerjual;
    strcpy(daftarHistory[jumlahHistory].kodeBuku, kodeInput);
    daftarHistory[jumlahHistory].jumlahTerjual = jumlahTerjual;
    daftarHistory[jumlahHistory].totalHarga = total;
    jumlahHistory++;

    char totalFormatted[50];
    formatRupiah(total, totalFormatted);
    printf("Transaksi berhasil dicatat. Total Harga: Rp %s\n", totalFormatted);
}
