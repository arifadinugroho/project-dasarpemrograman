#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NAMA_FILE "perpustakaan.txt"
#define NAMA_FILE2 "peminjam.txt"

typedef struct {
    char judul[100];
    char penulis[50];
    char genre[50]; 
    char tanggal_pinjam[11];  // DD/MM/YYYY untuk menghitung denda
    int tahun_terbit;
    int status; // 1 = Tersedia, 0 = Dipinjam
} Buku;

#define MAX_BUKU 100
Buku daftarBuku[MAX_BUKU];
int jumlahBuku = 0;

// Menghitung jumlah hari 
int tanggal_ke_hari(char *tanggal) {
    int d, m, y;
    sscanf(tanggal, "%d/%d/%d", &d, &m, &y);
    int hari = d;
    int bulan_hari[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    for (int i = 1; i < m; i++) {
        hari += bulan_hari[i];
        if (i == 2 && y % 4 == 0) hari++;  
    }
    hari += (y - 1900) * 365 + (y - 1900) / 4;  
    return hari;
}

// Menghitung selisih hari antara dua tanggal (DD/MM/YYYY)
int hitung_selisih_hari(char *tanggal1, char *tanggal2) {
    return tanggal_ke_hari(tanggal2) - tanggal_ke_hari(tanggal1);
}

typedef struct {
    char nama[50];
    char nim[15];
    char judul_buku[50];
    char tanggal_peminjaman[20];
    char tanggal_pengembalian[20];
    int denda;
} Peminjam;

#define MAX_PEMINJAM 100
Peminjam daftarPeminjam[MAX_PEMINJAM];
int jumlahPeminjam = 0;

void tampilan();
void muat_data_buku();
void simpan_data_buku();
void muat_data_peminjam();
void simpan_data_peminjam();
void tambah_buku();
void tampil_buku();
void edit_buku();
void tampil_peminjam();
void tambah_peminjam();
void kembalikan_buku();
void sorting_data();
void searching_data();

int main() {
    muat_data_buku();
    muat_data_peminjam();
    tampilan();
    simpan_data_buku();
    simpan_data_peminjam();
    return 0;
}

// Function untuk mengambil data buku dari file perpustakaan.txt
void muat_data_buku() {
    FILE *file = fopen(NAMA_FILE, "r");
    if (file == NULL) {
        printf("File '%s' tidak ditemukan. Membuat file baru.\n", NAMA_FILE);
        return;
    }

    jumlahBuku = 0;
    while (jumlahBuku < MAX_BUKU &&
           fscanf(file, "%99[^|]|%49[^|]|%d|%d\n",
                  daftarBuku[jumlahBuku].judul,
                  daftarBuku[jumlahBuku].penulis,
                  &daftarBuku[jumlahBuku].tahun_terbit,
                  &daftarBuku[jumlahBuku].status) == 4) {
        jumlahBuku++;
    }

    fclose(file);
    printf("Data %d buku berhasil dimuat dari '%s'.\n", jumlahBuku, NAMA_FILE);
}

// Function untuk menyimpan data buku ke file perpustakaan.txt
void simpan_data_buku() {
    FILE *file = fopen(NAMA_FILE, "w");
    if (file == NULL) {
        printf("Gagal membuka file '%s' untuk penyimpanan.\n", NAMA_FILE);
        return;
    }

    for (int i = 0; i < jumlahBuku; i++) {
        fprintf(file, "%s|%s|%d|%s|%d\n",
                daftarBuku[i].judul,
                daftarBuku[i].penulis,
                daftarBuku[i].tahun_terbit,
                daftarBuku[i].genre,
                daftarBuku[i].status);
    }

    fclose(file);
    printf("\nData %d buku berhasil disimpan ke '%s'.\n", jumlahBuku, NAMA_FILE);
}

// Function untuk mengambil data peminjam dari file peminjam.txt
void muat_data_peminjam() {
    FILE *file = fopen(NAMA_FILE2, "r");
    if (file == NULL) {
        printf("File '%s' tidak ditemukan. Membuat file baru.\n", NAMA_FILE2);
        return;
    }

    jumlahPeminjam = 0;
    while (jumlahPeminjam < MAX_PEMINJAM &&
           fscanf(file, "%49[^|]|%14[^|]|%49[^|]|%19[^|]|%19[^|]|%d\n",
                  daftarPeminjam[jumlahPeminjam].nama,
                  daftarPeminjam[jumlahPeminjam].nim,
                  daftarPeminjam[jumlahPeminjam].judul_buku,
                  daftarPeminjam[jumlahPeminjam].tanggal_peminjaman,
                  daftarPeminjam[jumlahPeminjam].tanggal_pengembalian,
                  &daftarPeminjam[jumlahPeminjam].denda) == 6) {
        jumlahPeminjam++;
    }

    fclose(file);
    printf("Data %d peminjam berhasil dimuat dari '%s'.\n", jumlahPeminjam, NAMA_FILE2);
}

// Function untuk menyimpan data buku ke file peminjam.txt
void simpan_data_peminjam() {
    FILE *file = fopen(NAMA_FILE2, "w");
    if (file == NULL) {
        printf("Gagal membuka file '%s' untuk penyimpanan.\n", NAMA_FILE2);
        return;
    }

    for (int i = 0; i < jumlahPeminjam; i++) {
        fprintf(file, "%s|%s|%s|%s|%s|%d\n",
                daftarPeminjam[i].nama,
                daftarPeminjam[i].nim,
                daftarPeminjam[i].judul_buku,
                daftarPeminjam[i].tanggal_peminjaman,
                daftarPeminjam[i].tanggal_pengembalian,
                daftarPeminjam[i].denda);
    }

    fclose(file);
    printf("\nData %d peminjam berhasil disimpan ke '%s'.\n", jumlahPeminjam, NAMA_FILE2);
}

// Function untuk menampilkan daftar menu
void tampilan() {
    int pilihan;
    do {
        printf("\n=== MENU PERPUSTAKAAN ===\n");
        printf("1. Tambah Data Buku\n");
        printf("2. Tampilkan Semua Data Buku\n");
        printf("3. Edit Data Buku\n");
        printf("4. Tambah Data Peminjam\n");
        printf("5. Tampilkan Data Peminjam\n");
        printf("6. Mengembalikan Buku\n");
        printf("7. Sorting Data Buku atau Data Peminjam\n");
        printf("8. Searching Data Buku atau Data Peminjam\n");
        printf("9. Keluar dan Simpan\n");
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
                simpan_data_buku();
                break;
            case 2:
                tampil_buku();
                break;
            case 3:
                edit_buku();
                simpan_data_buku();
                break;
            case 4:
                tambah_peminjam();
                simpan_data_peminjam();
                simpan_data_buku(); // update status buku jika dipinjam
                break;
            case 5:
                tampil_peminjam();
                break;
            case 6:
                kembalikan_buku();
                simpan_data_buku();
                break;
            case 7:
                sorting_data();
                break;
            case 8:
                searching_data();
                break;
            case 9:
                printf("Terima kasih.\n");
                break;
            default:
                printf("Pilihan tidak valid.\n");
        }
    } while (pilihan != 9);
}

