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

template <typename TipKljuca, typename TipVrijednosti>
class AVLStabloMapa : public Mapa<TipKljuca, TipVrijednosti>{
    int velicina;
    struct Cvor{
        TipKljuca kljuc;
        TipVrijednosti vrijednost;
        Cvor *lijevo, *desno, *roditelj;
        int balans;
        Cvor(){
            kljuc();
            vrijednost();
            balans = 0;
            lijevo = nullptr;
            desno = nullptr;
            roditelj = nullptr;
        }
        Cvor(const TipKljuca &k, const TipVrijednosti &vr){
            kljuc = k,
            vrijednost = vr;
            lijevo = nullptr;
            desno = nullptr;
            roditelj = nullptr;
            balans = 0;
        }
    };
    Cvor *korijen=nullptr;

    void ocisti(Cvor *cvoric){
        if(cvoric != nullptr){
            ocisti(cvoric->lijevo);
            ocisti(cvoric->desno);
            delete cvoric;
            velicina--;
        }
    }
    void kopiraj(Cvor *&cvoric1, Cvor *cvoric2){
        //radi balansa sam napisala malo drugaciju pomocnu metodu kopiraj
        if(cvoric2 != nullptr){
            cvoric1 = new Cvor(cvoric2->kljuc, cvoric2->vrijednost);
            cvoric1->roditelj = cvoric2->roditelj;
            cvoric1->lijevo = nullptr;
            cvoric1->desno = nullptr;
            cvoric1->balans = cvoric2->balans;
            kopiraj(cvoric1->lijevo, cvoric2->lijevo);
            kopiraj(cvoric1->desno, cvoric2->desno);
        }
    }
    Cvor *trazi(Cvor *korijenn, TipKljuca kljuc) const{
        if(korijenn == nullptr) return nullptr;
        if(kljuc == korijenn->kljuc) return korijenn;
        if(kljuc < korijenn->kljuc) return trazi(korijenn->lijevo, kljuc);
        else return trazi(korijenn->desno, kljuc);
    }

public:
    AVLStabloMapa(): korijen(nullptr), velicina(0) {}
    AVLStabloMapa(const AVLStabloMapa<TipKljuca, TipVrijednosti> &mapa){
        //korijen = nullptr;
        kopiraj(korijen, mapa.korijen);
        velicina = mapa.velicina;
    }
    ~AVLStabloMapa(){
        /*ocisti(korijen);
        velicina = 0;
        korijen = nullptr;*/
        if(velicina != 0) obrisi();
    }
    AVLStabloMapa &operator = (const AVLStabloMapa<TipKljuca, TipVrijednosti> &mapa){
        if(&mapa != this){
            obrisi();
            velicina = mapa.velicina;
            kopiraj(korijen, mapa.korijen);
        }
        return *this;
    }
    AVLStabloMapa &operator =(AVLStabloMapa<TipKljuca, TipVrijednosti> &&mapa){
        if(&mapa!=this){
            obrisi();
            velicina = mapa.velicina;
            korijen = mapa.korijen;
        }
        return *this;
    }
    int brojElemenata() const override{
        return velicina;
    } 
    /*void kopiraj(Cvor* &cvor1, Cvor* &cvor2){
        if(cvor2 != nullptr){
            cvor1 = new Cvor(cvor2->kljuc, cvor2->vrijednost);
            cvor1->roditelj = cvor2->roditelj;
            cvor1->lijevo = nullptr;
            cvor1->desno = nullptr;
            cvor1->balans = cvor2->balans;
            kopiraj(cvor1->lijevo, cvor2->lijevo);
            kopiraj(cvor1->desno, cvor2->desno);
        }
    }*/
    void obrisi() override{
        ocisti(korijen);
        velicina = 0;
        korijen = nullptr;
    }
    void obrisi(const TipKljuca &kljuc) override{
        //radi dodavanja balansa promjena u metodi
        Cvor *pom1 = korijen;
        Cvor *rr = nullptr;
        Cvor *pom2 = nullptr;
        while(pom1 && kljuc!=pom1->kljuc){
            rr = pom1;
            if(pom1->kljuc < kljuc) pom1 = pom1->desno;
            else pom1 = pom1->lijevo;
        }
        if(pom1 == nullptr) return;

        Cvor *pom3, *pom4;
        if(pom1->lijevo == nullptr){
            pom3 = rr;
            pom2 = pom1 -> desno;
        }
        else{
            if(pom1->desno == nullptr){
                pom3 = rr;
                pom2 = pom1 -> lijevo;
            }
            else{
                pom2 = pom1 -> lijevo;
                pom4 = pom2 -> desno;
                pom3 = pom1;
                while(pom4 != nullptr){
                    pom3 = pom2;
                    pom2 = pom4;
                    pom4 = pom2 -> desno;
                }
                if(pom1 != pom3){
                    pom3->desno = pom2->lijevo;
                    if(pom2->lijevo) pom2->lijevo->roditelj = pom3;
                    pom2->lijevo = pom1->lijevo;
                    if(pom1->lijevo) pom1->lijevo->roditelj = pom2;
                }
                pom2->desno = pom1->desno;
                if(pom1->desno) pom1->desno->roditelj = pom2;
            }
        }
        if(rr == nullptr) korijen = pom2;
        else{
            if(rr->lijevo == pom1){
                rr->lijevo = pom2;
                rr->balans = rr->balans - 1;
            }
            else{
                rr->desno = pom2;
                rr->balans = rr->balans + 1;
            }
        }
        if(pom2 != nullptr) pom2->roditelj = rr;
        pom1->desno = nullptr;
        pom1->lijevo = nullptr;
        pom1->roditelj = nullptr;
        delete pom1;
        velicina--;
    }
    void rotacijaLijevo(Cvor *cvoric){
        //Rotiramo lijevo kada je balans -2, tj. kada imamo 2 desna
        bool uslov = false;
        Cvor *left = cvoric->desno->lijevo;
        cvoric->balans = 0;
        Cvor *right = cvoric->desno;
        Cvor *r_p = cvoric->roditelj;
        right->roditelj = r_p;
        right->lijevo = cvoric;
        if(!uslov && right->roditelj == nullptr){
            uslov = true;
            korijen = right;
        }
        else if(!uslov && r_p->desno == cvoric){
            uslov = true;
            right->roditelj->desno = right;
        }

        if(!uslov)
            right->roditelj->lijevo = right;
        right->balans = 0;
        if(left!=nullptr)
            left->roditelj = cvoric;
        cvoric->desno = left;
        cvoric->roditelj = right;
    }

