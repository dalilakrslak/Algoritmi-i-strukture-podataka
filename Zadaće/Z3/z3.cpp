#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>
using namespace std;

void radixSort(vector<int> &a){
    int max = a.at(0);
    for(int x: a) if(x > max) max = x;
    int brojac(0);
    do{
        brojac++;
    }
    while(max/=10); 
    vector<vector<int>> cifre(10);
    for(int i = 0; i < brojac; i++){
        for(int j = 0; j < cifre.size(); j++) cifre.at(j).resize(0);
        for(int j = 0; j < a.size(); j++) cifre.at(a.at(j)%(10*(int)pow(10,i))/(int)pow(10,i)).push_back(a.at(j));
        a.clear();
        for(int j = 0; j < cifre.size(); j++) 
            for(int k = 0; k < cifre.at(j).size(); k++) a.push_back(cifre.at(j).at(k));
    }
}

void popraviDolje(std::vector<int> &a, int i, int vel){
    while(!(i>=vel/2 && i<vel)){
        int veci = 2*i+1;
        int dd = 2*i+2;
        if(dd<vel && a.at(dd) > a.at(veci)) veci = dd;
        if(a.at(i) > a.at(veci)) return;
        int temp = a.at(i);
        a.at(i) = a.at(veci);
        a.at(veci) = temp;
        i = veci;
    }
}

void stvoriGomilu(vector<int> &a){
    for(int i = std::floor(a.size()/2.); i>=0; i--) popraviDolje(a, i, a.size());
}

int roditelj(int pozicija){
    return std::ceil(pozicija-1)/2;
}

void popraviGore(vector<int> &a, int i){
    while(i!=0 && a.at(i)>a.at(roditelj(i))){
        int temp = a.at(i);
        a.at(i) = a.at(roditelj(i));
        a.at(roditelj(i)) = temp;
        i = roditelj(i);
    }
}

void umetniUGomilu(vector<int> &a, int umetnuti, int &velicina){
    if(velicina>a.size()) throw "Gomila je puna!";
    a.push_back(umetnuti);
    popraviGore(a, velicina);
    velicina++;
}

int izbaciPrvi(vector<int> &a, int &velicina){
    if(velicina == 0) throw "Gomila je prazna!";
    velicina--;
    int temp = a.at(0);
    a.at(0) = a.at(velicina);
    a.at(velicina) = temp;
    if(velicina != 0) popraviDolje(a, 0, velicina);
    return a.at(velicina);
}

void gomilaSort(vector<int> &a){
    stvoriGomilu(a);
    for(int i = a.size() - 1; i>0; i--){
        int temp = a.at(0);
        a.at(0) = a.at(i);
        a.at(i) = temp;
        popraviDolje(a, 0, i);
    }
}

int main(){
    return 0;
}