// Iz PZ6 kopiramo Mapa, NizMapa, BinStabloMapa
#include <bits/types/clock_t.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>

using namespace std;

template <typename TipKljuca, typename TipVrijednosti> class Mapa {
public:
  Mapa() {}
  virtual ~Mapa() {}
  virtual int brojElemenata() const = 0;
  virtual void obrisi() = 0;
  virtual void obrisi(const TipKljuca &kljuc) = 0;
  virtual TipVrijednosti operator[](TipKljuca kljuc) const = 0;
  virtual TipVrijednosti &operator[](TipKljuca kljuc) = 0;
};

template <typename TipKljuca, typename TipVrijednosti>
class NizMapa : public Mapa<TipKljuca, TipVrijednosti> {
  std::pair<TipKljuca, TipVrijednosti> *elementi;
  int kapacitet, duzina;

public:
  NizMapa() : elementi(nullptr), kapacitet(0), duzina(0) {}
  NizMapa(const NizMapa<TipKljuca, TipVrijednosti> &mapa)
      : kapacitet(mapa.kapacitet), duzina(mapa.duzina) {
    elementi = new std::pair<TipKljuca, TipVrijednosti>[mapa.kapacitet];
    for (int i = 0; i < mapa.duzina; i++)
      elementi[i] = mapa.elementi[i];
  }
  NizMapa &operator=(const NizMapa<TipKljuca, TipVrijednosti> &mapa) {
    if (this == &mapa)
      return *this;
    delete[] elementi;
    elementi = nullptr;
    kapacitet = mapa.kapacitet;
    duzina = mapa.duzina;
    elementi = new std::pair<TipKljuca, TipVrijednosti>[kapacitet];
    for (int i = 0; i < duzina; i++)
      elementi[i] = mapa.elementi[i];
    return *this;
  }
  ~NizMapa() { delete[] elementi; }
  int brojElemenata() const override { return duzina; }
  void obrisi() override { duzina = 0; }
  void obrisi(const TipKljuca &kljuc) override {
    int indeks = -1;
    for (int i = 0; i < duzina; i++) {
      if (elementi[i].first == kljuc) {
        indeks = i;
        break;
      }
    }
    if (indeks == -1)
      throw "Element ne postoji ili je izvan opsega";
    for (int i = indeks; i < duzina - 1; i++) {
      elementi[i] = elementi[i + 1];
    }
    duzina--;
  }
  TipVrijednosti operator[](TipKljuca kljuc) const override {
    for (int i = 0; i < duzina; i++) {
      if (elementi[i].first == kljuc)
        return elementi[i].second;
    }
    return TipVrijednosti();
  }
  TipVrijednosti &operator[](TipKljuca kljuc) override {
    for (int i = 0; i < duzina; i++) {
      if (elementi[i].first == kljuc)
        return elementi[i].second;
    }
    if (kapacitet == 0) {
      elementi = new std::pair<TipKljuca, TipVrijednosti>[1];
      elementi[0] =
          std::pair<TipKljuca, TipVrijednosti>(kljuc, TipVrijednosti());
      kapacitet = 1;
    } else if (kapacitet > duzina)
      elementi[duzina] =
          std::pair<TipKljuca, TipVrijednosti>(kljuc, TipVrijednosti());
    else {
      kapacitet *= 2;
      std::pair<TipKljuca, TipVrijednosti> *temp =
          new std::pair<TipKljuca, TipVrijednosti>[kapacitet] {};
      for (int i = 0; i < duzina; i++)
        temp[i] = elementi[i];
      delete[] elementi;
      elementi = temp;
      temp = nullptr;
      elementi[duzina] =
          std::pair<TipKljuca, TipVrijednosti>(kljuc, TipVrijednosti());
    }
    duzina++;
    return elementi[duzina - 1].second;
  }
};

template <typename TipKljuca, typename TipVrijednosti>
class BinStabloMapa : public Mapa<TipKljuca, TipVrijednosti> {
  int velicina;
  struct Cvor {
    TipKljuca kljuc;
    TipVrijednosti vrijednost;
    Cvor *lijevo, *desno, *roditelj;
    Cvor() {
      kljuc();
      vrijednost();
      lijevo = nullptr;
      desno = nullptr;
      roditelj = nullptr;
    }
    Cvor(TipKljuca k, TipVrijednosti vr, Cvor *left, Cvor *right,
         Cvor *parent) {
      kljuc = k;
      vrijednost = vr;
      lijevo = left;
      desno = right;
      roditelj = parent;
    }
  };
  Cvor *korijen;

