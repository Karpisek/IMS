//
// Created by Miro on 06/12/2018.
//

#include <iostream>
#include <list>
#include "simlib.h"
#include <math.h>
#include <algorithm>

#define KAPACITA_TRAKTORU 120   // ve sto-kilogramech
#define KAPACITA_MLATICKY 80    // ve sto-kilogramech
#define MINIMALNI_KAPACITA 0.5  // 0...1 minimalni kapacita pro vyprazdneni

using namespace std;

class Traktor;
class Hektar;
class Mlaticka;

// TODO mozno deditm se store -> output();
class Hektar {
public:
    int vynos;      //  v sto-kilogramech
    double doba;    //  doba sklizne sto-kilogramu v sekundach

    Hektar() {
        this->vynos = (int)Uniform(43, 49);         // TODO NAHODNE GENEROVANI
        this->doba = ((int)((Uniform(12, 20)/this->vynos) * 10 ))/ 10.0 ; // TODO DOPOCITAT DOBU PODLE VYGENEROVANI
    }
};

class Mlaticka: public Process {
private:
    list<Hektar *> *hektary;
    list<Mlaticka *> *mlaticky;

public:
    bool stop;
    Store *kapacita;     // vnitrni kapacita
    int id;

    Mlaticka(int id, list<Hektar *> *hektary, list<Mlaticka *> *mlaticky) {
        this->kapacita = new Store(KAPACITA_MLATICKY);
        this->id = id;
        this->hektary = hektary;
        this->mlaticky = mlaticky;

        this->stop = false;

        this->Activate();
    }

    void Behavior() {
        while(hektary->size() > 0) {

            Hektar *hektar = hektary->front();
            hektary->pop_front();

            Store stoKilogramy(hektar->vynos);
            stoKilogramy.Enter(this, hektar->vynos);

            while(!stoKilogramy.Empty()) {
                Leave(stoKilogramy, 1);     // vezme jeden sto-kilogram
                Wait(hektar->doba);         // sklizen jednoho sto-kilogramu
                Enter(*kapacita, 1);        // pridani do vlastni kapacity

                if(kapacita->Full()) {
                    cout << "Mlaticka " << id << " je plna v: " << Time << endl;
                    this->stop = true;
                    this->Passivate();
                }

                else {
                               // pokracovani sklizne
                }

            }
        }

        mlaticky->remove(this);
        this->Terminate();
    }

    void Activate() {
        this->stop = false;
        Process::Activate();
    }
};

class Traktor: public Process {
private:
    list<Mlaticka *> *mlaticky;
    list<Traktor *> *traktory;

public:
    Store *kapacita;     // vnitrni kapacita
    int id;

    Traktor(int id, list<Mlaticka *> *mlaticky, list<Traktor *> *traktory) {
        this->kapacita = new Store(KAPACITA_TRAKTORU);
        this->id = id;
        this->mlaticky = mlaticky;
        this->traktory = traktory;

        this->Activate();
    }

    void Behavior() {

        while(mlaticky->size() > 0) {
            // nalzeni nejplnejsi mlaticky

            while(!this->kapacita->Full() && mlaticky->size() != 0) {
                // dokud neni traktor plny

                Mlaticka *nejplnejsi = nullptr;
                for (auto const& mlaticka: *mlaticky) {

                    if(mlaticka->kapacita->Full()) {
                        nejplnejsi = mlaticka;
                        break;
                    }

                    if(mlaticka->kapacita->Used() < mlaticka->kapacita->Capacity() * MINIMALNI_KAPACITA) {
                        continue;
                    }

                    if(nejplnejsi == nullptr) {
                        nejplnejsi = mlaticka;
                    }

                    else if(nejplnejsi->kapacita->Used() < mlaticka->kapacita->Used()) {
                        nejplnejsi = mlaticka;
                    }
                }

                // vylozeni nejplnejsi mlaticky
                if(nejplnejsi != nullptr) {

                    while(!kapacita->Full() && !nejplnejsi->kapacita->Empty()) {
                        nejplnejsi->kapacita->Leave(1);
                        Enter(*kapacita, 1);

                        if(nejplnejsi->stop) {
                            nejplnejsi->Activate();
                        }

                        // vyprazdeni 100kg trva 0.02 minut
                        Wait(0.02);
                    }
                }

                Wait(0.01);
            }

            // transport plneho nakladaku
            cout << "nakladak odjizdi na 30 minut" << endl;

            this->Wait(40);

            // TODO navazat na sklad
            Leave(*kapacita, kapacita->Used());

        }

        traktory->remove(this);
        this->Terminate();
    }
};

class StoKilogram: public Store {

};

int main(int argc, char **argv) {

    Init(0, 12000);
    RandomSeed(time(nullptr));

    // listy jednotlivych stroju
    list<Hektar *> hektary;
    list<Mlaticka *> mlaticky;
    list<Traktor *> traktory;

    int rozloha = atoi(argv[1]);
    int pocetMlaticek = atoi(argv[2]);
    int pocetTraktoru = atoi(argv[3]);
    int vzdalenost = atoi(argv[4]);

    for(int x = 0; x < rozloha; x++) {
        hektary.push_back(new Hektar());
    }

    cout << "Pocet hektaru: " << hektary.size() << endl;
    cout << "Pocet mlaticek: " << pocetMlaticek << endl;
    cout << "Pocet nakladaku: " << pocetTraktoru << endl;
    cout << "Vzdalenost zasobniku: " << vzdalenost << endl;

    // Vytvareni mlaticek
    for(int x=0; x < pocetMlaticek ; x++) {
        mlaticky.push_back(new Mlaticka(x, &hektary, &mlaticky));
    }

    // Vytvareni traktoru
    for(int x=0; x < pocetTraktoru ; x++) {
        traktory.push_back(new Traktor(x, &mlaticky, &traktory));
    }

    Run();

    cout << "hello" << endl;
}

