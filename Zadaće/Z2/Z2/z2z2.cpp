#include <iostream>
#include <stdexcept>
#include <vector>
using namespace std;

template <typename Tip> struct Cvor{
    Tip element;
    Cvor<Tip> *veza;
    Cvor(const Tip &el, Cvor *slj){
        element = el;
        veza = slj;
    }
};

template <typename Tip> class Stek{
    int broj_elemenata;
    Cvor<Tip> *gore;
public:
    Stek(){
        broj_elemenata = 0;
        gore = nullptr;
    }
    ~Stek(){
        brisi();
    }
    Stek(const Stek &s){
        broj_elemenata = s.broj_elemenata;
        gore = s.gore;
        Cvor<Tip> *stekkk = nullptr;
        Cvor<Tip> *vezaaaa = nullptr;
        Cvor<Tip> *goreee = s.gore;
        while(goreee != nullptr){
            stekkk = new Cvor<Tip>(goreee->element, 0);
            if(goreee == s.gore) gore = stekkk;
            else vezaaaa->veza = stekkk;
            goreee = goreee->veza;
            vezaaaa = stekkk;
        }
        vezaaaa->veza = nullptr;
        broj_elemenata = s.broj_elemenata;
    }
    Stek &operator=(const Stek &s){
        if(this == &s){
            return *this;
        }
        brisi();
        Cvor<Tip> *stekkk = nullptr;
        Cvor<Tip> *vezaaaa = nullptr;
        Cvor<Tip> *goreee = s.gore;
        while(goreee != nullptr){
            stekkk = new Cvor<Tip>(gore->element, 0);
            if(goreee == s.gore) gore = stekkk;
            else vezaaaa -> veza = stekkk;
            goreee = goreee ->veza;
            vezaaaa = stekkk;
        }
    }
    void brisi(){
        Cvor<Tip> *it = gore;
        while(it!=nullptr){
            auto pom = it;
            it = it->veza;
            delete pom;
        }
        gore = nullptr;
        broj_elemenata = 0;
    }
    void stavi(const Tip &el){
        auto pom = gore;
        gore = new Cvor<Tip>{el, nullptr};
        gore->element = el;
        gore->veza = pom;
        broj_elemenata++;
    }
    Tip skini(){
        if(broj_elemenata == 0) throw std::domain_error("Stek je prazan!");
        auto el_na_vrhu = gore->element;
        auto pom = gore;
        gore = gore->veza;
        delete pom;
        broj_elemenata--;
        return el_na_vrhu;
    }
    Tip &vrh(){
        if(broj_elemenata == 0) throw std::domain_error("Stek je prazan!");
        return gore->element;
    }
    int brojElemenata(){
        return broj_elemenata;
    }
};
void pretraga(Stek<vector<int>>& s, int trazeni){
    if(s.brojElemenata() == 0){
        cout<<"Nema elementa";
    }
    else{
        vector<int> pomocni = s.skini();
        int velicina = pomocni.size();
        if(velicina!=0){
            int dno(0), srednji(0);
            int vrh = velicina - 1;
            while(dno <= vrh){
                srednji = (dno + vrh)/2;
                if(pomocni[srednji] == trazeni){
                    cout<<srednji<<" "<<s.brojElemenata();
                    s.stavi(pomocni);
                    return;
                }
                else if(pomocni[srednji] < trazeni){
                    dno = srednji + 1;
                }
                else{
                    vrh = srednji - 1;
                }
            }
        }
        pretraga(s, trazeni);
        s.stavi(pomocni);
    }
}

bool test1(){
    Stek<vector<int>> s;
    vector<int> v[3];
    v[0] = {1, 3, 5, 11};
    v[1] = {2, 7, 23, 28};
    v[2] = {8, 15, 17};
    for(int i=0; i<3; i++) s.stavi(v[i]);
    pretraga(s, 23);
    int i = 2;
    while(s.brojElemenata()!=0){
        vector<int> pom = s.skini();
        if(pom != v[i]) return false;
        i--;
    }
    return true;
}

bool test2(){
    Stek<vector<int>> s;
    vector<int> v[3];
    v[0] = {1, 3, 5, 11};
    v[1] = {2, 7, 23, 28};
    v[2] = {8, 15, 17};
    for(int i=0; i<3; i++) s.stavi(v[i]);
    pretraga(s, 11);
    int i = 2;
    while (s.brojElemenata()!=0) {
        vector<int> pom= s.skini();
        if(pom != v[i]) return false;
        i--;
    }
    return true;
}

bool test3(){
    Stek<vector<int>> s;
    vector<int> v[3];
    v[0] = {1, 3, 5, 11};
    v[1] = {2, 7, 23, 28};
    v[2] = {8, 15, 17};
    for(int i=0; i<3; i++) s.stavi(v[i]);
    pretraga(s, 8);
    int i = 2;
    while (s.brojElemenata()!=0) {
        vector<int> pom = s.skini();
        if(pom!=v[i]) return false;
        i--;
    }
    return true;
}

int main(){
    cout<<endl<<"Test 1: ";
    if(test1()) cout<<" OK!"<<endl;
    else cout<<"NOK!"<<endl;
    cout<<"Test 2: ";
    if(test2()) cout<<" OK!"<<endl;
    else cout<<"NOK!"<<endl;
    cout<<"Test 3: ";
    if(test3()) cout<<" OK!"<<endl;
    else cout<<"NOK!"<<endl;
    return 0;
}
