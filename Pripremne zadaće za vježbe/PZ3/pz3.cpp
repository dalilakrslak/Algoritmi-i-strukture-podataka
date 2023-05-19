#include <iostream>
#include <pthread.h>
#include <stdexcept>
using namespace std;

template <typename Tip> struct Cvor {
  Tip element;
  Cvor<Tip> *veza;
  Cvor(const Tip &el, Cvor *slj) {
    element = el;
    veza = slj;
  }
};

template <typename Tip> class Stek {
  int broj_elemenata;
  Cvor<Tip> *gore;

public:
  Stek() {
    broj_elemenata = 0;
    gore = nullptr;
  }
  ~Stek() { 
      brisi(); 
    }
  Stek(const Stek &s) {
    broj_elemenata = s.broj_elemenata;
    gore = s.gore;
    Cvor<Tip> *stekkk = nullptr;
    Cvor<Tip> *vezaaaa = nullptr;
    Cvor<Tip> *goreee = s.gore;
    while (goreee != nullptr) {
      stekkk = new Cvor<Tip>(goreee->element, 0);
      if (goreee == s.gore) gore = stekkk;
      else vezaaaa->veza=stekkk;
      goreee = goreee->veza;
      vezaaaa=stekkk;
    }
    vezaaaa->veza = nullptr;
    broj_elemenata=s.broj_elemenata;
  }
  Stek &operator=(const Stek &s) {
    if (this == &s)
      return *this;
    brisi();
    Cvor<Tip> *stekkk = nullptr;
    Cvor<Tip> *vezaaaa = nullptr;
    Cvor<Tip> *goreee = s.gore;
    while (goreee != nullptr) {
      stekkk = new Cvor<Tip>(goreee->element, 0);
      if (goreee == s.gore)
        gore = stekkk;
      else
        vezaaaa->veza = stekkk;
      goreee = goreee->veza;
      vezaaaa = stekkk;
    }
    vezaaaa->veza;
    broj_elemenata = s.broj_elemenata;
    return *this;
  }
  void brisi() {
    Cvor<Tip> *it = gore;
    while (it!=nullptr) {
      auto pom = it;
      it = it->veza;
      delete pom;
    }
    gore=nullptr;
    broj_elemenata=0;
  }
  void stavi(const Tip &el) {
    auto pom = gore;
    gore = new Cvor<Tip>{el, nullptr};
    gore->element = el;
    gore->veza = pom;
    broj_elemenata++;
  }
  Tip skini() {
    if (broj_elemenata == 0)
      throw domain_error("Stek je prazan!");
    auto el_na_vrhu = gore->element;
    auto pom = gore;
    gore = gore->veza;
    delete pom;
    broj_elemenata--;
    return el_na_vrhu;
  }
  Tip &vrh() {
    if (broj_elemenata == 0)
      throw domain_error("Stek je prazan!");
    return gore->element;
  }
  int brojElemenata() { return broj_elemenata; }
};

void testKonstruktorBezParametara() {
  Stek<int> s;
  std::cout << "Konstruktor bez parametara: ";
  if (s.brojElemenata() == 0)
    std::cout << "OK!" << std::endl;
  else
    std::cout << "NOK!" << std::endl;
}

void testMetodaBrisi() {
  Stek<int> s;
  for (int i = 0; i < 5; i++)
    s.stavi(i);
  s.brisi();
  std::cout << "Metoda brisi: ";
  if (s.brojElemenata() == 0)
    std::cout << "OK!" << std::endl;
  else
    std::cout << "NOK!" << std::endl;
}

void testMetodaStavi() {
  Stek<int> s;
  for (int i = 0; i < 5; i++)
    s.stavi(i);
  std::cout << "Metoda stavi: ";
  if (s.brojElemenata() == 5)
    std::cout << "OK!" << std::endl;
  else
    std::cout << "NOK!" << std::endl;
}

void testMetodaSkini() {
  Stek<int> s;
  for (int i = 0; i < 5; i++)
    s.stavi(i);
  int duzina1 = s.brojElemenata();
  s.skini();
  std::cout << "Metoda skini: ";
  if (s.brojElemenata() == duzina1 - 1)
    std::cout << "OK!" << std::endl;
  else
    std::cout << "NOK!" << std::endl;
}

void testMetodaVrh() {
  Stek<int> s;
  for (int i = 0; i < 5; i++)
    s.stavi(i);
  std::cout << "Metoda vrh: ";
  if (s.vrh() == 4)
    std::cout << "OK!" << std::endl;
  else
    std::cout << "NOK!" << std::endl;
}

void testMetodaBrojElemenata(){
    Stek<int> s;
    for(int i=0; i<5; i++) s.stavi(i);
    std::cout<<"Metoda broj elemenata: ";
    if(s.brojElemenata()==5) std::cout<<"OK!"<<std::endl;
    else cout<<"NOK!"<<endl;
}


int main() {
  testKonstruktorBezParametara();
  testMetodaBrisi();
  testMetodaStavi();
  testMetodaSkini();
  testMetodaVrh();
  testMetodaBrojElemenata();
  return 0;
}