// Function untuk menambah data buku baru
void tambah_buku() {
    if (jumlahBuku >= MAX_BUKU) {
        printf("\nKapasitas maksimum buku telah tercapai.\n");
        return;
    }

    printf("\n--- TAMBAH DATA BUKU BARU ---\n");

    printf("Judul: ");
    fgets(daftarBuku[jumlahBuku].judul, sizeof(daftarBuku[jumlahBuku].judul), stdin);
    daftarBuku[jumlahBuku].judul[strcspn(daftarBuku[jumlahBuku].judul, "\n")] = 0;

    printf("Penulis: ");
    fgets(daftarBuku[jumlahBuku].penulis, sizeof(daftarBuku[jumlahBuku].penulis), stdin);
    daftarBuku[jumlahBuku].penulis[strcspn(daftarBuku[jumlahBuku].penulis, "\n")] = 0;

    printf("Genre: ");
    fgets(daftarBuku[jumlahBuku].genre, sizeof(daftarBuku[jumlahBuku].genre), stdin);
    daftarBuku[jumlahBuku].genre[strcspn(daftarBuku[jumlahBuku].genre, "\n")] = 0;

    printf("Tahun Terbit: ");
    if (scanf("%d", &daftarBuku[jumlahBuku].tahun_terbit) != 1) {
        printf("\nInput tahun tidak valid.\n");
        while (getchar() != '\n');
        return;
    }
    while (getchar() != '\n');

    daftarBuku[jumlahBuku].status = 1;
    strcpy(daftarBuku[jumlahBuku].tanggal_pinjam, "00/00/0000");  
    
    jumlahBuku++;
    printf("\nBuku berhasil ditambahkan. Status default: Tersedia.\n");
}

