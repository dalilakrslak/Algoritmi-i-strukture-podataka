#include <iostream>
#include <stdexcept>
#include <vector>
#include <queue>
using namespace std;

template <typename TipOznake>
class Grana;

template <typename TipOznake>
class Cvor;

template <typename TipOznake>
class GranaIterator;

template <typename TipOznake>
class UsmjereniGraf{
public:
    UsmjereniGraf() {}
    UsmjereniGraf(int br_c) {}
    virtual ~UsmjereniGraf() {}
    virtual int dajBrojCvorova() const = 0;
    virtual void postaviBrojCvorova(int br_c) = 0;
    virtual void dodajGranu(int polazni, int dolazni, double tezina) = 0;
    virtual void obrisiGranu(int polazni, int dolazni) = 0;
    virtual void postaviTezinuGrane(int polazni, int dolazni, double tezina) = 0;
    virtual double dajTezinuGrane(int polazni, int dolazni) = 0;
    virtual bool postojiGrana(int polazni, int dolazni) = 0;
    virtual void postaviOznakuCvora(int br_c, TipOznake oznaka) = 0;
    virtual TipOznake dajOznakuCvora(int br_c) = 0;
    virtual void postaviOznakuGrane(int polazni, int dolazni, TipOznake oznaka) = 0;
    virtual TipOznake dajOznakuGrane(int polazni, int dolazni) = 0;
    virtual Grana<TipOznake> dajGranu(int polazni, int dolazni) = 0;
    virtual Cvor<TipOznake> dajCvor(int br_c) = 0;
    virtual GranaIterator<TipOznake> dajGranePocetak() = 0;
    virtual GranaIterator<TipOznake> dajGraneKraj() = 0;
    virtual GranaIterator<TipOznake> sljedecaGrana(int polazni, int dolazni) = 0;
};

template <typename TipOznake>
class Grana{
    int polazni, dolazni;
    double tezina;
    TipOznake oznaka;
    UsmjereniGraf<TipOznake> *usmjereniGraf;
public:
    Grana(UsmjereniGraf<TipOznake> *usmjereni, int p, int d){
        usmjereniGraf = usmjereni;
        polazni = p;
        dolazni = d;
    }
    double dajTezinu() const{
        return usmjereniGraf->dajTezinuGrane(polazni, dolazni);
    }
    void postaviTezinu(double tezina){
        this->tezina = tezina;
        usmjereniGraf->postaviTezinuGrane(polazni, dolazni, tezina);
    }
    TipOznake dajOznaku() const{
        return usmjereniGraf->dajOznakuGrane(polazni, dolazni);
    }
    void postaviOznaku(TipOznake oznaka){
        this->oznaka = oznaka;
        usmjereniGraf->postaviOznakuGrane(polazni, dolazni, oznaka);
    }
    Cvor<TipOznake> dajPolazniCvor() const{
        return usmjereniGraf->dajCvor(polazni);
    }
    Cvor<TipOznake> dajDolazniCvor() const{
        return usmjereniGraf->dajCvor(dolazni);
    }
};

template<typename TipOznake>
class Cvor{
    int br_c;
    TipOznake oznaka;
    UsmjereniGraf<TipOznake> *usmjereniGraf;
public:
    Cvor(UsmjereniGraf<TipOznake> *usmjereni, int broj){
        usmjereniGraf = usmjereni;
        br_c = broj;
    }
    TipOznake dajOznaku() const{
        return usmjereniGraf->dajOznakuCvora(br_c);
    }
    void postaviOznaku(TipOznake oznaka){
        this->oznaka = oznaka;
        usmjereniGraf->postaviOznakuCvora(br_c, oznaka);
    }
    int dajRedniBroj() const{
        return br_c;
    }
};

