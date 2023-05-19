//Iz PZ6 cu copy/paste klase Mapa i NizMapa
#include <bits/types/clock_t.h>
#include <cstdlib>
#include <ctime>
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

template <typename TipKljuca, typename TipVrijednosti>
class BinStabloMapa: public Mapa<TipKljuca, TipVrijednosti> {
    int velicina;
    struct Cvor{
        TipKljuca kljuc;
        TipVrijednosti vrijednost;
        Cvor *lijevo, *desno, *roditelj;
        Cvor(){
            kljuc();
            vrijednost();
            lijevo = nullptr;
            desno = nullptr;
            roditelj = nullptr;
        }
        Cvor(TipKljuca k, TipVrijednosti vr, Cvor *left, Cvor *right, Cvor *parent){
            kljuc = k;
            vrijednost = vr;
            lijevo = left;
            desno = right;
            roditelj = parent;
        }
    };
    Cvor *korijen;

    void ocisti(Cvor *cvoric){
        if(cvoric!=nullptr){
            ocisti(cvoric->lijevo);
            ocisti(cvoric->desno);
            delete cvoric;
            velicina--;
        }
    }

    void kopiraj(Cvor *&to, Cvor *from, Cvor *parent){
        if(from != nullptr) {
            to = new Cvor(from->kljuc, from->vrijednost, nullptr, nullptr, parent);
            kopiraj(to->lijevo, from->lijevo, to);
            kopiraj(to->desno, from->desno, to);
            velicina++;
        }
    }

    TipVrijednosti &vrati(Cvor *&korijenn, Cvor *rroditelj, TipKljuca k){
        if(korijenn == nullptr){
            korijenn = new Cvor(k, TipVrijednosti(), nullptr, nullptr, rroditelj);
            velicina++;
        }
        if(k == korijenn->kljuc) return korijenn->vrijednost;
        if(k < korijenn->kljuc) return vrati(korijenn->lijevo, korijenn, k);
        else return vrati(korijenn->desno, korijenn, k);
    }
    Cvor *trazi(Cvor *korijenn, TipKljuca kljuc) const{
        if(korijenn == nullptr) return nullptr;
        if(kljuc == korijenn->kljuc) return korijenn;
        if(kljuc < korijenn->kljuc) return trazi(korijenn->lijevo, kljuc);
        else return trazi(korijenn->desno, kljuc);
    }
    void brisi(Cvor *c){
        if(c != nullptr){
            if(c->lijevo == nullptr && c->desno==nullptr){
                if(c->roditelj==nullptr) korijen = nullptr;
                else{
                    if(c==c->roditelj->lijevo) c->roditelj->lijevo=nullptr;
                    else c->roditelj->desno=nullptr;
                }
                delete c;
                velicina--;
            }
            else if(c->lijevo!=nullptr && c->desno!=nullptr){
                Cvor *temp = c->lijevo;
                while (temp->desno != nullptr) temp=temp->desno;
                c->kljuc = temp->kljuc;
                c->vrijednost = temp->vrijednost;
                brisi(temp);
            }
            else{
                Cvor *dijete = c->lijevo;
                if(dijete==nullptr) dijete=c->desno;
                if(c->roditelj!=nullptr){
                    if(c==c->roditelj->lijevo) c->roditelj->lijevo = dijete;
                    else c->roditelj->desno = dijete;
                }
                else korijen=dijete;
                dijete->roditelj = c->roditelj;
                delete c;
                velicina--;
            }
        }
    }

public:
    BinStabloMapa(){
        velicina = 0;
        korijen = nullptr;
    }
    BinStabloMapa(const BinStabloMapa<TipKljuca, TipVrijednosti> &stablo): korijen(nullptr), velicina(0){
        kopiraj(korijen, stablo.korijen, nullptr);
    }
    ~BinStabloMapa(){
        ocisti(korijen);
    }
    BinStabloMapa &operator=(const BinStabloMapa<TipKljuca, TipVrijednosti> &stablo){
        if(&stablo!=this){
            ocisti(korijen);
            kopiraj(korijen, stablo.korijen, nullptr);
        }
        return *this;
    }
    int brojElemenata() const override{
        return velicina;
    }
    void obrisi() override{
        ocisti(korijen);
        korijen = nullptr;
        velicina = 0;
    }
    void obrisi(const TipKljuca &kljuc) override{
        Cvor *cvoric = trazi(korijen, kljuc);
        if(cvoric == nullptr) return;
        brisi(cvoric);
    }
    TipVrijednosti &operator[](TipKljuca kljuc) override{
        return vrati(korijen, nullptr, kljuc);
    }
    TipVrijednosti operator[](TipKljuca kljuc) const override{
        Cvor *cvoric = trazi(korijen, kljuc);
        if(cvoric != nullptr) return cvoric->vrijednost;
        return TipVrijednosti();
    }

};

int main(){
    NizMapa<int, int> nizMapa;
    BinStabloMapa<int, int> binStabloMapa;

    clock_t pocetak_niz1 = clock();
    for(int i = 0; i < 10000; i++) nizMapa[rand()] = rand();
    clock_t kraj_niz1 = clock();

    clock_t pocetak_mapa1 = clock();
    for(int i = 0; i < 10000; i++) binStabloMapa[rand()] = rand();
    clock_t kraj_mapa1 = clock();

    std::cout<<"Dodavanje 10000 elemenata u niz traje: "<<double((kraj_niz1-pocetak_niz1))/(CLOCKS_PER_SEC/1000)<<" ms, a u stablo "<<double((kraj_mapa1-pocetak_mapa1))/(CLOCKS_PER_SEC/1000)<<" ms."<<std::endl;
    
    clock_t pocetak_niz2 = clock();
    nizMapa[10001] = rand();
    clock_t kraj_niz2 = clock();

    clock_t pocetak_mapa2 = clock();
    binStabloMapa[10001] = rand();
    clock_t kraj_mapa2 = clock();
 
    std::cout<<"Dodavanje jednog elementa u niz traje: "<<double((kraj_niz2-pocetak_niz2))/(CLOCKS_PER_SEC/1000)<<" ms, a u stablo "<<double((kraj_mapa2-pocetak_mapa2))/(CLOCKS_PER_SEC/1000)<<" ms."<<std::endl;

    clock_t pocetak_niz3 = clock();
    nizMapa[10001];
    clock_t kraj_niz3 = clock();

    clock_t pocetak_mapa3 = clock();
    binStabloMapa[10001];
    clock_t kraj_mapa3 = clock();
 
    std::cout<<"Pristup postojecem elementu u niz traje: "<<double((kraj_niz3-pocetak_niz3))*1000/(CLOCKS_PER_SEC)<<" ms, a u stablo "<<double((kraj_mapa3-pocetak_mapa3))/(CLOCKS_PER_SEC/1000)<<" ms."<<std::endl;
    
    //Primjecujem da je rad sa klasom NizMapa sporiji u odnosu na rad sa klasom BinStabloMapa.
    //Prilikom dodavanja velike kolicine slucajno generisanih podataka vidimo da stablo to obavi dosta brze.
    //Kada je rijec o dodavanju jednog elementa u niz, u oba slucaja je trajanje kratko, ali vidimo da je opet BinStabloMapa brza.
    //Prilikom pristupa postojecem elementu, opet je u oba slucaja trajanje kratko, ali BinStabloMapa opet radi brze.

    return 0;
}