#include <iostream>
#include <stdexcept>
using namespace std;

template<typename Tip> struct Cvor{
    Tip element;
    Cvor<Tip> *veza;
    Cvor(const Tip &el, Cvor *slj){
        element = el;
        veza = slj;
    }
};

template <typename Tip> class Red{
    int broj_elemenata;
    Cvor<Tip> *zaceljee, *celoo;
public:
    Red(){
        broj_elemenata=0;
        zaceljee=nullptr;
        celoo=nullptr;
    }
    ~Red(){
        brisi();
    }
    Red(const Red &r){
        broj_elemenata=r.broj_elemenata;
        Cvor<Tip> *celoooooo=r.celoo;
        Cvor<Tip> *vezica = nullptr;
        Cvor<Tip> *novi=new Cvor<Tip>{celoooooo->element,nullptr};
        celoo=novi;
        celoooooo=celoooooo->veza;
        vezica=celoo;
        while(celoooooo!=nullptr){
            novi=new Cvor<Tip>{celoooooo->element, nullptr};
            if(celoooooo==r.zaceljee) zaceljee=novi;
            vezica->veza=novi;
            vezica=novi;
            celoooooo=celoooooo->veza;
        }
        zaceljee->veza=nullptr;
    }
    Red &operator= (const Red &r){
        if(this == &r)  return *this;
        brisi();
        broj_elemenata=r.broj_elemenata;
        Cvor<Tip> *celoooooo=r.celoo;
        Cvor<Tip> *vezica=nullptr;
        Cvor<Tip> *novi=new Cvor<Tip>{celoooooo->element,nullptr};
        celoo=novi;
        celoooooo=celoooooo->veza;
        vezica=celoo;
        while(celoooooo!=nullptr){
            novi=new Cvor<Tip>{celoooooo->element,nullptr};
            if(celoooooo==r.zaceljee) zaceljee=novi;
            vezica->veza=novi;
            vezica=novi;
            celoooooo=celoooooo->veza;
        }
        zaceljee->veza=nullptr;
        return *this;
    }
    void brisi(){
        while(celoo!=nullptr){
            auto pom=celoo;
            celoo=celoo->veza;
            delete pom;
        }
        zaceljee=nullptr;
        celoo=nullptr;
        broj_elemenata=0;
    }
    void stavi(const Tip &el){
        auto novi = new Cvor<Tip>{el,nullptr};
        if(celoo==nullptr) celoo=novi;
        else if(celoo==zaceljee) celoo->veza=novi;
        else zaceljee->veza=novi;    
        zaceljee=novi;
        broj_elemenata++;
    }
    Tip skini(){
        if(broj_elemenata==0) throw domain_error("Red je prazan!");
        auto el_na_celu=celoo->element;
        auto pom=celoo;
        celoo=celoo->veza;
        delete pom;
        broj_elemenata--;
        return el_na_celu;
    }
    Tip &celo(){
        if(broj_elemenata==0) throw domain_error("Red je prazan!");
        return celoo->element;
    }
    int brojElemenata() {
        return broj_elemenata;
    }
};

void testMetodaBrisi(){
    Red<int> r;
    for(int i=0; i<5; i++) r.stavi(i);
    r.brisi();
    std::cout<<"Metoda brisi: ";
    if(r.brojElemenata()==0) std::cout<<"OK!"<<std::endl;
    else std::cout<<"NOK!"<<std::endl;
}

void testMetodaStavi(){
    Red<int> r;
    for(int i=0; i<5; i++) r.stavi(i);
    std::cout<<"Metoda stavi: ";
    if(r.brojElemenata()==5) std::cout<<"OK!"<<std::endl;
    else std::cout<<"NOK!"<<std::endl;
}

void testMetodaSkini(){
    Red<int> r;
    for(int i=0; i<5; i++) r.stavi(i);
    int duzina1=r.brojElemenata();
    r.skini();
    std::cout<<"Metoda skini: ";
    if(r.brojElemenata()==duzina1-1) std::cout<<"OK!"<<std::endl;
    else std::cout<<"NOK!"<<std::endl;
}

void testMetodaCelo(){
    Red<int> r;;
    for(int i=0; i<5; i++) r.stavi(i);
    std::cout<<"Metoda vrh: ";
    if(r.celo()==0) std::cout<<"OK!"<<std::endl;
    else std::cout<<"NOK!"<<std::endl;
}

void testMetodaBrojElemenata(){
    Red<int> r;
    for(int i=0; i<5; i++) r.stavi(i);
    std::cout<<"Metoda broj elemenata: ";
    if(r.brojElemenata()==5) std::cout<<"OK!"<<std::endl;
    else std::cout<<"NOK!"<<std::endl;
}

int main(){
    testMetodaBrisi();
    testMetodaStavi();
    testMetodaSkini();
    testMetodaCelo();
    testMetodaBrojElemenata();
    return 0;
}