template <typename TipOznake>
class GranaIterator {
    UsmjereniGraf<TipOznake> *usmjereniGraf;
    int polazni, dolazni;
public:
    GranaIterator(UsmjereniGraf<TipOznake> *usmjereni, int p, int d){
        polazni = p;
        dolazni = d;
        usmjereniGraf = usmjereni;
    }
    Grana<TipOznake> operator*(){
        return usmjereniGraf->dajGranu(polazni, dolazni);
    }
    bool operator==(const GranaIterator &iter) const{
        if(iter.usmjereniGraf == usmjereniGraf && iter.polazni == polazni && iter.dolazni == dolazni) return true;
        return false;
    }
    bool operator!=(const GranaIterator &iter) const{
        if(iter.usmjereniGraf == usmjereniGraf && iter.polazni == polazni && iter.dolazni == dolazni) return false;
        return true;
    }
    GranaIterator& operator++(){
        GranaIterator<TipOznake> slj = usmjereniGraf->sljedecaGrana(polazni, dolazni);
        polazni = slj.polazni;
        dolazni = slj.dolazni;
        return *this;
    }
    GranaIterator operator++(int){
        GranaIterator<TipOznake> temp = *this;
        ++*this;
        return temp;
    }
};

template<typename TipOznake>
class MatricaGraf: public UsmjereniGraf<TipOznake>{
    struct Element{
        double tezina;
        TipOznake oznaka;
        int br_c = 0;
        Element(){}
        Element(int brc){br_c = brc;}
        Element(int brc, double t){
            br_c = brc;
            tezina = t;
        }
    };
    std::vector<std::vector<Element>> elementi;
    std::vector<TipOznake> cvorovi;
public:
    MatricaGraf(){}
    MatricaGraf(int br_c): UsmjereniGraf<TipOznake>(br_c) {
        if(br_c < 0) 
            throw std::logic_error("Ne moze biti negativan broj cvorova!");
        elementi.resize(br_c);
        for(int i = 0; i < br_c; i++){
            Element e(0);
            elementi.at(i).resize(br_c, e);
        }
        cvorovi.resize(br_c);
    }
    ~MatricaGraf() {}
    int dajBrojCvorova() const override{
        return elementi.size();
    }
    void postaviBrojCvorova(int br_c) override{
        if(br_c < 0)
            throw std::logic_error("Ne moze biti negativan broj cvorova!");
        cvorovi.resize(br_c);
        std::vector<Element> temp(br_c);
        for(int i=0; i<br_c; i++)
            temp.at(i) = Element(0);
        elementi.resize(br_c);
        for(int i=0; i<br_c; i++)
            elementi.at(i) = temp;
    }
    void dodajGranu(int polazni, int dolazni, double tezina) override{
        if(!postojiGrana(polazni, dolazni))
            elementi.at(polazni).at(dolazni) = Element(1, tezina);
        else
            throw std::logic_error("Vec postojeca grana!");
    }
    void obrisiGranu(int polazni, int dolazni) override{
        elementi.at(polazni).at(dolazni).br_c = 0;
    }
    void postaviTezinuGrane(int polazni, int dolazni, double tezina) override{
        elementi.at(polazni).at(dolazni).tezina = tezina;
    }
    double dajTezinuGrane(int polazni, int dolazni) override{
        return elementi.at(polazni).at(dolazni).tezina;
    }
    bool postojiGrana(int polazni, int dolazni) override{
        return elementi.at(polazni).at(dolazni).br_c;
    }
    void postaviOznakuCvora(int br_c, TipOznake oznaka) override{
        cvorovi.at(br_c) = oznaka;
    }
    TipOznake dajOznakuCvora(int br_c) override{
        return cvorovi.at(br_c);
    }
    void postaviOznakuGrane(int polazni, int dolazni, TipOznake oznaka) override{
        elementi.at(polazni).at(dolazni).oznaka = oznaka;
    }
    TipOznake dajOznakuGrane(int polazni, int dolazni) override{
        return elementi.at(polazni).at(dolazni).oznaka;
    }
    Grana<TipOznake> dajGranu(int polazni, int dolazni) override{
        return Grana<TipOznake>(this, polazni, dolazni);
    }
    Cvor<TipOznake> dajCvor(int br_c) override{
        return Cvor<TipOznake>(this, br_c);
    }
    GranaIterator<TipOznake> dajGranePocetak() override{
        return ++GranaIterator<TipOznake>(this, 0, -1);
    }
    GranaIterator<TipOznake> dajGraneKraj() override{
        return GranaIterator<TipOznake>(this, -1, -1);
    }
    GranaIterator<TipOznake> sljedecaGrana(int polazni, int dolazni) override{
        int pocetak = -1;
        bool da = false;
        int prva, druga;
        UsmjereniGraf<TipOznake> *temp = this;
        for(int i = polazni; i<elementi.size(); i++){
            for(int j=0; j<elementi.size(); j++){
                int uslov = 0;
                if(i == polazni) uslov++;
                if(j<= dolazni) uslov++;
                if(uslov != 2){
                    if(elementi.at(i).at(j).br_c){
                        prva = i;
                        druga = j;
                        da = true;
                        if(da) return GranaIterator<TipOznake> (temp, prva, druga);
                    }
                }
            }
        }
        return GranaIterator<TipOznake> (temp, pocetak, pocetak);
    }
};

