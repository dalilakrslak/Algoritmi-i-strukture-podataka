#include <iostream>
#include <stdexcept>
using namespace std;

template <typename Tip>
class Lista {
public:
    Lista(){}
    virtual ~Lista() {}
    virtual int brojElemenata() const = 0;
    virtual Tip trenutni() const = 0;
    virtual Tip& trenutni() = 0;
    virtual bool sljedeci() = 0;
    virtual bool prethodni() = 0;
    virtual void pocetak() = 0;
    virtual void kraj() = 0;
    virtual void obrisi() = 0;
    virtual void dodajIspred(const Tip& el) = 0;
    virtual void dodajIza(const Tip& el) = 0;
    virtual Tip &operator [](int indeks)= 0;
    virtual Tip operator[](int indeks) const = 0;
};

template <typename Tip>
class NizLista : public Lista<Tip> {
    Tip **niz;
    int kapacitet, broj_elemenata, indeks_trenutnog;
    void DaLiJePrazan() const{
        if(broj_elemenata==0) throw std::domain_error("Niz je prazan!");
    }
    void Dodaj(const Tip& el, int indeks);
public:
    NizLista(const NizLista<Tip>& l): indeks_trenutnog(l.indeks_trenutnog),kapacitet(l.kapacitet),broj_elemenata(l.broj_elemenata){
        niz=new Tip * [broj_elemenata];
        for(int i=0; i<broj_elemenata; i++) niz[i] = new Tip(*l.niz[i]);
    }
    NizLista(): Lista<Tip>(){
        niz = nullptr;
        indeks_trenutnog = 0;
        kapacitet = 0;
        broj_elemenata = 0;
    }
    ~NizLista() {
        for(int i = 0; i < broj_elemenata; i++) delete niz[i];
        delete[] niz;
    }
    NizLista& operator =(const NizLista<Tip>& l){
        for(int i = 0; i < broj_elemenata; i++){
            delete niz[i];
            niz[i] = 0;
        }
        delete[] niz;
        niz = nullptr;
        niz = new Tip * [l.broj_elemenata];
        for(int i=0; i<l.broj_elemenata; i++) niz[i] = new Tip(*l.niz[i]);
        indeks_trenutnog=l.indeks_trenutnog;
        kapacitet=l.kapacitet;
        broj_elemenata=l.broj_elemenata;
        return *this;
    }
    int brojElemenata() const override{
        return broj_elemenata;
    }
    Tip& trenutni() override{
        DaLiJePrazan();
        return *niz[indeks_trenutnog];
    }
    Tip trenutni() const override{
        if(broj_elemenata==0) throw std::domain_error("Niz je prazan!");
        return *niz[indeks_trenutnog];
    }
    bool prethodni() override{
        DaLiJePrazan();
        if(indeks_trenutnog == 0) return false;
        indeks_trenutnog--;
        return true;
    }
    bool sljedeci() override{
        DaLiJePrazan();
        if(indeks_trenutnog == broj_elemenata - 1) return false;
        indeks_trenutnog++;
        return true;
    }
    void pocetak() override{
        DaLiJePrazan();
        indeks_trenutnog = 0;
    }
    void kraj() override{
        DaLiJePrazan();
        indeks_trenutnog = broj_elemenata - 1;
    }
    void obrisi() override{
        DaLiJePrazan();
        delete niz[indeks_trenutnog];
        niz[indeks_trenutnog] = 0;
        for(int i = indeks_trenutnog; i < broj_elemenata - 1; i++) niz[i] = niz[i + 1];
        if(indeks_trenutnog == broj_elemenata - 1) indeks_trenutnog--;
        broj_elemenata--;
    }
    void dodajIspred(const Tip& el) override{
        Dodaj(el, indeks_trenutnog);
        if(broj_elemenata - 1 != 0) indeks_trenutnog++;
    }
    void dodajIza(const Tip& el) override{
        Dodaj(el, indeks_trenutnog + 1);
    }
    Tip &operator [](int indeks) override{
        if(indeks<0 || indeks>=broj_elemenata) throw std::domain_error("Neispravan indeks!");
        return *niz[indeks];
    }
    Tip operator[](int indeks) const override{
        if(indeks<0 || indeks>=broj_elemenata) throw std::domain_error("Neispravan indeks!");
        return *niz[indeks];
    }
};
template <typename Tip>
void NizLista<Tip>::Dodaj(const Tip& el, int indeks){
    if(broj_elemenata == 0){
        if(kapacitet == 0){
            niz = new Tip * [1];
            kapacitet = 1;
        }
        niz[0] = new Tip(el);
        indeks_trenutnog = 0;
    }
    else if(kapacitet == broj_elemenata){
        kapacitet*=2;
        Tip** pom = new Tip * [kapacitet ]{};
        for(int i=0; i<broj_elemenata+1; i++){
            if(i<indeks) pom[i] = niz[i];
            else if(i==indeks) pom[i] = new Tip(el);
            else pom[i] = niz[i-1];
        }
        delete [] niz;
        niz = pom;
        pom=nullptr;
    }
    else{
        for(int i = broj_elemenata; i > indeks; i--) niz[i] = niz[i-1];
        niz[indeks] = new Tip(el);
    }
    broj_elemenata++;
}

