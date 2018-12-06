//
// Created by Miro on 06/12/2018.
//

#include <iostream>
#include <list>
#include "simlib.h"
#include <math.h>

using namespace std;

// TODO mozno deditm se store -> output();
class Hektar {
public:
    int vynos;      //  v sto-kilogramech
    double doba;    //  doba sklizne sto-kilogramu v sekundach

    Hektar() {
        this->vynos = (int)Uniform(43, 49);         // TODO NAHODNE GENEROVANI
        this->doba = round((Uniform(720, 1200)/this->vynos)); // TODO DOPOCITAT DOBU PODLE VYGENEROVANI

        cout << "vynos: " << this->vynos << ", " << this->doba <<" sec/100kg"<< endl;
    }
};

list<Hektar *> hektary;

class Mlaticka: public Process {
    Store *kapacita;     // vnitrni kapacita

    Mlaticka() {
        kapacita = new Store(8);
    }

    void Behavior() {
        while(hektary.size() > 0) {
            Hektar *hektar = hektary.front();
            hektary.pop_front();

            Store *stoKilogramy = new Store(hektar->vynos);

            while(!stoKilogramy->Empty()) {
                Enter(*stoKilogramy, 1);     // vezme jeden sto-kilogram
                Wait(hektar->doba);         // sklizen jednoho sto-kilogramu
                Leave(*kapacita, 1);         // pridani do vlastni kapacity

                if(kapacita->Full()) {
                    // TODO cekani na traktor
                    Enter(*kapacita, 8);     // oddelani 8 z kapacity
                }

                else {
                    continue;           // pokracovani sklizne
                }

            }
        }
    }
};

class StoKilogram: public Store {

};

int main(int argc, char **argv) {

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

//    // kombajny
//    list<Mlaticka *> mlaticky;
//    for(int x=0; x < pocetMlaticek ; x++) {
//        pole.push_back(new Mlaticka());
//    }


    cout << "hello" << endl;
}

