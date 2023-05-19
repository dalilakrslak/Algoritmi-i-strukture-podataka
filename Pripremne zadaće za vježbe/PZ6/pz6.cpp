#include <iostream>
#include <utility>

using namespace std;

template <typename TipKljuca, typename TipVrijednosti>
class Mapa{
public:
    Mapa(){}
    virtual ~Mapa(){}
    virtual int brojElemenata() const = 0;
    virtual void obrisi() = 0;
    virtual void obrisi(const TipKljuca& kljuc) = 0;
    virtual TipVrijednosti operator[](TipKljuca kljuc) const = 0;
    virtual TipVrijednosti &operator[](TipKljuca kljuc) = 0;
};

template <typename TipKljuca, typename TipVrijednosti>
class NizMapa: public Mapa<TipKljuca, TipVrijednosti>{
    std::pair<TipKljuca, TipVrijednosti> *elementi;
    int kapacitet, duzina;
public:
    NizMapa(): elementi(nullptr), kapacitet(0), duzina(0){}
    NizMapa(const NizMapa<TipKljuca, TipVrijednosti> &mapa): kapacitet(mapa.kapacitet), duzina(mapa.duzina){
        elementi = new std::pair<TipKljuca, TipVrijednosti>[mapa.kapacitet];
        for(int i = 0; i < mapa.duzina; i++) elementi[i] = mapa.elementi[i];
    }
    NizMapa &operator =(const NizMapa<TipKljuca, TipVrijednosti> &mapa){
        if(this == &mapa) return *this;
        delete [] elementi;
        elementi = nullptr;
        kapacitet = mapa.kapacitet;
        duzina = mapa.duzina;
        elementi = new std::pair<TipKljuca, TipVrijednosti>[kapacitet];
        for(int i = 0; i < duzina; i++) elementi[i] = mapa.elementi[i];
        return *this;
    }
    ~NizMapa(){
        delete [] elementi;
    }
    int brojElemenata() const override{
        return duzina;
    }
    void obrisi() override{
        duzina = 0;
    }
    void obrisi(const TipKljuca& kljuc) override{
        int indeks = -1;
        for(int i = 0; i < duzina; i++){
            if(elementi[i].first == kljuc){ 
                indeks = i;
                break;
            }
        }
        if(indeks == -1) throw "Element ne postoji ili je izvan opsega";
        for(int i = indeks; i < duzina - 1; i++){
            elementi[i] = elementi[i+1];
        }
        duzina--;
    }
    TipVrijednosti operator[](TipKljuca kljuc) const override{
        for(int i = 0; i < duzina; i++){
            if(elementi[i].first == kljuc) return elementi[i].second;
        }
        return TipVrijednosti();
    }
    TipVrijednosti &operator[](TipKljuca kljuc) override{
        for(int i = 0; i < duzina; i++){
            if(elementi[i].first == kljuc) return elementi[i].second;
        }
        if(kapacitet == 0){
            elementi = new std::pair<TipKljuca, TipVrijednosti>[1];
            elementi[0] = std::pair<TipKljuca, TipVrijednosti>(kljuc, TipVrijednosti());
            kapacitet = 1;
        }
        else if(kapacitet > duzina) elementi[duzina] = std::pair<TipKljuca, TipVrijednosti>(kljuc, TipVrijednosti());
        else{
            kapacitet *= 2;
            std::pair<TipKljuca, TipVrijednosti> *temp = new std::pair<TipKljuca, TipVrijednosti>[kapacitet]{};
            for(int i = 0; i < duzina; i++) temp[i] = elementi[i];
            delete [] elementi;
            elementi = temp;
            temp = nullptr;
            elementi[duzina] = std::pair<TipKljuca, TipVrijednosti>(kljuc, TipVrijednosti());
        }
        duzina++;
        return elementi[duzina-1].second;
    }
};

void testKonstruktor(){
    NizMapa<int, double> mapa;
    cout<<"Konstruktor bez parametara: ";
    if(mapa.brojElemenata() == 0) cout<<"OK!"<<endl;
    else cout<<"NOK!"<<endl;
}

void testKopirajuciKonstruktor(){
    NizMapa<int, int> mapa;
    mapa[0] = 23;
    NizMapa<int, int> mapica(mapa);
    cout<<"Kopirajuci konstruktor: ";
    if(mapa.brojElemenata() == mapica.brojElemenata()) cout<<"OK!"<<endl;
    else cout<<"NOK!"<<endl;
}

void testOperatorDodjele(){
    NizMapa<int, char> mapa;
    mapa[0] = 'D';
    NizMapa<int, char> mapica;
    mapica = mapa;
    cout<<"Operator dodjele: ";
    if(mapa.brojElemenata() == mapica.brojElemenata()) cout<<"OK!"<<endl;
    else cout<<"NOK!"<<endl;
}

void testBrojElemenata(){
    NizMapa<int, int> mapa;
    mapa[0] = 23;
    cout<<"Broj elemenata: ";
    if(mapa.brojElemenata() == 1) cout<<"OK!"<<endl;
    else cout<<"NOK!"<<endl;
}

void testObrisiBezParametara(){
    NizMapa<int, char> mapa;
    mapa[0] = 'D';
    mapa.obrisi();
    cout<<"Obrisi bez parametara: ";
    if(mapa.brojElemenata() == 0) cout<<"OK!"<<endl;
    else cout<<"NOK!"<<endl;
}

void testObrisiSaParametrom(){
    NizMapa<int, char> mapa;
    mapa[0] = 'D';
    mapa[1] = 'M';
    mapa[2] = 'F';
    mapa.obrisi(2);
    cout<<"Obrisi bez parametara: ";
    if(mapa.brojElemenata() == 2) cout<<"OK!"<<endl;
    else cout<<"NOK!"<<endl;
}

int main(){
    testKonstruktor();
    testKopirajuciKonstruktor();
    testOperatorDodjele();
    testBrojElemenata();
    testObrisiBezParametara();
    testObrisiSaParametrom();
    return 0;
}