template <typename Tip>
class JednostrukaLista: public Lista<Tip>{
    struct Cvor{
        Tip element;
        Cvor* sljedeci;
        Cvor(const Tip& el, Cvor* slj): element(el), sljedeci(slj){}
    };
    Cvor* begin, *trenutnii, *end;
    int broj_elemenata=0;
    void DaLiJePrazan(){
        if(broj_elemenata==0) throw std::logic_error("Lista je prazna!");
    }
    void kopiraj(const JednostrukaLista<Tip>& l){
        begin=nullptr;
        trenutnii=nullptr;
        end=nullptr;
        broj_elemenata=0;
        Cvor* pom = l.begin;
        while(pom){
            dodajIza(pom->element);
            sljedeci();
            pom=pom->sljedeci;
        }
        pom=l.begin;
        Cvor* pom2 = begin;
        while(pom){
            if(pom==l.trenutnii) trenutnii=pom2;
            pom=pom->sljedeci;
            pom2=pom2->sljedeci;
        }
    }
public:
    JednostrukaLista(): Lista<Tip>(){
        begin=nullptr;
        trenutnii=nullptr;
        end=nullptr;
    }
    JednostrukaLista(const JednostrukaLista<Tip>& l){
        kopiraj(l);
    }
    ~JednostrukaLista(){
        if(broj_elemenata>0){
            while(begin){
                trenutnii=begin;
                begin=begin->sljedeci;
                delete trenutnii;
            }
        }
        begin = nullptr;
        end = nullptr;
        trenutnii = nullptr;
    }
    JednostrukaLista& operator =(const JednostrukaLista<Tip>& l){
        while(begin){
            trenutnii=begin;
            begin=begin->sljedeci;
            delete trenutnii;
        }
        kopiraj(l);
        return *this;
    }
    int brojElemenata() const override{
        return broj_elemenata;
    }
    Tip& trenutni() override{
        DaLiJePrazan();
        return trenutnii->element;
    }
    Tip trenutni() const override{
        if(broj_elemenata<=0) throw std::logic_error("Lista je prazna!");
        return trenutnii->element;
    }
    bool prethodni() override{
        DaLiJePrazan();
        if(trenutnii==begin) return false;
        Cvor *pom = begin;
        while(pom){
            if(pom->sljedeci == trenutnii){
                trenutnii=pom;
                break;
            }
            pom = pom->sljedeci;
        }
        return true;
    }
    bool sljedeci() override{
        DaLiJePrazan();
        if(trenutnii==end) return false;
        trenutnii=trenutnii->sljedeci;
        return true;
    }
    void pocetak() override{
        DaLiJePrazan();
        trenutnii=begin;
    }
    void kraj() override{
        DaLiJePrazan();
        trenutnii=end;
    }
    void obrisi() override{
        DaLiJePrazan();
        Cvor* pom;
        if(broj_elemenata==1){
            delete trenutnii;
            begin=nullptr;
            trenutnii=nullptr;
            end=nullptr;
        }
        else if(trenutnii==end){
            pom=begin;
            while(pom->sljedeci != end) pom = pom->sljedeci;
            trenutnii=pom;
            delete pom->sljedeci;
            trenutnii->sljedeci = nullptr;
            end = trenutnii;
        }
        else if(trenutnii==begin){
            pom=trenutnii->sljedeci;
            delete trenutnii;
            trenutnii=pom;
            begin=pom;
        }
        else{
            pom=trenutnii->sljedeci;
            trenutnii->sljedeci=nullptr;
            prethodni();
            delete trenutnii->sljedeci;
            trenutnii->sljedeci=pom;
            sljedeci();
        }
        broj_elemenata--;
    }
    void dodajIspred(const Tip& el) override{
        if(broj_elemenata==0){
            Cvor *novi = new Cvor(el, nullptr);
            begin = novi;
            trenutnii=novi;
            end=novi;
        }
        else if(broj_elemenata==1){
            Cvor* novi = new Cvor(el, nullptr);
            novi->sljedeci = trenutnii;
            begin = novi;
        }
        else{
            Cvor* pom = begin;
            Cvor* novi = new Cvor(el, nullptr);
            if(begin == trenutnii){
                begin = novi;
                begin->sljedeci = trenutnii;
            }
            else{
                while(pom->sljedeci!=trenutnii) pom = pom->sljedeci;
                Cvor* pom2 = pom->sljedeci;
                pom->sljedeci = novi;
                novi->sljedeci = pom2;
            }
        }
        broj_elemenata++;
    }
    void dodajIza(const Tip& el) override{
        if(broj_elemenata==0){
            Cvor *novi = new Cvor(el, nullptr);
            begin = novi;
            trenutnii = novi;
            end = novi;
        }
        else{
            Cvor* novi = new Cvor(el, nullptr);
            novi->sljedeci = trenutnii->sljedeci;
            trenutnii->sljedeci=novi;
            if(trenutnii==end){
                end=novi;
                novi=nullptr;
            }
        }
        broj_elemenata++;
    }
    Tip &operator [](int indeks) override{
        if(indeks>=broj_elemenata || indeks<0) throw std::domain_error("Neispravan indeks!");
        Cvor* pom=begin;
        for(int i=0; i<indeks; i++) pom=pom->sljedeci;
        return pom->element;
    }
    Tip operator[](int indeks) const override{
        if(indeks>=broj_elemenata || indeks<0) throw std::domain_error("Neispravan indeks!");
        Cvor *pom=begin;
        for(int i=0; i<indeks; i++) pom=pom->sljedeci;
        return pom->element;
    }
};