template <typename TipOznake>
void bfs(UsmjereniGraf<TipOznake> *usmjereniGraf, vector<Cvor<TipOznake>> &bfss, Cvor<TipOznake> cvoric){
    cvoric.postaviOznaku(1);
    bfss.push_back(cvoric);
    std::queue<Cvor<TipOznake>> red;
    red.push(cvoric);
    while (!red.empty()) {
        Cvor<TipOznake> cvor1 = red.front();
        red.pop();
        GranaIterator<TipOznake> grana = usmjereniGraf->dajGranePocetak();
        for(GranaIterator<TipOznake> i = grana; i != usmjereniGraf->dajGraneKraj(); i++){
            Cvor<TipOznake> polazni = (*i).dajPolazniCvor();
            Cvor<TipOznake> dolazni = (*i).dajDolazniCvor();
            if(dolazni.dajOznaku()!=1 && polazni.dajRedniBroj() == cvor1.dajRedniBroj()){
                dolazni.postaviOznaku(1);
                bfss.push_back(dolazni);
                red.push(dolazni);
            }
        }
    }
}

template <typename TipOznake>
void dfs(UsmjereniGraf<TipOznake> *usmjereniGraf, vector<Cvor<TipOznake>> &dfss, Cvor<TipOznake> cvoric){
    cvoric.postaviOznaku(1);
    dfss.push_back(cvoric);
    GranaIterator<TipOznake> pocetakgrane = usmjereniGraf->dajGranePocetak();
    GranaIterator<TipOznake> i = pocetakgrane;
    i++;
    for(; i!= usmjereniGraf->dajGraneKraj(); i++){
        Cvor<TipOznake> dolazni;
        dolazni = (*i).dajDolazniCvor();
        if(dolazni.dajOznaku()!=1 && (*i).dajPolazniCvor().dajRedniBroj() == cvoric.dajRedniBroj()) dfs(usmjereniGraf, dfss, dolazni);
    }
}

int main(){
    
    try {
        UsmjereniGraf<bool> *g = new MatricaGraf<bool>(3);
        g->dodajGranu(0, 1, 2.5);
        g->dodajGranu(1, 0, 1.2);
        g->dodajGranu(1, 2, 0.1);
        g->dodajGranu(0, 0, 3.14);
        for (GranaIterator<bool> iter = g->dajGranePocetak(); iter != g->dajGraneKraj(); ++iter)
            cout << (*iter).dajPolazniCvor().dajRedniBroj() << " " << (*iter).dajDolazniCvor().dajRedniBroj() << " " << (*iter).dajTezinu() << endl;
            delete g;
    } catch (const char izuzetak[]) {
        cout << izuzetak << endl;
    }
    return 0;
}