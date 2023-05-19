#include <iostream>
#include <list>
#include <vector>
using namespace std;

//Kopiram klasu Mapa iz z4
template <typename TipKljuca, typename TipVrijednosti>
class Mapa{
public:
    Mapa(){}
    virtual ~Mapa(){}
    virtual int brojElemenata() const = 0;
    virtual void obrisi() = 0;
    virtual void obrisi(const TipKljuca& kljuc) = 0;
    virtual TipVrijednosti &operator[](const TipKljuca &kljuc) = 0;
};

template <typename TipKljuca, typename TipVrijednosti>
class HashMapaLan: public Mapa<TipKljuca, TipVrijednosti>{
    pair<TipKljuca, TipVrijednosti> parovi;
    int velicina;
    int brojEl;
    list<pair<TipKljuca, TipVrijednosti>> *lista;
    unsigned int (*hashFunkcija)(TipKljuca kljuc, unsigned int max);
public:
    HashMapaLan(){
        lista = new list<pair<TipKljuca, TipVrijednosti>>[100];
        velicina = 100;
        brojEl = 0;
        hashFunkcija = nullptr;
    }
    ~HashMapaLan(){
        delete[] lista;
    }
    HashMapaLan(HashMapaLan const &mapa){
        velicina = mapa.velicina;
        lista = new list<pair<TipKljuca, TipVrijednosti>>[velicina];
        for(int i = 0; i < 100; i++) 
            lista[i] = mapa.lista[i];
        brojEl = mapa.brojEl;
        hashFunkcija = mapa.hashFunkcija;
    }
    HashMapaLan &operator =(const HashMapaLan<TipKljuca, TipVrijednosti> &mapa){
        if(lista != mapa.lista){
            delete[] lista;
            velicina = mapa.velicina;
            lista = new list<pair<TipKljuca, TipVrijednosti>>[velicina];
            for(int i = 0; i < 100; i++)
                lista[i] = mapa.lista[i];
            brojEl = mapa.brojEl;
            hashFunkcija = mapa.hashFunkcija;
        }
        return *this;
    }
    void definisiHashFunkciju(unsigned int (*fja)(TipKljuca kljuc, unsigned int opseg)){
        hashFunkcija = fja;
    }
    int brojElemenata() const {
        return brojEl;
    }
    void obrisi() {
        delete [] lista;
        lista = new list<pair<TipKljuca, TipVrijednosti>>[100];
        brojEl = 0;
    }
    void obrisi(const TipKljuca &kljuc) {
        int pozicija(1);
        unsigned int i;
        do{
            i = hashFunkcija(kljuc, pozicija);
            for(auto it = lista[i].begin(); it != lista[i].end(); ++it){
                if((*it).first == kljuc){
                    it = lista[i].erase(it);
                    brojEl--;
                }
            }
            pozicija++;
        } while(pozicija < velicina);
    }
    TipVrijednosti &operator [](const TipKljuca &kljuc) {
        if(hashFunkcija == nullptr) throw logic_error("Nije definisana hash funkcija!");
        if(brojEl==100){
            list<pair<TipKljuca, TipVrijednosti>> *temp = new list<pair<TipKljuca, TipVrijednosti>>[velicina + 100];
            std::copy(lista, lista + brojEl, temp);
            delete[] lista;
            lista = temp;
            velicina = velicina + 100;
        }
        int pozicija(1);
        unsigned int i;
        do{
            i = hashFunkcija(kljuc, pozicija);
            for(auto &it: lista[i]){
                if(it.first == kljuc) return it.second;
            }
            pozicija++;
        }while(pozicija < velicina);
        pair<TipKljuca, TipVrijednosti> temp1;
        temp1.first = kljuc;
        lista[i].push_back(temp1);
        brojEl++;
        for(auto &it: lista[i]){
            if(it.first == kljuc) return it.second;
        }
        return parovi.second;
    }
    TipVrijednosti const &operator[](const TipKljuca &kljuc) const {
        int pozicija(1);
        unsigned int i;
        do{
            i = hashFunkcija(kljuc, pozicija);
            for(auto &it: lista[i]){
                if(it.first == kljuc) return it.second;
            }
            pozicija++;
        } while(pozicija < velicina);
        return parovi.second;
    }
};

//kopiram klase UsmjereniGraf, Cvor, Grana, GranaIterator iz pz10

template <typename TipOznake>
class Grana;

template <typename TipOznake>
class Cvor;

template <typename TipOznake>
class GranaIterator;