//TESTOVI ZA NIZ LISTU
void TestKonstruktorBezParametaraNiz(){
    NizLista<int>l;
    std::cout<<"Konstruktor bez parametara: ";
    if(l.brojElemenata()==0) std::cout << "OK" << std::endl;
    else std::cout << "NOK" << std::endl;
}
void TestKopirajuciKonstruktorNiz(){
    NizLista<int>l1;
    for(int i=1; i<=23; i++) l1.dodajIza(i);
    NizLista<int>l2(l1);
    std::cout<<"Kopirajuci konstruktor: ";
    if(l1.brojElemenata()==l2.brojElemenata()) std::cout<< "OK" << std::endl;
    else std::cout << "NOK" <<std::endl;
}
void TestKopirajucegOperatoraNiz(){
    NizLista<int> l1;
    for(int i=1; i<5; i++) l1.dodajIspred(i);
    NizLista<int> l2;
    l2 = l1;
    std::cout<<"Kopirajuci operator: ";
    if(l1.brojElemenata() == l2.brojElemenata()) std::cout << "OK" << std::endl;
    else std::cout << "NOK" << std::endl;
}
void TestMetodeTrenutniNiz(){
    NizLista<int> l;
    l.dodajIspred(1); l.dodajIspred(4); l.dodajIspred(23); l.dodajIspred(11); l.dodajIspred(14);
    std::cout<<"Metoda trenutni: ";
    if(l.trenutni() == 1) std::cout << "OK" << std::endl;
    else std::cout <<"NOK" << std::endl;
}
void TestMetodePrethodniNiz(){
    NizLista<int> l;
    l.dodajIspred(1); l.dodajIspred(4); l.dodajIspred(23); l.dodajIspred(11); l.dodajIspred(14);
    l.prethodni();
    std::cout<<"Metoda prethodni: ";
    if(l.trenutni() == 14) std::cout<<"OK"<<std::endl;
    else std::cout<<"NOK"<<std::endl;
}
void TestMetodeSljedeciNiz(){
    NizLista<int> l;
    l.dodajIza(1); l.dodajIza(4); l.dodajIza(23); l.dodajIza(11); l.dodajIza(14);
    l.sljedeci();
    std::cout<<"Metoda sljedeci: ";
    if(l.trenutni() == 14) std::cout <<"OK" <<std::endl;
    else std::cout<<"NOK"<<std::endl;
}
void TestMetodePocetakNiz(){
    NizLista<int> l;
    l.dodajIspred(1); l.dodajIza(4); l.dodajIza(23);
    l.pocetak();
    std::cout<<"Metoda pocetak: ";
    if(l.trenutni() == 1) std::cout<<"OK"<<std::endl;
    else std::cout<<"NOK"<<std::endl;
}
void TestMetodeKrajNiz(){
    NizLista<int> l;
    l.dodajIspred(1); l.dodajIza(4); l.dodajIza(23);
    l.kraj();
    std::cout<<"Metoda kraj:";
    if(l.trenutni() == 4) std::cout <<"OK" <<std::endl;
    else std::cout<<"NOK"<<std::endl;
}
void TestMetodeObrisiNiz(){
    NizLista<int> l;
    for(int i=1; i<=3; i++){
        l.dodajIza(i); l.sljedeci();
    }
    l.obrisi();
    std::cout<<"Metoda obrisi: ";
    if(l.trenutni()!=3) std::cout<<"OK"<<std::endl;
    else std::cout<<"NOK"<<std::endl;
}
void TestMetodeDodajIspredNiz(){
    NizLista<int> l;
    l.dodajIspred(1); l.dodajIspred(2); l.dodajIspred(3);
    std::cout<<"Metoda dodaj ispred: ";
    if(l[0]==2 && l[1]==3 && l[2]==1) std::cout<<"OK"<<std::endl;
    else std::cout<<"NOK"<<std::endl;
}
void TestMetodeDodajIzaNiz(){
    NizLista<int> l;
    l.dodajIza(1); l.dodajIza(2); l.dodajIza(3);
    std::cout<<"Metoda dodaj iza: ";
    if(l[0]==1 && l[1]==3 && l[2]==2) std::cout<<"OK"<<std::endl;
    else std::cout<<"NOK"<<std::endl;
}

