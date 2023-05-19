#include <iostream>
#include <stdexcept>
using namespace std;

template <typename Tip>
class Lista{
public:
    Lista() {}
    Lista(const Lista<Tip>& l);
    Lista& operator =(const Lista<Tip>& l);
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
    virtual Tip &operator [](int indeks) = 0;
    virtual Tip operator[](int indeks) const = 0;
};

template <typename Tip>
class DvostrukaLista: public Lista<Tip>{
    struct Cvor{
        Tip element;
        Cvor *sljedeci, *prethodni;
        Cvor(const Tip& el, Cvor* slj, Cvor* pr): element(el), sljedeci(slj), prethodni(pr){}
    };
    Cvor *begin, *trenutnii, *end;
    int broj_elemenata;
    void DaLiJePrazan(){
        if(broj_elemenata==0) throw std::domain_error("Lista je prazna!");
    }
public:
    DvostrukaLista(){
        begin=nullptr;
        trenutnii=nullptr;
        end=nullptr;
        broj_elemenata = 0;
    }
    DvostrukaLista(const DvostrukaLista<Tip>& l){
        begin = nullptr;
        end = nullptr;
        trenutnii = nullptr;
        broj_elemenata = 0;
        for(Cvor* pom = l.begin; pom!=nullptr; pom = pom ->sljedeci){
            dodajIza(pom->element);
            sljedeci();
        }
        Cvor* pom2 = begin;
        for(Cvor* pom = l.begin; pom!=nullptr; pom = pom->sljedeci){
            if(pom == l.trenutnii) trenutnii = pom2;
            pom2 = pom2->sljedeci;
        }
    }
    ~DvostrukaLista(){
        if(broj_elemenata>0){
            while(begin){
                trenutnii=begin;
                begin=begin->sljedeci;
                delete trenutnii;
            }
        }
        begin=nullptr;
        end=nullptr;
        trenutnii=nullptr;
    }
    DvostrukaLista& operator =(const DvostrukaLista<Tip>& l){
        if(&l == this) return *this;
        while(begin){
            trenutnii = begin;
            begin = begin -> sljedeci;
            delete trenutnii;
        }
        begin = nullptr;
        end = nullptr;
        trenutnii = nullptr;
        broj_elemenata = 0;
        for(Cvor* pom=l.begin; pom!=nullptr; pom=pom->sljedeci){
            dodajIza(pom->element);
            sljedeci();
        }
        Cvor* pom2 = begin;
        for(Cvor* pom = l.begin; pom!=nullptr; pom=pom->sljedeci){
            if(pom == l.trenutnii) trenutnii = pom2;
            pom2 = pom2->sljedeci;
        }
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
        if(broj_elemenata==0) throw std::domain_error("Lista je prazna!");
        return trenutnii->element; 
    }
    bool prethodni() override{
        DaLiJePrazan();
        if(trenutnii==begin) return false;
        trenutnii=trenutnii->prethodni;
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
            pom = trenutnii -> prethodni;   
            trenutnii = pom;
            delete pom -> sljedeci;
            trenutnii -> sljedeci = nullptr;
            end = trenutnii;
        }
        else if(trenutnii == begin){
            pom = trenutnii -> sljedeci;
            delete trenutnii;
            trenutnii = pom;
            begin = pom;
        }
        else{
            pom = trenutnii -> sljedeci;
            trenutnii -> sljedeci = nullptr;
            prethodni();
            pom -> prethodni = trenutnii;
            delete trenutnii -> sljedeci;
            trenutnii -> sljedeci = pom;
            sljedeci();
        }
        broj_elemenata--;
    }
    void dodajIspred(const Tip& el) override{
        Cvor *novi = new Cvor(el, nullptr, nullptr);
        if(broj_elemenata == 0){
            begin = novi; 
            end = novi;
            trenutnii = novi;
        }
        else if(broj_elemenata == 1){
            novi -> sljedeci = trenutnii;
            begin = novi;
            trenutnii -> prethodni = novi;
        }
        else{
            if(begin == trenutnii){
                begin = novi;
                begin -> sljedeci = trenutnii;
                trenutnii -> prethodni = novi;
            }
            else{
                novi -> sljedeci = trenutnii;
                novi -> prethodni = trenutnii -> prethodni;
                trenutnii->prethodni->sljedeci = novi;
                trenutnii -> prethodni = novi;
            }
        }
        broj_elemenata++;
    }
    void dodajIza(const Tip& el) override{
        Cvor *novi = new Cvor(el, nullptr, nullptr);
        if(broj_elemenata == 0){
            begin = novi;
            end = novi;
            trenutnii = novi;
        }
        else{
            novi -> sljedeci = trenutnii -> sljedeci;
            if(trenutnii -> sljedeci){
                trenutnii->sljedeci->prethodni = novi;
            }
            trenutnii -> sljedeci = novi;
            novi->prethodni = trenutnii;
            if(trenutnii == end){
                end = novi;
                novi = nullptr;
            }
        }
        broj_elemenata++;
    }
    Tip &operator [](int indeks) override{
        if(indeks >= broj_elemenata || indeks < 0) throw std::domain_error("Neispravan indeks!");
        Cvor* pom = begin;
        for(int i = 0; i < indeks; i++) pom = pom -> sljedeci;
        return pom -> element;
    }
    Tip operator[](int indeks) const override{
        if(indeks >= broj_elemenata || indeks < 0) throw std::domain_error("Neispravan indeks!");
        Cvor* pom = begin;
        for(int i = 0 ; i < indeks; i++) pom = pom -> sljedeci;
        return pom -> element;
    }
};