template <typename TipOznake>
class UsmjereniGraf{
public:
    //UsmjereniGraf() {}
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
    virtual Grana<TipOznake> dajGranu(int polazni, int dolazni) {
        return Grana<TipOznake> (this, polazni, dolazni);
    }
    virtual Cvor<TipOznake> dajCvor(int br_c) {
        return Cvor<TipOznake>(this, br_c);
    }
    virtual GranaIterator<TipOznake> dajGranePocetak() = 0; 
    /*{
        GranaIterator<TipOznake> g(this, 0, -1);
        return ++g;
    }*/
    virtual GranaIterator<TipOznake> dajGraneKraj() = 0;
    /*{
        return GranaIterator<TipOznake>(this, -1, -1);
    }*/
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

template <typename TipOznake>
class ListaGraf: public UsmjereniGraf<TipOznake> {
    struct Grancica{
        TipOznake oznaka;
        double tezina;
        bool postoji;
    };
    vector<TipOznake> oznake;
    vector<vector<Grancica>> informacije;
public:
    ListaGraf(int brojCvorova): UsmjereniGraf<TipOznake>(brojCvorova){
        postaviBrojCvorova(brojCvorova);
    }
    int dajBrojCvorova() const override{
        return informacije.size();
    }
    void postaviBrojCvorova(int brojCvorova) override{
        Grancica inf;
        inf.postoji = false;
        for(int i = 0; i < informacije.size(); i++)
            informacije.at(i).resize(brojCvorova, inf);
        vector<Grancica> dodaj(brojCvorova, inf);
        informacije.resize(brojCvorova, dodaj);
        oznake.resize(brojCvorova);
    }
    void dodajGranu(int polazni, int dolazni, double tezina) override{
        Grancica inf;
        inf.postoji = true;
        inf.tezina = tezina;
        informacije.at(polazni).at(dolazni) = inf;
    }
    void obrisiGranu(int polazni, int dolazni)override {
        informacije.at(polazni).at(dolazni).postoji = false;

    }
    void postaviTezinuGrane(int polazni, int dolazni, double tezina) override{
        informacije.at(polazni).at(dolazni).tezina = tezina;
    }
    double dajTezinuGrane(int polazni, int dolazni) override{
        return informacije.at(polazni).at(dolazni).tezina;
    }
    bool postojiGrana(int polazni, int dolazni) override{
        return informacije.at(polazni).at(dolazni).postoji;
    }
    void postaviOznakuCvora(int br_c, TipOznake oznaka) override{
        oznake.at(br_c) = oznaka;
    }
    TipOznake dajOznakuCvora(int br_c) override{
        return oznake.at(br_c);
    }
    void postaviOznakuGrane(int polazni, int dolazni, TipOznake oznaka) override{
        informacije.at(polazni).at(dolazni).oznaka = oznaka;
    }
    TipOznake dajOznakuGrane(int polazni, int dolazni) override{
        return informacije.at(polazni).at(dolazni).oznaka;
    }
    /*Grana<TipOznake> dajGranu(int polazni, int dolazni){
        return Grana<TipOznake>(this, polazni, dolazni);
    }
    Cvor<TipOznake> dajCvor(int br_c){
        return Cvor<TipOznake>(this, br_c);
    }*/
    virtual GranaIterator<TipOznake> dajGranePocetak() override{
        GranaIterator<TipOznake> g(this, 0, -1);
        return ++g;
    }
    virtual GranaIterator<TipOznake> dajGraneKraj() override{
        return GranaIterator<TipOznake>(this, -1, -1);
    }
    virtual GranaIterator<TipOznake> sljedecaGrana(int polazni, int dolazni) override{
        for(int i = polazni; i < informacije.size(); i++){
            for(int j = 0; j < informacije.size(); j++){
                if(i == polazni && j <= dolazni) continue;
                if(informacije.at(i).at(j).postoji) return GranaIterator<TipOznake>(this, i, j);
            }
        }
        return GranaIterator<TipOznake>(this, -1, -1);
    }
};

template <typename TipOznake>
void dfs(UsmjereniGraf<TipOznake> *usmjereniGraf, vector<Cvor<TipOznake>> &obilazak, Cvor<TipOznake> cvoric){
    bool posjecen = false;
    for(int i = 0; i < obilazak.size(); i++){
        if(cvoric.dajRedniBroj() == obilazak.at(i).dajRedniBroj()){
            posjecen = true;
            break;
        }
    }
    if(posjecen == false) obilazak.push_back(cvoric);
    int n = usmjereniGraf->dajBrojCvorova();
    for(int i = 0; i < n; i++){
        if(posjecen == false && usmjereniGraf -> postojiGrana(cvoric.dajRedniBroj(), i))
            dfs(usmjereniGraf, obilazak, usmjereniGraf->dajCvor(i));
    }
}


template<typename TipOznake>
void bfs(UsmjereniGraf<TipOznake> *usmjereniGraf, vector<Cvor<TipOznake>> &obilazak, Cvor<TipOznake> cvoric){
    bool posjecen = false;
    for(int i = 0; i < obilazak.size(); i++){
        if(cvoric.dajRedniBroj() == obilazak.at(i).dajRedniBroj()){
            posjecen = true;
            break;
        }
    }
    if(posjecen == false) obilazak.push_back(cvoric);
    int n = usmjereniGraf->dajBrojCvorova();
    for(int i = 0; i < n; i++){
        if(posjecen == false) bfs(usmjereniGraf, obilazak, usmjereniGraf->dajCvor(i));
    }
}

int main(){
    return 0;
}