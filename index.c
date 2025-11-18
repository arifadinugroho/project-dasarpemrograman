#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAMA_FILE "perpustakaan.txt"

typedef struct {
    char kode_buku[10];
    char judul[100];
    char penulis[50];
    int tahun_terbit;
    int status; // 1 = Tersedia, 0 = Dipinjam
} Buku;

#define MAX_BUKU 100
Buku daftarBuku[MAX_BUKU];
int jumlahBuku = 0;

void tampilan();
void muat_data();
void simpan_data();
void tambah_buku();
void tampil_buku();

int main() {
    muat_data();
    tampilan();
    simpan_data();
}

// Function untuk mengambil data buku dari file .txt
void muat_data() {
    FILE *file = fopen(NAMA_FILE, "r");
    if (file == NULL) {
        printf("File '%s' tidak ditemukan. Membuat file baru.\n", NAMA_FILE);
        return;
    }

    jumlahBuku = 0;
    while (jumlahBuku < MAX_BUKU && 
           fscanf(file, "%9[^|]|%99[^|]|%49[^|]|%d|%d\n", 
                  daftarBuku[jumlahBuku].kode_buku,
                  daftarBuku[jumlahBuku].judul,
                  daftarBuku[jumlahBuku].penulis,
                  &daftarBuku[jumlahBuku].tahun_terbit,
                  &daftarBuku[jumlahBuku].status) == 5) {
        jumlahBuku++;
    }
    
    fclose(file);
    printf("Data %d buku berhasil dimuat dari '%s'.\n", jumlahBuku, NAMA_FILE);
}

// Function untuk menyimpan data buku ke file .txt
void simpan_data() {
    FILE *file = fopen(NAMA_FILE, "w");
    if (file == NULL) {
        printf("Gagal membuka file '%s' untuk penyimpanan.\n", NAMA_FILE);
        return;
    }

    for (int i = 0; i < jumlahBuku; i++) {
        fprintf(file, "%s|%s|%s|%d|%d\n",
                daftarBuku[i].kode_buku,
                daftarBuku[i].judul,
                daftarBuku[i].penulis,
                daftarBuku[i].tahun_terbit,
                daftarBuku[i].status);
    }

    fclose(file);
    printf("\nData %d buku berhasil disimpan ke '%s'.\n", jumlahBuku, NAMA_FILE);
}

// Function untuk menampilkan daftar menu
void tampilan() {
    int pilihan;
    do {
        printf("\n===================================\n");
        printf("       PERPUSTAKAAN GIMANG\n");
        printf("===================================\n");
        printf("1. Tambah Data Buku\n");
        printf("2. Tampilkan Semua Data Buku\n");
        printf("3. Keluar dan Simpan\n");
        printf("Pilihan: ");
        
        if (scanf("%d", &pilihan) != 1) {
             printf("Input tidak valid. Silakan masukkan angka.\n");
             while (getchar() != '\n');
             pilihan = 0;
             continue;
        }

        while (getchar() != '\n');

        switch (pilihan) {
            case 1:
                tambah_buku();
                simpan_data();
                break;
            case 2:
                tampil_buku();
                break;
            case 3:
                printf("Terima kasih.\n");
                break;
            default:
                printf("Pilihan tidak valid.\n");
        }
    } while (pilihan != 3);
}

// Function untuk menambah data buku baru
void tambah_buku() {
    if (jumlahBuku >= MAX_BUKU) {
        printf("\nKapasitas maksimum buku telah tercapai.\n");
        return;
    }

    printf("\n--- TAMBAH DATA BUKU BARU ---\n");
    printf("Kode Buku (diawali 'KB'): ");
    fgets(daftarBuku[jumlahBuku].kode_buku, 10, stdin);
    daftarBuku[jumlahBuku].kode_buku[strcspn(daftarBuku[jumlahBuku].kode_buku, "\n")] = 0;

    printf("Judul: ");
    fgets(daftarBuku[jumlahBuku].judul, 100, stdin);
    daftarBuku[jumlahBuku].judul[strcspn(daftarBuku[jumlahBuku].judul, "\n")] = 0;

    printf("Penulis: ");
    fgets(daftarBuku[jumlahBuku].penulis, 50, stdin);
    daftarBuku[jumlahBuku].penulis[strcspn(daftarBuku[jumlahBuku].penulis, "\n")] = 0;

    printf("Tahun Terbit: ");
    if (scanf("%d", &daftarBuku[jumlahBuku].tahun_terbit) != 1) {
        printf("\nInput tahun tidak valid.\n");
        while (getchar() != '\n');
        return;
    }
    while (getchar() != '\n');

    daftarBuku[jumlahBuku].status = 1; 

    jumlahBuku++;
    printf("\nBuku berhasil ditambahkan. Status default: Tersedia.\n");
}

// Function untuk menampilkan data buku
void tampil_buku() {
    if (jumlahBuku == 0) {
        printf("\nBelum ada data buku.\n");
        return;
    }

    printf("\n--- DAFTAR SEMUA BUKU ---\n", jumlahBuku);
    printf("--------------------------------------------------------------------------------------------------------------------------------\n");
    printf("| %-8s | %-40s | %-20s | %-12s | %-10s |\n", 
           "KODE", "JUDUL", "PENULIS", "TAHUN TERBIT", "STATUS");
    printf("--------------------------------------------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < jumlahBuku; i++) {
        char *status_text = (daftarBuku[i].status == 1) ? "Tersedia" : "Dipinjam";
        printf("| %-8s | %-40s | %-20s | %-12d | %-10s |\n",
               daftarBuku[i].kode_buku,
               daftarBuku[i].judul,
               daftarBuku[i].penulis,
               daftarBuku[i].tahun_terbit,
               status_text);
    }
    printf("--------------------------------------------------------------------------------------------------------------------------------\n");
}