  void ocisti(Cvor *cvoric) {
    if (cvoric != nullptr) {
      ocisti(cvoric->lijevo);
      ocisti(cvoric->desno);
      delete cvoric;
      velicina--;
    }
  }

  void kopiraj(Cvor *&to, Cvor *from, Cvor *parent) {
    if (from != nullptr) {
      to = new Cvor(from->kljuc, from->vrijednost, nullptr, nullptr, parent);
      kopiraj(to->lijevo, from->lijevo, to);
      kopiraj(to->desno, from->desno, to);
      velicina++;
    }
  }

  TipVrijednosti &vrati(Cvor *&korijenn, Cvor *rroditelj, TipKljuca k) {
    if (korijenn == nullptr) {
      korijenn = new Cvor(k, TipVrijednosti(), nullptr, nullptr, rroditelj);
      velicina++;
    }
    if (k == korijenn->kljuc)
      return korijenn->vrijednost;
    if (k < korijenn->kljuc)
      return vrati(korijenn->lijevo, korijenn, k);
    else
      return vrati(korijenn->desno, korijenn, k);
  }
  Cvor *trazi(Cvor *korijenn, TipKljuca kljuc) const {
    if (korijenn == nullptr)
      return nullptr;
    if (kljuc == korijenn->kljuc)
      return korijenn;
    if (kljuc < korijenn->kljuc)
      return trazi(korijenn->lijevo, kljuc);
    else
      return trazi(korijenn->desno, kljuc);
  }
  void brisi(Cvor *c) {
    if (c != nullptr) {
      if (c->lijevo == nullptr && c->desno == nullptr) {
        if (c->roditelj == nullptr)
          korijen = nullptr;
        else {
          if (c == c->roditelj->lijevo)
            c->roditelj->lijevo = nullptr;
          else
            c->roditelj->desno = nullptr;
        }
        delete c;
        velicina--;
      } else if (c->lijevo != nullptr && c->desno != nullptr) {
        Cvor *temp = c->lijevo;
        while (temp->desno != nullptr)
          temp = temp->desno;
        c->kljuc = temp->kljuc;
        c->vrijednost = temp->vrijednost;
        brisi(temp);
      } else {
        Cvor *dijete = c->lijevo;
        if (dijete == nullptr)
          dijete = c->desno;
        if (c->roditelj != nullptr) {
          if (c == c->roditelj->lijevo)
            c->roditelj->lijevo = dijete;
          else
            c->roditelj->desno = dijete;
        } else
          korijen = dijete;
        dijete->roditelj = c->roditelj;
        delete c;
        velicina--;
      }
    }
  }

public:
  BinStabloMapa() {
    velicina = 0;
    korijen = nullptr;
  }
  BinStabloMapa(const BinStabloMapa<TipKljuca, TipVrijednosti> &stablo)
      : korijen(nullptr), velicina(0) {
    kopiraj(korijen, stablo.korijen, nullptr);
  }
  ~BinStabloMapa() { ocisti(korijen); }
  BinStabloMapa &
  operator=(const BinStabloMapa<TipKljuca, TipVrijednosti> &stablo) {
    if (&stablo != this) {
      ocisti(korijen);
      kopiraj(korijen, stablo.korijen, nullptr);
    }
    return *this;
  }
  int brojElemenata() const override { return velicina; }
  void obrisi() override {
    ocisti(korijen);
    korijen = nullptr;
    velicina = 0;
  }
  void obrisi(const TipKljuca &kljuc) override {
    Cvor *cvoric = trazi(korijen, kljuc);
    if (cvoric == nullptr)
      return;
    brisi(cvoric);
  }
  TipVrijednosti &operator[](TipKljuca kljuc) override {
    return vrati(korijen, nullptr, kljuc);
  }
  TipVrijednosti operator[](TipKljuca kljuc) const override {
    Cvor *cvoric = trazi(korijen, kljuc);
    if (cvoric != nullptr)
      return cvoric->vrijednost;
    return TipVrijednosti();
  }
};

template <typename TipKljuca, typename TipVrijednosti>
class HashMapa : public Mapa<TipKljuca, TipVrijednosti> {
    int velicina;
    int max;
    std::pair<TipKljuca, TipVrijednosti> *elementi;
    int *oznake; //0 - prazan, 1 - obrisan, 2 - postoji
    unsigned int (*hashFunkcija)(TipKljuca, unsigned int);