template <typename Tip>
Tip dajMaksimum(const Lista<Tip>& n){
    Tip maksimum;
    Lista<Tip>* pom = nullptr;
    if(const DvostrukaLista<Tip>* lista = dynamic_cast<const DvostrukaLista<Tip>*>(&n)){
        pom = new DvostrukaLista<Tip>(*lista);
    }
    pom -> pocetak();
    maksimum = pom -> trenutni();
    while(pom -> sljedeci()){
        if(pom-> trenutni() > maksimum) maksimum = pom -> trenutni();
    }
    delete pom;
    return maksimum;
}

void testKonstruktorBezParametara(){
    Lista<int> *l = new DvostrukaLista<int>();
    std::cout<<"Konstruktor bez parametara: ";
    if(l->brojElemenata() == 0) cout<<"OK!"<<endl;
    else cout<<"NOK!"<<endl;
}
void testKopirajuciKonstruktor(){
    DvostrukaLista<int> l;
    for(int i=11; i<=23; i++) l.dodajIspred(i);
    DvostrukaLista<int> l1(l);
    int br_el = l1.brojElemenata();
    bool da_li_je=true;
    if(l.brojElemenata()==l1.brojElemenata() && l.trenutni()==l1.trenutni()){
        for(int i=0; i<br_el; i++){
            if(l[i]!=l1[i]){
                da_li_je=false;
                break;
            }
        }
    }
    cout<<"Kopirajuci konstruktor: ";
    if(da_li_je) cout<<"OK!"<<endl;
    else cout<<"NOK!"<<endl;
}

void testOperatoraDodjele(){
    DvostrukaLista<int> l;
    for(int i = 11; i <= 23 ; i++) l.dodajIspred(i);
    DvostrukaLista<int> l1 = l;
    cout<<"Operator dodjele: ";
    if(l1.brojElemenata()== l.brojElemenata() && l1.trenutni() == l.trenutni()) cout<<"OK!"<<endl;
    else cout<<"NOK!"<<endl;
}

void testTrenutni(){
    DvostrukaLista<int> l;
    for(int i = 11; i<= 23; i++) l.dodajIza(i);
    cout<<"Metoda trenutni: ";
    if(l.trenutni() == 11) cout<<"OK!"<<endl;
    else cout<<"NOK!"<<endl;
}

void testPrethodni(){
    DvostrukaLista<int> l;
    for(int i=11; i<=23; i++) l.dodajIspred(i);
    l.prethodni();
    cout<<"Metoda prethodni: ";
    if(l.trenutni() == 23) cout<<"OK!"<<endl;
    else cout<<"NOK!"<<endl;
}

void testSljedeci(){
    DvostrukaLista<int> l;
    for(int i=11; i<=23; i++) l.dodajIza(i);
    l.sljedeci();
    cout<<"Metoda sljedeci: ";
    if(l.trenutni() == 23) cout<<"OK!"<<endl;
    else cout<<"NOK!"<<endl;
}

void testPocetak(){
    DvostrukaLista<int> l;
    for(int i=11; i<=23; i++) l.dodajIspred(i);
    l.pocetak();
    cout<<"Metoda pocetak: ";
    if(l.trenutni() == 12 ) cout<<"OK!"<<endl;
    else cout<<"NOK!"<<endl;
}

void testKraj(){
    DvostrukaLista<int> l;
    for(int i=11; i<=23; i++) l.dodajIza(i);
    l.kraj();
    cout<<"Metoda kraj: ";
    if(l.trenutni() == 12) cout<<"OK!"<<endl;
    else cout<<"NOK!"<<endl;
}

void testObrisi(){
    DvostrukaLista<int> l;
    for(int i=0; i<3; i++) l.dodajIspred(i);
    for(int i=0; i<3; i++) l.obrisi();
    cout<<"Metoda obrisi: ";
    if(l.brojElemenata()==0) cout<<"OK!"<<endl;
    else cout<<"NOK!"<<endl;
}

void testDodajIza(){
    DvostrukaLista<int> l;
    for(int i=11 ; i<=23; i++) l.dodajIza(i);
    cout<<"Metoda dodaj iza: ";
    l.pocetak();
    if(l.trenutni()==11) cout<<"OK!"<<endl;
    else cout<<"NOK!"<<endl;
}

void testDodajIspred(){
    DvostrukaLista<int> l;
    for(int i=11; i<=23; i++) l.dodajIspred(i);
    cout<<"Metoda dodaj ispred: ";
    l.kraj();
    if(l.trenutni()==11) cout<<"OK!"<<endl;
    else cout<<"NOK!"<<endl;
}

void testDajMaksimum(){
    DvostrukaLista<int> l;
    l.dodajIspred(11);
    l.dodajIspred(23);
    l.dodajIza(28);
    l.dodajIza(8);
    l.dodajIspred(11);
    l.dodajIza(236);
    cout<<"Funkcija DajMaksimum: ";
    if(dajMaksimum(l) == 236) cout<<"OK!"<<endl;
    else cout<<"NOK!"<<endl;
}

int main(){
    testKonstruktorBezParametara();
    testKopirajuciKonstruktor();
    testOperatoraDodjele();
    testTrenutni();
    testPrethodni();
    testSljedeci();
    testPocetak();
    testKraj();
    testObrisi();
    testDodajIspred();
    testDodajIza();
    testDajMaksimum();
    return 0;
}