#include <stdio.h>
#include <stdlib.h>
#include "bookstore.h"

void pause() {
    printf("\nTekan Enter untuk kembali ke menu...");
    getchar(); // tangkap newline sisa
    getchar(); // tunggu input
}

int main() {
    loadDataBuku();
    loadDataHistory();

    int pilihan;

    do {
        system("cls"); // Membersihkan layar terminal
        printf("=========================================\n");
        printf("  TOKO BUKU LITERASI NUSANTARA JAYA\n");
        printf("=========================================\n");
        printf("1. Input Data Buku Baru\n");
        printf("2. View History Transaksi Penjualan\n");
        printf("3. View Buku (Seluruh Data)\n");
        printf("4. Delete History (Hapus Histori Penjualan)\n");
        printf("5. Delete Buku (Hapus Data Buku)\n");
        printf("6. Exit (Simpan data & keluar)\n");
        printf("7. Input Transaksi Penjualan\n");
        printf("=========================================\n");
        printf("Pilih Menu (1-7): ");
        
        if (scanf("%d", &pilihan) != 1) {
            printf("Input tidak valid. Masukkan angka.\n");
            while (getchar() != '\n'); 
            pause();
            continue;
        }

        switch (pilihan) {
            case 1: inputDataBuku(); pause(); break;
            case 2: viewHistory(); pause(); break;
            case 3: viewBuku(); pause(); break;
            case 4: deleteHistory(); pause(); break;
            case 5: deleteBuku(); pause(); break;
            case 6: 
                saveData(); 
                printf("Data berhasil disimpan. Terima kasih!\n");
                break;
            case 7: inputTransaksi(); pause(); break;
            default: printf("Pilihan menu tidak valid!\n"); pause();
        }
    } while (pilihan != 6);

    return 0;
}
