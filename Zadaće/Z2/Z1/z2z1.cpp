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
        for(Cvor* pom = l.begin; pom!= nullptr; pom = pom->sljedeci){
            if(pom==l.trenutnii) trenutnii=pom2;
            pom2 = pom2->sljedeci;
        } 
    }
    ~DvostrukaLista(){
        if(broj_elemenata>0){
            while (begin) {
                trenutnii=begin;
                begin=begin->sljedeci;
                delete trenutnii;
            }
        }
        begin = nullptr;
        end = nullptr;
        trenutnii = nullptr;
    }
    DvostrukaLista& operator =(const DvostrukaLista<Tip>& l){
        if(&l == this) return *this;
        while(begin){
            trenutnii=begin;
            begin=begin->sljedeci;
            delete trenutnii;
        }
        begin = nullptr;
        end = nullptr;
        trenutnii = nullptr;
        broj_elemenata = 0;
        for(Cvor* pom = l.begin; pom!=nullptr; pom=pom->sljedeci){
            dodajIza(pom->element);
            sljedeci();
        }
        Cvor* pom2 = begin;
        for(Cvor* pom = l.begin; pom!=nullptr; pom=pom->sljedeci){
            if(pom==l.trenutnii) trenutnii = pom2;
            pom2 = pom2 -> sljedeci;
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
    Tip &operator[](int indeks) override{
        if(indeks >= broj_elemenata || indeks < 0) throw std::domain_error("Neispravan indeks!");
        Cvor* pom = begin;
        for(int i = 0; i < indeks; i++) pom = pom -> sljedeci;
        return pom -> element;
    }
    Tip operator[](int indeks) const override{
        if(indeks >= broj_elemenata || indeks < 0) throw std::domain_error("Neispravan indeks!");
        Cvor* pom = begin;
        for(int i = 0; i < indeks; i++) pom = pom -> sljedeci;
        return pom -> element;
    }
};
template <typename Tip>
class DvostraniRed{
    DvostrukaLista<Tip> d;
public:
    DvostraniRed(): d() {}
    DvostraniRed(const DvostraniRed<Tip> &dr): d(dr.d) {}
    DvostraniRed &operator =(const DvostraniRed<Tip> &dr){
        if(this == &dr) return *this;
        d = dr. d;
        return *this;
    }
    ~DvostraniRed() {}
    void staviNaVrh(const Tip &el){
        if(d.brojElemenata() != 0) d.kraj();
        d.dodajIza(el);
    }
    Tip skiniSaVrha(){
        d.kraj();
        Tip vrhic = d.trenutni();
        d.obrisi();
        return vrhic;
    }
    void staviNaCelo(const Tip &el){
        if(d.brojElemenata() != 0) d.pocetak();
        d.dodajIspred(el);
    }
    Tip skiniSaCela(){
        d.pocetak();
        Tip celoo = d.trenutni();
        d.obrisi();
        return celoo;
    }
    Tip &vrh(){
        d.kraj();
        Tip& vrhic = d.trenutni();
        return vrhic;
    }
    Tip &celo(){
        d.pocetak();
        Tip& celoo = d.trenutni();
        return celoo;
    }
    int brojElemenata() const{
        return d.brojElemenata();
    }
    void brisi(){
        while(d.brojElemenata() != 0) d.obrisi();
    }
};

void TestStaviNaVrh(){
    DvostraniRed<int> red;
    int vrh = 23;
    red.staviNaVrh(11);
    red.staviNaVrh(vrh);
    cout<<"Metoda staviNaVrh: ";
    if(red.vrh() == vrh) cout<<"OK!"<<endl;
    else cout<<"NOK!"<<endl;
}

void TestSkiniSaVrha(){
    DvostraniRed<int> red;
    red.staviNaVrh(23);
    red.staviNaVrh(11);
    red.staviNaVrh(2001);
    red.skiniSaVrha();
    cout<<"Metoda skiniSaVrha: ";
    if(red.vrh() == 11) cout<<"OK!"<<endl;
    else cout<<"NOK!"<<endl;
}

void TestStaviNaCelo(){
    DvostraniRed<int> red;
    int celo = 11;
    red.staviNaCelo(23);
    red.staviNaCelo(celo);
    cout<<"Metoda staviNaCelo: ";
    if(red.celo() == 11) cout<<"OK!"<<endl;
    else cout<<"NOK!"<<endl;
}

void TestSkiniSaCela(){
    DvostraniRed<int> red;
    red.staviNaCelo(23);
    red.staviNaCelo(11);
    red.staviNaCelo(2001);
    red.skiniSaCela();
    cout<<"Metoda skiniSaCela: ";
    if(red.celo()==11) cout<<"OK!"<<endl;
    else cout<<"NOK!"<<endl;
}

void TestVrh(){
    DvostraniRed<int> red;
    int vrh = 5;
    red.staviNaVrh(1);
    red.staviNaVrh(2);
    red.staviNaVrh(3);
    red.staviNaVrh(vrh);
    cout<<"Metoda vrh: ";
    if(vrh == red.vrh()) cout<<"OK!" <<endl;
    else cout<<"NOK!"<<endl;
}

void TestCelo(){
    DvostraniRed<int> red;
    int celo = 5;
    red.staviNaCelo(1);
    red.staviNaCelo(2);
    red.staviNaCelo(3);
    red.staviNaCelo(celo);
    cout<<"Metoda celo: ";
    if(celo == red.celo()) cout<<"OK!"<<endl;
    else cout<<"NOK!"<<endl;
}

void TestBrisi(){
    DvostraniRed<int> red;
    red.staviNaCelo(2);
    red.staviNaVrh(3);
    red.staviNaVrh(5);
    red.staviNaCelo(11);
    red.staviNaCelo(23);
    red.brisi();
    cout<<"Metoda brisi: ";
    if(red.brojElemenata() == 0) cout<<"OK!"<<endl;
    else cout<<"NOK!"<<endl;
}

int main(){
    TestStaviNaVrh();
    TestSkiniSaVrha();
    TestStaviNaCelo();
    TestSkiniSaCela();
    TestVrh();
    TestCelo();
    TestBrisi();
    return 0;
}