    void rotacijaDesno(Cvor *cvoric){
        //Rotiramo desno kada je balans 2, tj. kada imamo 2 lijeva 
        bool uslov = false;
        Cvor *right = cvoric->lijevo->desno;
        cvoric->balans = 0;
        Cvor *left = cvoric->lijevo;
        Cvor *l_p = cvoric->roditelj;
        left->roditelj = l_p;
        left->desno = cvoric;
        if(!uslov && left->roditelj == nullptr){
            uslov = true;
            korijen = left;
        }
        else if(!uslov && l_p->desno == cvoric){
            uslov = true;
            left->roditelj->desno = left;
        }

        if(!uslov)
            left->roditelj->lijevo = left;
        left->balans = 0;
        if(right!=nullptr)
            right->roditelj = cvoric;
        cvoric->lijevo = right;
        cvoric->roditelj = left;
    }

    void balans(Cvor *cvoric, bool uslov = false){
        if(cvoric == nullptr) return;
        else if(uslov) cvoric->balans = cvoric->balans - 1;
        else cvoric->balans = cvoric->balans + 1;
        int balansic = cvoric -> balans;
        if(cvoric->roditelj){
            if(cvoric->roditelj->desno!=cvoric) uslov = false;
            else uslov = true;
        }
        if(balansic == -2){
            if(cvoric->desno->balans == -1) 
                rotacijaLijevo(cvoric);
            else{
                rotacijaDesno(cvoric->desno);
                rotacijaLijevo(cvoric);
            }
        }
        else if(balansic == 2){
            if(cvoric->lijevo->balans == 1) 
                rotacijaDesno(cvoric);
            else{
                rotacijaLijevo(cvoric->lijevo);
                rotacijaDesno(cvoric);
            }
        }
        else{
            if(balansic == 0) return;
            balans(cvoric->roditelj, uslov);
        }
    }