//TESTOVI ZA JEDNOSTRUKU LISTU
void TestKonstruktorBezParametara(){
    JednostrukaLista<int> l;
    std::cout<<"Konstruktor bez parametara: ";
    if(l.brojElemenata()==0) std::cout<<"OK"<<std::endl;
    else std::cout<<"NOK"<<std::endl;
}
void TestKopirajuciKonstruktor(){
    JednostrukaLista<int> l1;
    for(int i=1; i<=3; i++) l1.dodajIza(i);
    JednostrukaLista<int> l2(l1);
    std::cout<<"Kopirajuci konstruktor: ";
    if(l1.brojElemenata()==l2.brojElemenata()) std::cout<<"OK"<<std::endl;
    else std::cout<<"NOK"<<std::endl;
}

void TestKopirajucegOperatora(){
    JednostrukaLista<int> l1;
    for(int i=1; i<5; i++) l1.dodajIspred(i);
    JednostrukaLista<int>l2;
    l2=l1;
    std::cout<<"Kopirajuci operator: ";
    if(l1.brojElemenata()==l2.brojElemenata()) std::cout<<"OK"<<std::endl;
    else std::cout<<"NOK"<<std::endl;
}
void TestMetodeTrenutni(){
    JednostrukaLista<int> l;
    l.dodajIspred(1); l.dodajIspred(23); l.dodajIspred(11);
    std::cout<<"Metoda trenutni: ";
    if(l.trenutni()==1) std::cout<<"OK"<<std::endl;
    else std::cout<<"NOK"<<std::endl;
}
void TestMetodePrethodni(){
    JednostrukaLista<int> l;
    l.dodajIspred(1); l.dodajIspred(23); l.dodajIspred(11);
    l.prethodni();
    std::cout<<"Metoda prethodni: ";
    if(l.trenutni()==11) std::cout<<"OK"<<std::endl;
    else std::cout<<"NOK"<<std::endl;
}
void TestMetodeSljedeci(){
    JednostrukaLista<int> l;
    l.dodajIza(1); l.dodajIza(23); l.dodajIza(11);
    l.sljedeci();
    std::cout<<"Metoda sljedeci: ";
    if(l.trenutni()==11) std::cout<<"OK"<<std::endl;
    else std::cout<<"NOK"<<std::endl;
}
void TestMetodePocetak(){
    JednostrukaLista<int> l;
    l.dodajIspred(1); l.dodajIza(4); l.dodajIza(23);
    l.pocetak();
    std::cout<<"Metoda pocetak: ";
    if(l.trenutni()==1) std::cout<<"OK"<<std::endl;
    else std::cout<<"NOK"<<std::endl;
}
void TestMetodeKraj(){
    JednostrukaLista<int> l;
    l.dodajIspred(1); l.dodajIza(4); l.dodajIza(23);
    l.kraj();
    std::cout<<"Metoda kraj: ";
    if(l.trenutni()==4) std::cout<<"OK"<<std::endl;
    else std::cout<<"NOK"<<std::endl;
}
void TestMetodeObrisi(){
    JednostrukaLista<int> l;
    for(int i=1; i<=3; i++){
        l.dodajIza(i); l.sljedeci();
    }
    l.obrisi();
    std::cout<<"Metoda obrisi: ";
    if(l.trenutni()!=3) std::cout<<"OK"<<std::endl;
    else std::cout<<"NOK"<<std::endl;
}
void TestMetodeDodajIspred(){
    JednostrukaLista<int> l;
    l.dodajIspred(1); l.dodajIspred(2); l.dodajIspred(3);
    std::cout<<"Metoda dodaj ispred: ";
    if(l[0]==2 && l[1]==3 && l[2]==1) std::cout<<"OK"<<std::endl;
    else std::cout<<"NOK"<<std::endl;
}
void TestMetodeDodajIza(){
    JednostrukaLista<int> l;
    l.dodajIza(1); l.dodajIza(2); l.dodajIza(3);
    std::cout<<"Metoda dodaj iza: ";
    if(l[0]==1 && l[1]==3 && l[2]==2) std::cout<<"OK"<<std::endl;
    else std::cout<<"NOK"<<std::endl;
}
int main(){
    TestKonstruktorBezParametaraNiz();
    TestKopirajucegOperatoraNiz();
    TestKopirajuciKonstruktorNiz();
    TestMetodeTrenutniNiz();
    TestMetodePrethodniNiz();
    TestMetodeSljedeciNiz();
    TestMetodePocetakNiz();
    TestMetodeKrajNiz();
    TestMetodeObrisiNiz();
    TestMetodeDodajIspredNiz();
    TestMetodeDodajIzaNiz();

    std::cout<<std::endl;

    TestKonstruktorBezParametara();
    TestKopirajuciKonstruktor();
    TestKopirajucegOperatora();
    TestMetodeTrenutni();
    TestMetodePrethodni();
    TestMetodeSljedeci();
    TestMetodePocetak();
    TestMetodeKraj();
    TestMetodeObrisi();
    TestMetodeDodajIspred();
    TestMetodeDodajIza();
    return 0;
}