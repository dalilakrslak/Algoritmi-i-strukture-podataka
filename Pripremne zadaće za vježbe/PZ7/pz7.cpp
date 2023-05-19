#include <bits/types/clock_t.h>
#include <ctime>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <ios>
#include <string>
#include <chrono>

using namespace std;

template <typename Tip>
void bubble_sort(Tip* niz, int vel){
    for(int i = vel - 1; i >= 1; i--){
        for(int j = 1; j <= i; j++){
            if(niz[j-1] > niz[j]){
                Tip pom = niz[j-1];
                niz[j-1] = niz[j];
                niz[j] = pom;
            }
        }
    }
}

template <typename Tip>
void selection_sort(Tip* niz, int vel){
    for(int i = 0; i <= vel - 2; i++){
        Tip min = niz[i];
        int pmin = i;
        for(int j = i+1; j <= vel - 1; j++){
            if(niz[j] < min){
                min = niz[j];
                pmin = j;
            }
        }
        niz[pmin] = niz[i];
        niz[i] = min;
    }
}

template <typename Tip>
int particija(Tip *A, int prvi, int zadnji){
    Tip pivot = A[prvi];
    int p = prvi + 1;
    while(p <= zadnji && A[p] < pivot) {
        p = p + 1;
    }
    for(int i = p + 1; i <= zadnji; i++){
        if(A[i] < pivot){
            Tip pom = A[i];
            A[i] = A[p];
            A[p] = pom;
            p = p + 1;
        }
    }
    Tip pom = A[prvi];
    A[prvi] = A[p-1];
    A[p-1] = pom;
    return p-1;
}

template <typename Tip>
void quick_sort(Tip* niz, int vel){
    int prvi = 0;
    int zadnji = vel - 1;
    int j = 0;
    if(prvi < zadnji){
        j = particija(niz, prvi, zadnji);
        quick_sort(niz, j);
        quick_sort(niz + j + 1, zadnji - j);
    }
}

template <typename Tip>
void merge_sort(Tip* niz, int vel){
    if(vel > 1){
        int p = vel/2;
        merge_sort(niz, p);
        merge_sort(niz + p, vel - p);
        int i(0), j(p), k(0);
        Tip *temp = new Tip[vel]{};
        for(int d = 0; d < vel; d++) temp[d] = niz[d];
        while(i < p && j < vel){
            if(temp[i] < temp[j]){
                niz[k] = temp[i];
                i++;
            }
            else{
                niz[k] = temp[j];
                j++;
            }
            k++;
        }
        while(i < p){
            niz[k] = temp[i];
            i++;
            k++;
        }
        while(j < vel){
            niz[k] = temp[j];
            j++;
            k++;
        }
        delete[] temp;
    }
}

void ucitaj(std::string filename, int* &niz, int &vel){
    std::ifstream ulaz(filename);
    if(!ulaz) throw logic_error("Datoteka ne postoji!");
    int brojac(0), n(0);
    while(ulaz >> n) brojac++;
    vel = brojac;
    niz = new int[vel];
    ulaz.clear();
    ulaz.seekg(0);
    brojac = 0;
    while(ulaz >> n){
        niz[brojac] = n;
        brojac++;
    }
}

void generisi(std::string filename, int vel){
    std::ofstream izlaz;
    izlaz.open(filename, std::ios::out | std::ios::trunc);
    if(!izlaz) throw std::logic_error("Greska prilikom otvaranja datoteke!");
    for(int i = 0; i < vel; i++) izlaz << rand() << " ";
}

void funkcija(){
    int n = 0;
    std::cout<<"Unesite broj elemenata niza: ";
    std::cin>>n;
    std::string filename;
    std::cout<<"Unesite naziv datoteke: ";
    std::cin>>filename;
    filename += ".txt";
    generisi(filename,  n);
    int *niz = nullptr;
    int vel(0);
    ucitaj(filename, niz, vel);
    std::cout<<"Izaberite algoritam koji zelite koristiti za sortiranje!"<<std::endl;
    std::cout<<"  1 - BUBBLE SORT "<<std::endl;
    std::cout<<"  2 - SELECTION SORT"<<std::endl;
    std::cout<<"  3 - QUICK SORT"<<std::endl;
    std::cout<<"  4 - MERGE SORT"<<std::endl;
    int izbor;
    std::cin>>izbor;
    clock_t pocetak = clock();
    //auto pocetak = std::chrono::high_resolution_clock::now();
    switch (izbor) {
        case 1: 
            bubble_sort(niz, vel);
            break;
        case 2:
            selection_sort(niz, vel);
            break;
        case 3:
            quick_sort(niz, vel);
            break;
        case 4:
            merge_sort(niz, vel);
            break;
        default:
            throw std::domain_error("Pogresan izbor!");
            break;
    }
    clock_t kraj = clock();
    //auto kraj = std::chrono::high_resolution_clock::now();
    //auto trajanje = std::chrono::duration_cast<std::chrono::microseconds> (kraj-pocetak);
    std::cout<<"Algoritam ";
    if(izbor == 1) cout<<"BUBBLE SORT";
    else if(izbor == 2) cout<<"SELECTION SORT";
    else if(izbor == 3) cout<<"QUICK SORT";
    else if(izbor == 4) cout<<"MERGE SORT";
    std::cout<<" traje "<<(kraj-pocetak)/(CLOCKS_PER_SEC/1000)<<" ms."<<std::endl;

    for(int i = 1; i < vel; i++){
        if(niz[i] < niz[i-1]) throw std::domain_error("Ucitani niz nije sortiran!");
    }
    cout<<"Ucitani niz je sortiran!"<<endl;
    cout<<"Unesite naziv datoteke u koju zelite zapisati sortirani niz: ";
    std::string izlaznaDatoteka;
    std::cin>>izlaznaDatoteka;
    izlaznaDatoteka+=".txt";
    std::ofstream izlaz;
    izlaz.open(izlaznaDatoteka, std::ios::out | std::ios::trunc);
    if(!izlaz) throw std::logic_error("Greska!");
    for(int i=0; i<vel; i++) izlaz<<niz[i]<<" ";
    delete[] niz;
}

int main(){
    funkcija();
    return 0;
}