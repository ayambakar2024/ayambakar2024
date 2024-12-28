#include <iostream>
#include <string>
#include <ctime>
#include <vector>

using namespace std;

struct Item {
    string nama;
    string kategori;
    int harga;
    int jumlah;
};

struct Keranjang {
    Item items[100];
    int totalItems = 0;
};

struct Transaksi {
    string namaPembeli;
    string waktuTransaksi;
    vector<Item> daftarBarang;
    int totalBelanja;
    int uangBayar;
    int kembalian;
};

Item daftarBarang[100] = {
    {"Ayam Geprek", "Makanan", 10000, 0},
    {"Nasi Telor", "Makanan", 10000, 0},
    {"Ayam Bakar", "Makanan", 10000, 0},
    {"Mie Ayam", "Makanan", 10000, 0},
    {"Bakso", "Makanan", 15000, 0},
    {"Nutrisari", "Minuman", 4000, 0},
    {"Air Mineral", "Minuman", 5000, 0},
    {"Es Cekek", "Minuman", 1500, 0}
};

int maxBarang = 8;
vector<Transaksi> historiTransaksi;

string getWaktuSaatIni() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    char waktu[100];
    strftime(waktu, sizeof(waktu), "%A, %d %B %Y %H:%M:%S", ltm);
    return string(waktu);
}

void tampilkanDaftarBarang() {
    cout << "Daftar Barang Tersedia:\n";
    cout << "------------------------------------------\n";
    cout << "No  Nama Barang       Kategori       Harga\n";
    cout << "------------------------------------------\n";
    for (int i = 0; i < maxBarang; i++) {
        cout << (i + 1) << "   " 
             << daftarBarang[i].nama 
             << "     " << daftarBarang[i].kategori 
             << "     " << daftarBarang[i].harga << "\n";
    }
    cout << "------------------------------------------\n";
}

void tambahKeKeranjangMassal(Keranjang &keranjang) {
    tampilkanDaftarBarang();
    int nomorBarang, jumlah;

    do {
        cout << "Masukkan nomor barang (0 untuk selesai): ";
        cin >> nomorBarang;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Input tidak valid, masukkan angka.\n";
            continue;
        }

        if (nomorBarang == 0) break;

        if (nomorBarang >=1&& nomorBarang <= maxBarang) {
            cout << "Masukkan jumlah untuk " << daftarBarang[nomorBarang - 1].nama << ": ";
            cin >> jumlah;

            if (cin.fail() || jumlah < 1) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "Input jumlah tidak valid, masukkan angka yang valid.\n";
                continue;
            }

            bool found = false;
            for (int i = 0; i < keranjang.totalItems; i++) {
                if (keranjang.items[i].nama == daftarBarang[nomorBarang - 1].nama) {
                    keranjang.items[i].jumlah += jumlah;
                    found = true;
                    break;
                }
            }

            if (!found) {
                keranjang.items[keranjang.totalItems] = daftarBarang[nomorBarang - 1];
                keranjang.items[keranjang.totalItems].jumlah = jumlah;
                keranjang.totalItems++;
            }

            cout << jumlah << " " << daftarBarang[nomorBarang - 1].nama << " berhasil ditambahkan ke keranjang.\n";
        } else {
            cout << "Nomor barang tidak valid.\n";
        }
    } while (nomorBarang != 0);
}

void tampilkanKeranjang(Keranjang &keranjang) {
    if (keranjang.totalItems == 0) {
        cout << "Keranjang belanja kosong.\n";
        return;
    }

    cout << "Isi Keranjang Belanja:\n";
    cout << "-------------------------------------------------\n";
    cout << "No  Nama Barang       Jumlah     Harga Total\n";
    cout << "-------------------------------------------------\n";

    int total = 0;
    for (int i = 0; i < keranjang.totalItems; i++) {
        int hargaTotal = keranjang.items[i].jumlah * keranjang.items[i].harga;
        cout << (i + 1) << "   " 
             << keranjang.items[i].nama 
             << "     " << keranjang.items[i].jumlah 
             << "     " << hargaTotal << "\n";
        total += hargaTotal;
    }

    cout << "-------------------------------------------------\n";
    cout << "Total Belanja: " << total << "\n";
}

void cetakStruk(Keranjang &keranjang, const string &namaPembeli, int uangBayar) {
    if (keranjang.totalItems == 0) {
        cout << "Keranjang belanja kosong. Tidak ada struk yang dicetak.\n";
        return;
    }

    int total = 0;
    for (int i = 0; i < keranjang.totalItems; i++) {
        total += keranjang.items[i].jumlah * keranjang.items[i].harga;
    }

    if (uangBayar < total) {
        cout << "Uang yang dibayarkan kurang.\n";
        return;
    }

    int kembalian = uangBayar - total;
    string waktuTransaksi = getWaktuSaatIni();

    cout << "\n=========== STRUK BELANJA ===========\n";
    cout << "Nama Pembeli: " << namaPembeli << "\n";
    cout << "Waktu: " << waktuTransaksi << "\n";
    cout << "-------------------------------------------\n";
    cout << "No  Nama Barang       Jumlah     Harga Total\n";
    cout << "------------------------------------------\n";
    cout << "Total Belanja: " << total << "\n";
    cout << "Uang Bayar: " << uangBayar << "\n";
    cout << "Kembalian: " << kembalian << "\n";
    cout << "===========================================\n";
    cout << "Terima kasih telah berbelanja!\n";


    Transaksi transaksi;
    transaksi.namaPembeli = namaPembeli;
    transaksi.waktuTransaksi = waktuTransaksi;
    transaksi.totalBelanja = total;
    transaksi.uangBayar = uangBayar;
    transaksi.kembalian = kembalian;


    for (int i = 0; i < keranjang.totalItems; i++) {
        transaksi.daftarBarang.push_back(keranjang.items[i]);
    }

    historiTransaksi.push_back(transaksi);


    keranjang.totalItems = 0;
}