// Function untuk menampilkan data buku
void tampil_buku() {
    if (jumlahBuku == 0) {
        printf("\nBelum ada data buku.\n");
        return;
    }

    printf("\n--- DAFTAR SEMUA BUKU ---\n");
    printf("-----------------------------------------------------------------------------------------------------------\n");
    printf("| %-40s | %-20s | %-12s | %-10s | %-10s |\n",
           "Judul", "Penulis", "Tahun Terbit", "Genre", "Status");
    printf("-----------------------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < jumlahBuku; i++) {
        char *status_text = (daftarBuku[i].status == 1) ? "Tersedia" : "Dipinjam";
        printf("| %-40s | %-20s | %-12d | %-10s | %-10s |\n",
               daftarBuku[i].judul,
               daftarBuku[i].penulis,
               daftarBuku[i].tahun_terbit,
               daftarBuku[i].genre,
               status_text);
    }
    printf("-----------------------------------------------------------------------------------------------------------\n");
}

// Function untuk mengedit data buku berdasarkan kode buku
void edit_buku() {
    if (jumlahBuku == 0) {
        printf("Belum ada data buku untuk diedit.\n");
        return;
    }

    tampil_buku();
    char cariJudulBuku[100];
    printf("Masukkan Judul buku yang ingin diedit: ");
    fgets(cariJudulBuku, sizeof(cariJudulBuku), stdin);
    cariJudulBuku[strcspn(cariJudulBuku, "\n")] = '\0';

    int index = -1;

    // Mencari buku berdasarkan judulnya
    for (int i = 0; i < jumlahBuku; i++) {
        if (strcmp(daftarBuku[i].judul, cariJudulBuku) == 0) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("Buku dengan judul %s tidak ditemukan\n", cariJudulBuku);
        return;
    }

    // Jika buku ditemukan
    printf("Masukkan Judul Baru: ");
    fgets(daftarBuku[index].judul, sizeof(daftarBuku[index].judul), stdin);
    daftarBuku[index].judul[strcspn(daftarBuku[index].judul, "\n")] = 0;

    printf("Masukkan Nama Penulis Baru: ");
    fgets(daftarBuku[index].penulis, sizeof(daftarBuku[index].penulis), stdin);
    daftarBuku[index].penulis[strcspn(daftarBuku[index].penulis, "\n")] = 0;

    printf("Masukkan Genre Baru: ");
    fgets(daftarBuku[index].genre, sizeof(daftarBuku[index].genre), stdin);
    daftarBuku[index].genre[strcspn(daftarBuku[index].genre, "\n")] = 0;

    printf("Masukkan Tahun Terbit Baru: ");
    if (scanf("%d", &daftarBuku[index].tahun_terbit) != 1) {
        printf("\nInput tahun tidak valid.\n");
        while (getchar() != '\n');
        return;
    }
    while (getchar() != '\n');

    printf("Masukkan Status Baru (0 = Dipinjam, 1 = Tersedia): ");
    if (scanf("%d", &daftarBuku[index].status) != 1) {
        printf("\nInput status tidak valid.\n");
        while (getchar() != '\n');
        return;
    }
    while (getchar() != '\n');

    printf("\nBuku berhasil diedit.\n");

}