    int dajPoziciju(const TipKljuca &kljuc) const{
        int i = hashFunkcija(kljuc, max);
        if(elementi[i].first == kljuc){
            if(oznake[i] == 2) return i;
        }
        else{
            int j = i;
            i = (i+1) % max;
            while(i!=j){
                if(oznake[i] == 0) break;
                if(elementi[i].first == kljuc) return i;
                i = (i+1)%max;
            }
        }
        return -1;
    }

    int slobodnaPozicija(const TipKljuca &kljuc){
        int pozicija = hashFunkcija(kljuc, max);
        while(1){
            if(oznake[pozicija] == 0 || oznake[pozicija] == 1)
                return pozicija;
            pozicija = (pozicija + 1) % max;
        }
        return -1;
    }

public:
    HashMapa(): elementi(nullptr), oznake(nullptr), hashFunkcija(nullptr) {
        max = 10;
        velicina = 0;
        elementi = new std::pair<TipKljuca, TipVrijednosti>[max];
        oznake = new int[max];
        for (int i = 0; i < max; i++)
            oznake[i] = 0;
    }
    HashMapa(const HashMapa<TipKljuca, TipVrijednosti> &mapa) {
        elementi = new std::pair<TipKljuca, TipVrijednosti>[mapa.max];
        oznake = new int[mapa.max];
        for (int i = 0; i < mapa.max; i++) {
            elementi[i] = mapa.elementi[i];
            oznake[i] = mapa.oznake[i];
        }
        max = mapa.max;
        velicina = mapa.velicina;
        hashFunkcija = mapa.hashFunkcija;
    }
    ~HashMapa() {
        delete[] elementi;
        delete[] oznake;
    }
    HashMapa &operator=(const HashMapa<TipKljuca, TipVrijednosti> &mapa) {
        if (&mapa != this) {
            delete[] elementi;
            delete[] oznake;
            elementi = nullptr;
            oznake = nullptr;
            elementi = new std::pair<TipKljuca, TipVrijednosti>[mapa.max];
            oznake = new int[mapa.max];
            for (int i = 0; i < mapa.max; i++) {
                elementi[i] = mapa.elementi[i];
                oznake[i] = mapa.oznake[i];
            }
            velicina = mapa.velicina;
            max = mapa.max;
            hashFunkcija = mapa.hashFunkcija;
        }
        return *this;
    }
    int brojElemenata() const override { 
        return velicina; 
    }
    void definisiHashFunkciju(unsigned int (*fja)(TipKljuca kljuc, unsigned int opseg)) {
        hashFunkcija = fja;
    }
    void obrisi() override {
        for (int i = 0; i < max; i++)
            oznake[i] = 0;
        velicina = 0;
    }
    void obrisi(const TipKljuca &kljuc) override {
        int pozicija = dajPoziciju(kljuc);
        if(pozicija<0)
            throw std::logic_error("Element sa datim kljucem se ne nalazi u kolekciji");
        oznake[pozicija] = 1;
        velicina--;
    }
    TipVrijednosti operator [](TipKljuca kljuc) const override {
        int pozicija = dajPoziciju(kljuc);
        if(pozicija < 0)
            return TipVrijednosti(); 
        return elementi[pozicija].second;
    }
    TipVrijednosti &operator[](TipKljuca kljuc) override {
        if (hashFunkcija == nullptr)
            throw std::domain_error("Hash funkcija nije definisana!");
        int i = dajPoziciju(kljuc);
        if(i >= 0)
            return elementi[i].second;
        unsigned int indeks = slobodnaPozicija(kljuc);
        elementi[indeks] = std::pair<TipKljuca, TipVrijednosti>(kljuc, TipVrijednosti());
        oznake[indeks] = 2;
        int vrati = indeks;
        velicina++;
        if(((double)velicina/max) > 0.2){
            max = max*10;
            std::pair<TipKljuca, TipVrijednosti> *temp = new std::pair<TipKljuca, TipVrijednosti>[max];
            int *tempo = new int[max]{};
            auto pom_elem = elementi;
            elementi = temp;
            temp = pom_elem;
            pom_elem = nullptr;
            auto pom_ozn = oznake;
            oznake = tempo;
            tempo = pom_ozn;
            pom_ozn = nullptr;
            int pozicija;
            for(int j = 0; j<max/10; j++){
                if(tempo[j]==2){
                    pozicija = slobodnaPozicija(temp[j].first);
                    elementi[pozicija] = temp[j];
                    oznake[pozicija] = 2;
                    if(kljuc == temp[j].first) vrati = pozicija;
                }
            }
            delete[] temp;
            temp = nullptr;
            delete[] tempo;
            tempo = nullptr;
        }
        return elementi[vrati].second;
    }
};

