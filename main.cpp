#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <fstream>
#include <limits>

using namespace std;

const char alphaNum[] ="0123456789!@#$%^&*_abcdefghijklmnopqrstuvwxyz";
int panjang_string = sizeof(alphaNum);

struct MyToDo{
    int id;
    char judul[99999];
    string keterangan;
};

//prototype procedur
void password_generator();
int main_menu();
void checkDatabase(fstream &data);
void writeData(fstream &data, int posisi, MyToDo &inputTodo);
int getDataSize(fstream &data);
MyToDo readData(fstream &data, int posisi);
void addTodo(fstream &data);
void displayDataTodo(fstream &data);
void updateRecord(fstream &data);
void deleteTodo(fstream &data);

int main(){
    for(int a =0;a<=100;a++){
        cout << endl << "Loading..." << a << "%";
        system("cls");
    }
    cout << "LOADING COMPLETE" << endl;

    fstream data;

    checkDatabase(data);

    int pilihan = main_menu();
    char is_continue;

    enum option {CREATE = 1, READ, UPDATE, DELETE, MINI_FEATURES, FINISH};

    while(pilihan != FINISH) {
        switch(pilihan){
            case CREATE:
                cout << "Menambah Data Catatan" << endl;
                addTodo(data);
                break;
            case READ:
                cout << "Tampilkan Data Catatan" << endl;
                displayDataTodo(data);
                break;
            case UPDATE:
                cout << "Ubah Data Catatan" << endl;
                displayDataTodo(data);
                updateRecord(data);
                displayDataTodo(data);
                break;
            case DELETE:
                cout << "Hapus Data Catatan" << endl;
                displayDataTodo(data);
                deleteTodo(data);
                displayDataTodo(data);
                break;
            case MINI_FEATURES:
                cout << "fitur Tambahan" << endl;
                password_generator();
                break;
            default:
                cout << "Pilihan tidak ditemukan" << endl;
                break;
        }

        label_lanjut:
        cout << "Lanjutkan?(y/n) : ";
        cin >> is_continue;
        if(is_continue == 'y' || is_continue == 'Y'){
            pilihan = main_menu();
        } else if (is_continue == 'n' || is_continue == 'N'){
            break;
        } else {
            goto label_lanjut;
        }
    }
    cout << "akhir dari program"<< endl;

    cin.get();
    return 0;
}


void writeData(fstream &data, int posisi, MyToDo &inputTodo){
    data.seekp((posisi - 1)*sizeof(MyToDo),ios::beg);
    data.write(reinterpret_cast<char*>(&inputTodo),sizeof(MyToDo));
}

MyToDo readData(fstream &data, int posisi){
    MyToDo readTodo;
    data.seekg((posisi-1)*sizeof(MyToDo),ios::beg);
    data.read(reinterpret_cast<char*>(&readTodo),sizeof(MyToDo));
    return readTodo;
}

void addTodo(fstream &data){

    MyToDo inputTodo, lastTodo;
    int size = getDataSize(data);

    cout << "Ukuran data : " << size << endl;
    if(size == 0){
        inputTodo.id = 1;
    } else {
        lastTodo = readData(data, size);
        cout << "id = " << lastTodo.id << endl;
        inputTodo.id = lastTodo.id +1;
    }

    cout << "Judul : ";
    cin.get(inputTodo.judul,99999);
    cin.ignore();
    cout << "keterangan : ";
    getline(cin, inputTodo.keterangan);

    writeData(data, size+1, inputTodo);
    cout << "catatan berhasil dibuat!!!" << endl;
}

void displayDataTodo(fstream &data){
    int size = getDataSize(data);
    MyToDo showMyTodo;
    cout << "no.\tid.\tjudul.\tketerangan." << endl;
    for(int i = 1; i <=size; i++){
        showMyTodo = readData(data,i);
        cout << i << "\t";
        cout << showMyTodo.id << "\t";
        cout << showMyTodo.judul << "\t";
        cout << showMyTodo.keterangan << endl;
    }
}

void updateRecord(fstream &data){
    int nomor;
    MyToDo updateTodo;
    cout << "Pilih Nomor : ";
    cin >> nomor;
    cin.ignore(numeric_limits<streamsize>::max(),'\n');

    updateTodo = readData(data, nomor);

    cout << "\n\npilihan data : " << endl;
    cout << "judul : " <<updateTodo.judul << endl;
    cout << "keterangan : " <<updateTodo.keterangan << endl;

    cout << "Merubah data : "<< endl;
    cout << "Judul: ";
    cin.get(updateTodo.judul,99999);
    cin.ignore();
    cout << "Keterangan: ";
    getline(cin, updateTodo.keterangan);

    writeData(data, nomor,updateTodo);
}

void deleteTodo(fstream &data){
    int nomor,size,offset;
    MyToDo blankTodo, tempTodo;
    fstream dataSementara;

    size = getDataSize(data);
    // 1. pilih nomor
    cout << "Hapus nomor: ";
    cin >> nomor;
    // 2. isi dengan data kosong
    writeData(data, nomor, blankTodo);
    // 3. cek data dari data.bin
    dataSementara.open("temp.dat", ios::trunc|ios::out|ios::in|ios::binary);

    offset = 0;

    for(int i=1; i<=size;i++){
        tempTodo = readData(data,i);

        if(tempTodo.judul[0]!=0){
            writeData(dataSementara,i - offset,tempTodo);
        }else{
            offset++;
            cout << "deleted item" << endl;
        }
    }
    // 4. pindahkan data dari file sementara ke data bin
    size = getDataSize(dataSementara);
    data.close();
    data.open("data.txt",ios::trunc|ios::out|ios::binary);
    data.close();
    data.open("data.txt",ios::out|ios::in|ios::binary);

    for(int i=1;i<=size;i++){
        tempTodo = readData(dataSementara,i);
        writeData(data,i,tempTodo);
    }
}

void checkDatabase(fstream &data){
    data.open("data.txt", ios::out | ios::in | ios::binary);

    if(data.is_open()){
        cout << "database Exist" << endl;
    } else {
        cout << "database Tidak Exist, buat database baru" << endl;
        data.close();
        data.open("data.txt", ios::trunc | ios::out | ios::in | ios::binary);
    }
}

int main_menu(){
    int input;

    system("cls");
    cout << "PROGRAM CRUD To Do List" << endl;
    cout << "=======================" << endl;
    cout << "[1] Tambah Data Catatan" << endl;
    cout << "[2] Tampilkan Catatan" << endl;
    cout << "[3] Ubah Catatan" << endl;
    cout << "[4] Hapus Catatan" << endl;
    cout << "[5] Mini Features" << endl;
    cout << "[6] Selesai" << endl;
    cout << "=======================" << endl;
    cout << "Pilih [1-6]? :";
    cin >> input;
    cin.ignore(numeric_limits<streamsize>::max(),'\n');
    return input;
}

int getDataSize(fstream &data){
    int awal, akhir;
    data.seekg(0,ios::beg);
    awal = data.tellg();
    data.seekg(0,ios::end);
    akhir = data.tellg();
    return (akhir-awal)/sizeof(MyToDo);
}

void password_generator(){
    system("cls");
    int length = 0;
    cout << "masukan panjang password : ";
    cin >> length;
    srand(time(0));
    cout << "Password yang dihasilkan : ";
    for(int p=0;p<length;p++){
        cout << alphaNum[rand()% panjang_string];
    }
    cout << endl;
}