void tampilkanHistoriTransaksi() {
    if (historiTransaksi.empty()) {
        cout << "Belum ada histori transaksi.\n";
        return;
    }

    cout << "\n=========== HISTORI TRANSAKSI ===========\n";
    for (size_t i = 0; i < historiTransaksi.size(); i++) {
        const Transaksi &transaksi = historiTransaksi[i];
        cout << "\nTransaksi #" << (i + 1) << ":\n";
        cout << "Nama Pembeli: " << transaksi.namaPembeli << "\n";
        cout << "Waktu: " << transaksi.waktuTransaksi << "\n";
        cout << "-------------------------------------------\n";
        cout << "No  Nama Barang       Jumlah     Harga Total\n";
        cout << "-------------------------------------------\n";

        for (size_t j = 0; j < transaksi.daftarBarang.size(); j++) {
            const Item &barang = transaksi.daftarBarang[j];
            int hargaTotal = barang.jumlah * barang.harga;
            cout << (j + 1) << "   " 
                 << barang.nama 
                 << "     " << barang.jumlah 
                 << "     " << hargaTotal << "\n";
        }

        cout << "-------------------------------------------\n";
        cout << "Total Belanja: " << transaksi.totalBelanja << "\n";
        cout << "Uang Bayar: " << transaksi.uangBayar << "\n";
        cout << "Kembalian: " << transaksi.kembalian << "\n";
        cout << "===========================================\n";
    }
}

void tambahProdukBaru() {
    if (maxBarang >= 100) {
        cout << "Tidak dapat menambah produk baru. Batas barang telah tercapai.\n";
        return;
    }

    string nama, kategori;
    int harga;

    cout << "Masukkan nama produk baru: ";
    cin.ignore();
    getline(cin, nama);

    cout << "Masukkan kategori produk baru: ";
    getline(cin, kategori);

    cout << "Masukkan harga produk baru: ";
    cin >> harga;


    daftarBarang[maxBarang] = {nama, kategori, harga, 0};
    maxBarang++;

    cout << "Produk baru berhasil ditambahkan.\n";
}

void hapusProduk() {
    if (maxBarang <= 0) {
        cout << "Tidak ada produk yang dapat dihapus.\n";
        return;
    }

    tampilkanDaftarBarang();
    int nomorBarang;
    cout << "Masukkan nomor barang yang ingin dihapus: ";
    cin >> nomorBarang;

    if (nomorBarang < 1 || nomorBarang > maxBarang) {
        cout << "Nomor barang tidak valid.\n";
        return;
    }

    for (int i = nomorBarang - 1; i < maxBarang - 1; i++) {
        daftarBarang[i] = daftarBarang[i + 1];
    }

    maxBarang--;
    cout << "Produk berhasil dihapus.\n";
}

int main() {
    Keranjang keranjang;
    string namaPembeli;
    int pilihan;

    cout << "Selamat datang di Toko Online!\n";
    cout << "Masukkan nama Anda: ";
    getline(cin, namaPembeli);

    do {
        cout << "\nMenu:\n";
        cout << "1. Lihat Daftar Barang\n";
        cout << "2. Tambah Barang ke Keranjang\n";
        cout << "3. Lihat Keranjang\n";
        cout << "4. Checkout\n";
        cout << "5. Lihat Histori Transaksi\n";
        cout << "6. Tambah Produk Baru\n";
        cout << "7. Hapus Produk\n";
        cout << "0. Keluar\n";
        cout << "Pilih menu (0-7): ";
        cin >> pilihan;

        switch (pilihan) {
            case 1:
                tampilkanDaftarBarang();
                break;
            case 2:
                tambahKeKeranjangMassal(keranjang);
                break;
            case 3:
                tampilkanKeranjang(keranjang);
                break;
            case 4: {
                int uangBayar;
                tampilkanKeranjang(keranjang);
                cout << "Masukkan jumlah uang yang dibayar: ";
                cin >> uangBayar;
                cetakStruk(keranjang, namaPembeli, uangBayar);
                break;
            }
            case 5:
                tampilkanHistoriTransaksi();
                break;
            case 6:
                tambahProdukBaru();
                break;
            case 7:
                hapusProduk();
                break;
            case 0:
                cout << "Terima kasih! Sampai jumpa.\n";
                break;
            default:
                cout << "Pilihan tidak valid. Silakan coba lagi.\n";
        }
    } while (pilihan != 0);

    return 0;
}

