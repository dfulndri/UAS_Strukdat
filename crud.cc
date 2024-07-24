#include <iostream>
#include <mysql/mysql.h>
#include <sstream>

using namespace std;

const char* hostname = "127.0.0.1";
const char* user = "root";
const char* pass = "123";
const char* dbname = "cpp_crud_toko";
unsigned int port = 31235;
const char* unixsocket = NULL;
unsigned long clientflag = 0;

MYSQL* koneksi_db() {
    MYSQL* conn = mysql_init(0);
    if (conn) {
        conn = mysql_real_connect(conn, hostname, user, pass, dbname, port, unixsocket, clientflag);
        if (conn) {
            cout << "Berhasil terhubung ke database." << endl;
        } else {
            cerr << "Koneksi gagal: " << mysql_error(conn) << endl;
        }
    } else {
        cerr << "mysql_init gagal" << endl;
    }
    return conn;
}

void tambah_produk(const string& nama, const string& kategori, double harga, int stok) {
    MYSQL* conn = koneksi_db();
    if (conn) {
        stringstream query;
        query << "INSERT INTO produk (nama, kategori, harga, stok) VALUES ('" << nama << "', '" << kategori << "', " << harga << ", " << stok << ")";
        if (mysql_query(conn, query.str().c_str())) {
            cerr << "INSERT gagal: " << mysql_error(conn) << endl;
        } else {
            cout << "Produk berhasil ditambahkan." << endl;
        }
        mysql_close(conn);
    }
}

void lihat_produk() {
    MYSQL* conn = koneksi_db();
    if (conn) {
        if (mysql_query(conn, "SELECT * FROM produk")) {
            cerr << "SELECT gagal: " << mysql_error(conn) << endl;
            mysql_close(conn);
            return;
        }

        MYSQL_RES* res = mysql_store_result(conn);
        if (res == nullptr) {
            cerr << "mysql_store_result gagal: " << mysql_error(conn) << endl;
            mysql_close(conn);
            return;
        }

        MYSQL_ROW row;
        while ((row = mysql_fetch_row(res))) {
            cout << "ID: " << row[0] << ", Nama: " << row[1] << ", Kategori: " << row[2] << ", Harga: " << row[3] << ", Stok: " << row[4] << endl;
        }

        mysql_free_result(res);
        mysql_close(conn);
    }
}

void ubah_produk(int id_produk, const string& nama, const string& kategori, double harga, int stok) {
    MYSQL* conn = koneksi_db();
    if (conn) {
        stringstream query;
        query << "UPDATE produk SET nama = '" << nama << "', kategori = '" << kategori << "', harga = " << harga << ", stok = " << stok << " WHERE id = " << id_produk;
        if (mysql_query(conn, query.str().c_str())) {
            cerr << "UPDATE gagal: " << mysql_error(conn) << endl;
        } else {
            cout << "Produk berhasil diubah." << endl;
        }
        mysql_close(conn);
    }
}

void hapus_produk(int id_produk) {
    MYSQL* conn = koneksi_db();
    if (conn) {
        stringstream query;
        query << "DELETE FROM produk WHERE id = " << id_produk;
        if (mysql_query(conn, query.str().c_str())) {
            cerr << "DELETE gagal: " << mysql_error(conn) << endl;
        } else {
            cout << "Produk berhasil dihapus." << endl;
        }
        mysql_close(conn);
    }
}

int main() {
    int pilihan_role;
    cout << "Pilih Peran:\n";
    cout << "1. Admin\n";
    cout << "2. Pengguna\n";
    cout << "Masukkan pilihan: ";
    cin >> pilihan_role;

    int pilihan;
    while (true) {
        if (pilihan_role == 1) {
            cout << "\nMenu Admin:\n";
            cout << "1. Tambah Produk\n";
            cout << "2. Lihat Semua Produk\n";
            cout << "3. Ubah Produk\n";
            cout << "4. Hapus Produk\n";
            cout << "5. Keluar\n";
            cout << "Masukkan pilihan: ";
            cin >> pilihan;

            if (pilihan == 1) {
                string nama, kategori;
                double harga;
                int stok;
                cout << "Masukkan nama produk: ";
                cin.ignore();
                getline(cin, nama);
                cout << "Masukkan kategori: ";
                getline(cin, kategori);
                cout << "Masukkan harga: ";
                cin >> harga;
                cout << "Masukkan jumlah stok: ";
                cin >> stok;
                tambah_produk(nama, kategori, harga, stok);
            } else if (pilihan == 2) {
                lihat_produk();
            } else if (pilihan == 3) {
                int id_produk;
                string nama, kategori;
                double harga;
                int stok;
                cout << "Masukkan ID produk yang akan diubah: ";
                cin >> id_produk;
                cin.ignore();
                cout << "Masukkan nama produk baru: ";
                getline(cin, nama);
                cout << "Masukkan kategori baru: ";
                getline(cin, kategori);
                cout << "Masukkan harga baru: ";
                cin >> harga;
                cout << "Masukkan jumlah stok baru: ";
                cin >> stok;
                ubah_produk(id_produk, nama, kategori, harga, stok);
            } else if (pilihan == 4) {
                int id_produk;
                cout << "Masukkan ID produk yang akan dihapus: ";
                cin >> id_produk;
                hapus_produk(id_produk);
            } else if (pilihan == 5) {
                break;
            } else {
                cout << "Pilihan tidak valid. Silakan coba lagi." << endl;
            }
        } else if (pilihan_role == 2) {
            cout << "\nMenu Pengguna:\n";
            cout << "1. Lihat Semua Produk\n";
            cout << "2. Keluar\n";
            cout << "Masukkan pilihan: ";
            cin >> pilihan;

            if (pilihan == 1) {
                lihat_produk();
            } else if (pilihan == 2) {
                break;
            } else {
                cout << "Pilihan tidak valid. Silakan coba lagi." << endl;
            }
        } else {
            cout << "Pilihan peran tidak valid. Keluar..." << endl;
            break;
        }
    }
    return 0;
}