    TipVrijednosti &operator[](TipKljuca kljuc) override{
        Cvor *p1;
        p1 = korijen;
        while(p1 != nullptr){
            if((p1->kljuc) == kljuc) 
                return p1->vrijednost;
            else if(kljuc > p1->kljuc)
                p1 = p1->desno;
            else
                p1 = p1->lijevo;
        }

        if(p1 == nullptr){
            velicina++;
            Cvor *p3 = korijen;
            Cvor *p4 = nullptr;
            TipVrijednosti vr = TipVrijednosti();
            p1 = new Cvor(kljuc, vr);
            while(p3 != nullptr){
                p4 = p3;
                if(p3->kljuc > p1->kljuc)
                    p3 = p3->lijevo;
                else
                    p3 = p3->desno;
            }
            if(p4 == nullptr)
                korijen = p1;
            else if(p4->kljuc > p1->kljuc){
                p4->lijevo = p1;
                p1->roditelj = p4;
            }
            else{
                p4->desno = p1;
                p1->roditelj = p4;
            }
            if(p4 == nullptr) 
                return p1->vrijednost;
            if(p4->lijevo && p4->desno)
                p4->balans = 0;
            else{
                int nalaziSe;
                if(p4->lijevo) nalaziSe = 0;
                if(p4->desno) nalaziSe = 1;
                balans(p4, nalaziSe);
            }
        }
        return p1->vrijednost;
    }
    TipVrijednosti operator[](TipKljuca kljuc) const override{
        /*Cvor *cvoric = trazi(korijen, kljuc);
        if(cvoric != nullptr) return cvoric->vrijednost;
        return TipVrijednosti();*/
        Cvor *p1 = korijen;
        while(p1 != nullptr){
            if(p1 -> kljuc == kljuc)
                return p1->vrijednost;
            else if(p1->kljuc > kljuc)
                p1 = p1->lijevo;
            else
                p1 = p1->desno;
        }
        return TipVrijednosti();
    }
};

int main(){
    BinStabloMapa<int, int> binarno;
    AVLStabloMapa<int, int> avl;
    clock_t binarno_pocetak1;
    for(int i = 0; i < 10000; i++) binarno[rand()] = rand();
    clock_t binarno_kraj1;

    clock_t avl_pocetak1;
    for(int i = 0; i < 10000; i++) avl[rand()] = rand();
    clock_t avl_kraj1;

    std::cout<<"Dodavanje 10000 elemenata u binarno stablo traje: "<<(binarno_kraj1 - binarno_pocetak1)/(CLOCKS_PER_SEC/1000)<<" ms, a u AVL: "<<(avl_kraj1 - avl_pocetak1)/(CLOCKS_PER_SEC/1000)<<" ms."<<endl;

    clock_t binarno_pocetak2;
    binarno[10001] = rand();
    clock_t binarno_kraj2;

    clock_t avl_pocetak2;
    avl[10001] = rand();
    clock_t avl_kraj2;

    std::cout<<"Dodavanje jednog elementa u binarno stablo traje: "<<(binarno_kraj2 - binarno_pocetak2)/(CLOCKS_PER_SEC/1000)<<" ms, a u AVL: "<<(avl_kraj2 - avl_pocetak2)/(CLOCKS_PER_SEC/1000)<<" ms."<<endl;
 

    return 0;
}