unsigned int hashf(int p, unsigned int max){
    return p % max;
}

int main() {
    //kopirat cu main iz pz8
    NizMapa<int, int> nizMapa;
    BinStabloMapa<int, int> binStabloMapa;
    HashMapa<int, int> hashMapa;
    hashMapa.definisiHashFunkciju(hashf);

    clock_t pocetak_niz1 = clock();
    for(int i = 0; i < 10000; i++) nizMapa[rand()] = rand();
    clock_t kraj_niz1 = clock();

    clock_t pocetak_mapa1 = clock();
    for(int i = 0; i < 10000; i++) binStabloMapa[rand()] = rand();
    clock_t kraj_mapa1 = clock();

    clock_t pocetak_hash1 = clock();
    for(int i = 0; i < 10000; i++) hashMapa[rand()] = rand();
    clock_t kraj_hash1 = clock();

    std::cout<<"Dodavanje 10000 elemenata u niz traje: "<<double((kraj_niz1-pocetak_niz1))/(CLOCKS_PER_SEC/1000)<<" ms, u stablo "<<double((kraj_mapa1-pocetak_mapa1))/(CLOCKS_PER_SEC/1000)<<" ms, a u hash "<<double((kraj_hash1-pocetak_hash1))/(CLOCKS_PER_SEC/1000)<<" ms."<<std::endl;
    
    clock_t pocetak_niz2 = clock();
    nizMapa[10001] = rand();
    clock_t kraj_niz2 = clock();

    clock_t pocetak_mapa2 = clock();
    binStabloMapa[10001] = rand();
    clock_t kraj_mapa2 = clock();

    clock_t pocetak_hash2 = clock();
    hashMapa[100001] = rand();
    clock_t kraj_hash2 = clock();
 
    std::cout<<"Dodavanje jednog elementa u niz traje: "<<double((kraj_niz2-pocetak_niz2))/(CLOCKS_PER_SEC/1000)<<" ms, u stablo "<<double((kraj_mapa2-pocetak_mapa2))/(CLOCKS_PER_SEC/1000)<<", a u hash "<<double((kraj_hash2-pocetak_hash2))/(CLOCKS_PER_SEC/1000)<<" ms."<<std::endl;

    clock_t pocetak_niz3 = clock();
    nizMapa[10001];
    clock_t kraj_niz3 = clock();

    clock_t pocetak_mapa3 = clock();
    binStabloMapa[10001];
    clock_t kraj_mapa3 = clock();

    clock_t pocetak_hash3 = clock();
    hashMapa[10001];
    clock_t kraj_hash3 = clock();
 
    std::cout<<"Pristup postojecem elementu u niz traje: "<<double((kraj_niz3-pocetak_niz3))*1000/(CLOCKS_PER_SEC)<<" ms, u stablo "<<double((kraj_mapa3-pocetak_mapa3))/(CLOCKS_PER_SEC/1000)<<", a u hash "<<double((kraj_hash3-pocetak_hash3))/(CLOCKS_PER_SEC/1000)<<" ms."<<std::endl;
    
    /*
        Primjecujem da je rad sa klasom NizMapa sporiji u odnosu na rad sa klasama BinStabloMapa i HashMapa.
        Prilikom dodavanja velike kolicine slucajno generisanih podataka vidimo da hash i stablo to obave dosta brze (poprilicno mala razlika).
        Kada je rijec o dodavanju jednog elementa u niz, u oba slucaja je trajanje kratko, ali vidimo da su opet BinStabloMapa i HashMapa brze.
        Prilikom pristupa postojecem elementu, opet je u oba slucaja trajanje kratko, ali BinStabloMapa i HashMapa opet rade brze.
        Zakljucak je da je u ovom slucaju HashMapa najbrza, ali njena brzina ovisi od hash funkcije.  
    */
 
    return 0;
}