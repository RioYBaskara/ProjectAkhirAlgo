#include <iostream>

using namespace std;

struct NodeLangkah {
    int nomorLangkah;
    string deskripsiLangkah;

    NodeLangkah* next;
    NodeLangkah* prev;
};

struct NodeResep {
    // informasi resep
    string namaResep;
    int estimasiWaktu;
    
    // untuk ngehubungin ke DLL Langkah
    NodeLangkah* headLangkah;
    NodeLangkah* tailLangkah;

    NodeResep* next;
};

NodeResep* headKatalog = NULL;
NodeResep* tailKatalog = NULL;

int main(){
    cout << "coba";

    return 0;
}