// Function untuk mengembalikan buku dan menghitung denda
void kembalikan_buku() {
    if (jumlahBuku == 0) {
        printf("Belum ada data buku.\n");
        return;
    }

    char cariJudul[100];
    printf("Masukkan judul buku yang ingin dikembalikan: ");
    fgets(cariJudul, sizeof(cariJudul), stdin);
    cariJudul[strcspn(cariJudul, "\n")] = '\0';

    int index = -1;

    // Mencari buku berdasarkan judulnya
    for (int i = 0; i < jumlahBuku; i++) {
        if (strcmp(daftarBuku[i].judul, cariJudul) == 0) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("Buku dengan judul %s tidak ditemukan\n", cariJudul);
        return;
    }

    if (daftarBuku[index].status == 1) {
        printf("Buku sudah tersedia (sudah dikembalikan).\n");
        return;
    }

    char tanggal_kembali[11];
    printf("Masukkan tanggal pengembalian (DD/MM/YYYY): ");
    fgets(tanggal_kembali, sizeof(tanggal_kembali), stdin);
    tanggal_kembali[strcspn(tanggal_kembali, "\n")] = '\0';

    // Asumsi batas pengembalian 7 hari setelah pinjam
    int selisih = hitung_selisih_hari(daftarBuku[index].tanggal_pinjam, tanggal_kembali);
    int hari_telat = selisih - 7; 
    if (hari_telat > 0) {
        int denda = hari_telat * 5000;
        printf("Buku telat dikembalikan %d hari. Denda: Rp %d\n", hari_telat, denda);
    } else {
        printf("Buku dikembalikan tepat waktu. Tidak ada denda.\n");
    }

    daftarBuku[index].status = 1;  
    strcpy(daftarBuku[index].tanggal_pinjam, "00/00/0000");  
    printf("Buku berhasil dikembalikan.\n");
}

// Function untuk menambah data peminjam baru
void tambah_peminjam() {
    if (jumlahPeminjam >= MAX_PEMINJAM) {
        printf("\nKapasitas maksimum peminjam telah tercapai.\n");
        return;
    }

    printf("\n--- TAMBAH DATA PEMINJAM BARU ---\n");

    printf("Nama Peminjam: ");
    fgets(daftarPeminjam[jumlahPeminjam].nama, sizeof(daftarPeminjam[jumlahPeminjam].nama), stdin);
    daftarPeminjam[jumlahPeminjam].nama[strcspn(daftarPeminjam[jumlahPeminjam].nama, "\n")] = 0;

    printf("NIM Peminjam: ");
    fgets(daftarPeminjam[jumlahPeminjam].nim, sizeof(daftarPeminjam[jumlahPeminjam].nim), stdin);
    daftarPeminjam[jumlahPeminjam].nim[strcspn(daftarPeminjam[jumlahPeminjam].nim, "\n")] = 0;

    printf("Judul Buku yang Dipinjam: ");
    fgets(daftarPeminjam[jumlahPeminjam].judul_buku, sizeof(daftarPeminjam[jumlahPeminjam].judul_buku), stdin);
    daftarPeminjam[jumlahPeminjam].judul_buku[strcspn(daftarPeminjam[jumlahPeminjam].judul_buku, "\n")] = 0;

    // cek apakah buku ada dan tersedia
    int idxBuku = -1;
    for (int i = 0; i < jumlahBuku; i++) {
        if (strcmp(daftarBuku[i].judul, daftarPeminjam[jumlahPeminjam].judul_buku) == 0) {
            idxBuku = i;
            break;
        }
    }

    if (idxBuku == -1) {
        printf("Buku dengan judul \"%s\" tidak ditemukan. Batalkan peminjaman.\n", daftarPeminjam[jumlahPeminjam].judul_buku);
        return;
    }
    if (daftarBuku[idxBuku].status == 0) {
        printf("Buku \"%s\" saat ini sedang dipinjam. Batalkan peminjaman.\n", daftarPeminjam[jumlahPeminjam].judul_buku);
        return;
    }

    printf("Tanggal Peminjaman: ");
    fgets(daftarPeminjam[jumlahPeminjam].tanggal_peminjaman, sizeof(daftarPeminjam[jumlahPeminjam].tanggal_peminjaman), stdin);
    daftarPeminjam[jumlahPeminjam].tanggal_peminjaman[strcspn(daftarPeminjam[jumlahPeminjam].tanggal_peminjaman, "\n")] = 0;

    strcpy(daftarPeminjam[jumlahPeminjam].tanggal_pengembalian, "-"); // default tanggal pengembalian

    daftarPeminjam[jumlahPeminjam].denda = 0; // default denda

    // set status buku jadi dipinjam (0) 
    daftarBuku[idxBuku].status = 0;

    jumlahPeminjam++;

    printf("\nData peminjam berhasil ditambahkan.\n");

    // simpan otomatis
}

