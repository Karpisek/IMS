//
// Created by Miro on 06/12/2018.
//

#include <iostream>
#include <list>
#include "simlib.h"
#include <math.h>

#define KAPACITA_MLATICKY_V_STOKILOGRAMECH 80

using namespace std;

// TODO mozno deditm se store -> output();
class Hektar {
public:
    int vynos;      //  v sto-kilogramech
    double doba;    //  doba sklizne sto-kilogramu v sekundach

    Hektar() {
        this->vynos = (int)Uniform(43, 49);         // TODO NAHODNE GENEROVANI
        this->doba = ((int)((Uniform(12, 20)/this->vynos) * 10 ))/ 10.0 ; // TODO DOPOCITAT DOBU PODLE VYGENEROVANI


//        cout << "vynos: " << this->vynos << ", " << this->doba <<" min/100kg"<< endl;
    }
};

list<Hektar *> hektary;

class Mlaticka: public Process {

public:
    Store *kapacita;     // vnitrni kapacita
    int id;

    Mlaticka(int id) {
        kapacita = new Store(KAPACITA_MLATICKY_V_STOKILOGRAMECH);
        this->id = id;

        cout << "vytvoril sem mlaticku " << id << endl;
    }

    void Behavior() {
        int count = 0;
        while(hektary.size() > 0) {
            count++;
            Hektar *hektar = hektary.front();
            hektary.pop_front();

            Store *stoKilogramy = new Store(hektar->vynos);
            stoKilogramy->Enter(this, hektar->vynos);

            cout << "mlaticka" << id << " jde po novem hektaru o vleikost: "<< hektar->vynos << endl;

            while(!stoKilogramy->Empty()) {
                Leave(*stoKilogramy, 1);     // vezme jeden sto-kilogram
                Wait(hektar->doba);         // sklizen jednoho sto-kilogramu
                Enter(*kapacita, 1);         // pridani do vlastni kapacity

                if(kapacita->Full()) {
                    // TODO cekani na traktor
                    cout << "Mlaticka " << id << " je plna v: " << Time << endl;
                    cout << "zbyva sklidi:" << stoKilogramy->Used() << endl;
                    Leave(*kapacita, KAPACITA_MLATICKY_V_STOKILOGRAMECH);     // oddelani 8 z kapacity
                }

                else {
                               // pokracovani sklizne
                }

            }
        }
    }
};

class StoKilogram: public Store {

};

int main(int argc, char **argv) {

    Init(0, 120000);
    RandomSeed(time(nullptr));

    int rozloha = atoi(argv[1]);
    int pocetMlaticek = atoi(argv[2]);
    int pocetNakl = atoi(argv[3]);
    int vzdalenost = atoi(argv[4]);

    for(int x = 0; x < rozloha; x++) {
        hektary.push_back(new Hektar());
    }

    cout << "Pocet hektaru: " << hektary.size() << endl;
    cout << "Pocet mlaticek: " << pocetMlaticek << endl;
    cout << "Pocet nakladaku: " << pocetNakl << endl;
    cout << "Vzdalenost zasobniku: " << vzdalenost << endl;

    // kombajny
    list<Mlaticka *> mlaticky;
    for(int x=0; x < pocetMlaticek ; x++) {
//        Mlaticka mlaticka(x);
//        mlaticky.push_back(&mlaticka);
//        mlaticka.Activate();

        Mlaticka *mlaticka = new Mlaticka(x);
        mlaticky.push_back(mlaticka);
        mlaticka->Activate();

    }

    Run();


    cout << "hello" << endl;
}

