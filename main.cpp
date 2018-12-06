//
// Created by Miro on 06/12/2018.
//

#include <iostream>
#include <list>
#include "simlib.h"
#include <math.h>


using namespace std;



// TODO mozno dedit se store -> output();
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

class Mlaticka: public Process {

};

class StoKilogram: public Process {
    Mlaticka mlaticka;

    StoKilogram(Mlaticka mlaticka) {
        this->mlaticka = mlaticka;
    }

    void Behavior() {
        Seize(mlaticka);
        Wait(SERVICE);
        Release(mlaticka);
    }
};

int main(int argc, char **argv) {

    int rozloha = atoi(argv[1]);
    int pocetMlaticek = atoi(argv[2]);
    int pocetNakl = atoi(argv[3]);
    int vzdalenost = atoi(argv[4]);


    list<Hektar *> pole;
    for(int x = 0; x < rozloha; x++) {
        pole.push_back(new Hektar());
    }

    cout << "Pocet hektaru: " << pole.size() << endl;
    cout << "Pocet mlaticek: " << pocetMlaticek << endl;
    cout << "Pocet nakladaku: " << pocetNakl << endl;
    cout << "Vzdalenost zasobniku: " << vzdalenost << endl;


    // kombajny
    list<Mlaticka *> mlaticky;
    for(int x=0; x < pocetMlaticek ; x++) {
        pole.push_back(new Mlaticka());
    }


    // jeden hektar a jeden kombajn
    StoKilogram* stoKilogram = new StoKilogram(new Mlaticka);
    stoKilogram->Activate();



    cout << "hello" << endl;
}