// Function untuk menampilkan data peminjam
void tampil_peminjam() {
    if (jumlahPeminjam == 0) {
        printf("\nBelum ada peminjam buku.\n");
        return;
    }

    printf("\n--- DAFTAR PEMINJAM BUKU ---\n");
    printf("-----------------------------------------------------------------------------------------------------------------------------\n");
    printf("| %-20s | %-15s | %-20s | %-20s | %-20s | %-7s |\n",
           "Nama", "NIM", "Judul Buku", "Tanggal Pinjam", "Tanggal Kembali", "Denda");
    printf("-----------------------------------------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < jumlahPeminjam; i++) {
        printf("| %-20s | %-15s | %-20s | %-20s | %-20s | %-7d |\n",
               daftarPeminjam[i].nama,
               daftarPeminjam[i].nim,
               daftarPeminjam[i].judul_buku,
               daftarPeminjam[i].tanggal_peminjaman,
               daftarPeminjam[i].tanggal_pengembalian,
               daftarPeminjam[i].denda);
    }
    printf("-----------------------------------------------------------------------------------------------------------------------------\n");
}

// Function untuk mengurutkan data buku dan peminjam
void sorting_data() {
    Buku tempBuku[MAX_BUKU];
    Peminjam tempPeminjam[MAX_PEMINJAM];

    int i, j, pilihan;

    for (i = 0; i < jumlahBuku; i++) {
        tempBuku[i] = daftarBuku[i];
    }

    for (i = 0; i < jumlahPeminjam; i++) {
        tempPeminjam[i] = daftarPeminjam[i];
    }

    printf("\n1. Sorting data buku ASC.\n");
    printf("2. Sorting data buku DESC.\n");
    printf("3. Sorting data peminjam ASC.\n");
    printf("4. Sorting data peminjam DESC.\n");

    printf("Pilihan: ");
    scanf("%d", &pilihan);

    // Sorting data berdasarkan judul buku
    if (pilihan == 1 || pilihan == 2) {
        for (i = 0; i < jumlahBuku - 1; i++) {
            for (j = 0; j < jumlahBuku - i - 1; j++) {

                int cmp = strcmp(tempBuku[j].judul, tempBuku[j + 1].judul);

                if ((pilihan == 1 && cmp > 0) || (pilihan == 2 && cmp < 0)) {

                    Buku swap = tempBuku[j];
                    tempBuku[j] = tempBuku[j + 1];
                    tempBuku[j + 1] = swap;
                }
            }
        }

        // Tampilkan hasil sorting berdasarkan judul buku
        printf("\n--- HASIL SORTING DATA BUKU ---\n");
        printf("-------------------------------------------------------------------------------------------------\n");
        printf("| %-40s | %-20s | %-12s | %-10s |\n",
           "Judul", "Penulis", "Tahun Terbit", "Status");
        printf("-------------------------------------------------------------------------------------------------\n");

        for (i = 0; i < jumlahBuku; i++) {
            printf("| %-40s | %-20s | %-12d | %-10s |\n",
                   tempBuku[i].judul,
                   tempBuku[i].penulis,
                   tempBuku[i].tahun_terbit,
                   tempBuku[i].status == 1 ? "Tersedia" : "Dipinjam");
        }

        printf("-------------------------------------------------------------------------------------------------\n");
    } else if (pilihan == 3 || pilihan == 4) { // Sorting data berdasarkan nama peminjam
        for (i = 0; i < jumlahPeminjam - 1; i++) {
            for (j = 0; j < jumlahPeminjam - i - 1; j++) {

                int cmp = strcmp(tempPeminjam[j].nama, tempPeminjam[j + 1].nama);

                if ((pilihan == 3 && cmp > 0) || (pilihan == 4 && cmp < 0)) {

                    Peminjam swap = tempPeminjam[j];
                    tempPeminjam[j] = tempPeminjam[j + 1];
                    tempPeminjam[j + 1] = swap;
                }
            }
        }

        // Tampilkan hasil sorting berdasarkan nama peminjam 
        printf("\n--- HASIL SORTING DATA PEMINJAM ---\n");
        printf("-----------------------------------------------------------------------------------------------------------------------------\n");
        printf("| %-20s | %-15s | %-20s | %-20s | %-20s | %-7s |\n",
           "Nama", "NIM", "Judul Buku", "Tanggal Pinjam", "Tanggal Kembali", "Denda");
        printf("-----------------------------------------------------------------------------------------------------------------------------\n");

        for (i = 0; i < jumlahPeminjam; i++) {
            printf("| %-20s | %-15s | %-20s | %-20s | %-20s | %-7d |\n",
                   tempPeminjam[i].nama,
                   tempPeminjam[i].nim,
                   tempPeminjam[i].judul_buku,
                   tempPeminjam[i].tanggal_peminjaman,
                   tempPeminjam[i].tanggal_pengembalian,
                   tempPeminjam[i].denda);
        }

        printf("-----------------------------------------------------------------------------------------------------------------------------\n");
    } else {
        printf("Pilihan sorting tidak valid.\n");
    }
}

// Function untuk mencari data buku atau peminjam
void searching_data() {
    int pilihan;
    char keyword[100];
    int found = 0;

    printf("\n1. Searching judul buku\n");
    printf("2. Searching nama peminjam\n");

    printf("Pilihan: ");
    scanf("%d", &pilihan);
    while (getchar() != '\n');

    if (pilihan != 1 && pilihan != 2) {
        printf("Pilihan searching tidak valid.\n");
        return;
    }

    printf("Masukkan kata kunci: ");
    fgets(keyword, sizeof(keyword), stdin);
    keyword[strcspn(keyword, "\n")] = 0;

    // Searching data berdasarjan judul buku
    if (pilihan == 1) {
        printf("\nHasil pencarian buku:\n");
        for (int i = 0; i < jumlahBuku; i++) {
            if (strstr(daftarBuku[i].judul, keyword) != NULL) {
                printf("%s (Penulis: %s, Tahun: %d, Status: %s)\n",
                       daftarBuku[i].judul,
                       daftarBuku[i].penulis,
                       daftarBuku[i].tahun_terbit,
                       daftarBuku[i].status == 1 ? "Tersedia" : "Dipinjam");

                found = 1;
            }
        }
    } else if (pilihan == 2) { // Searching data berdasarkan nama peminjam
        printf("\nHasil pencarian peminjam:\n");
        for (int i = 0; i < jumlahPeminjam; i++) {
            if (strstr(daftarPeminjam[i].nama, keyword) != NULL) {
                printf("%s | NIM: %s | Pinjam Buku: %s | Tanggal Peminjaman: %s | Tanggal Pengembalian: %s | Denda: %d \n",
                       daftarPeminjam[i].nama,
                       daftarPeminjam[i].nim,
                       daftarPeminjam[i].judul_buku,
                       daftarPeminjam[i].tanggal_peminjaman,
                       daftarPeminjam[i].tanggal_pengembalian,
                       daftarPeminjam[i].denda);

                found = 1;
            }
        }
    } 

    if (!found) {
        printf("Data tidak ditemukan.\n");
    }
}
