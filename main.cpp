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

#define MIN_DOBA_CESTY 100      // minimalni doba cesty v minutach
#define MAX_DOBA_CESTY 200      // maximalni doba cesty //todo prevest na exp (vsechno je exp)

#define MIST_NA_VYLOZENI 2
#define SILO_CAPACTY 30000      // kapacita sila ve stokilogramech

using namespace std;

class Traktor;
class Hektar;
class Mlaticka;

class Vykladka: public Store {
public:
    Queue queue;

    Vykladka() {
        SetCapacity(MIST_NA_VYLOZENI);
    }

    void Enter(Entity *e, unsigned long rcap) {
        if(Free() > 0) {
            Store::Enter(e, rcap);
        } else {
            queue.InsLast(e);
            e->Passivate();
        }
    }

    void Leave(unsigned long rcap) {
        if(queue.size() == 0) {
            Store::Leave(rcap);
        }
        else {
            queue.GetFirst()->Activate();
        }
    }

};

class Silo: public Store {
public:
    Silo() {
        SetCapacity(10000);
    }
};


// TODO mozno deditm se store -> output();
class Hektar {
public:
    int vynos;      //  v sto-kilogramech
    double doba;    //  doba sklizne sto-kilogramu v sekundach

    Hektar() {
        this->vynos = (int)Uniform(43, 49);
        this->doba = ((int)((Uniform(12, 20)/this->vynos) * 10 ))/ 10.0 ;
    }
};

class Mlaticka: public Process {
private:
    list<Hektar *> *hektary;
    list<Mlaticka *> *mlaticky;

public:
    bool jizVyklada;
    bool stop;
    Store *kapacita;     // vnitrni kapacita
    int id;

    Mlaticka(int id, list<Hektar *> *hektary, list<Mlaticka *> *mlaticky) {
        this->kapacita = new Store(KAPACITA_MLATICKY);
        this->id = id;
        this->hektary = hektary;
        this->mlaticky = mlaticky;

        this->stop = false;
        this->jizVyklada = false;

        this->Activate();
    }

    void Behavior() {
        while(hektary->size() > 0) {

            Hektar *hektar = hektary->front();
            hektary->pop_front();

            Store stoKilogramy(hektar->vynos);
            Enter(stoKilogramy, hektar->vynos);

            while(!stoKilogramy.Empty()) {
                Leave(stoKilogramy, 1);     // vezme jeden sto-kilogram
                Wait(hektar->doba);         // sklizen jednoho sto-kilogramu
                Enter(*kapacita, 1);        // pridani do vlastni kapacity

                if(kapacita->Full()) {
                    cout << "Mlaticka " << id << " je plna v: " << Time << endl;
                    this->stop = true;
                    this->Passivate();
                }

                // pokracovani sklizne az kdyz jej nakladak znovu aktivuje

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

    Vykladka *vykladka;
    Silo *silo;

    int vzdalenost;

public:
    Store *kapacita;     // vnitrni kapacita
    int id;

    Traktor(int id, list<Mlaticka *> *mlaticky, list<Traktor *> *traktory, int vzdalenost, Vykladka *vykladka, Silo *silo) {
        this->kapacita = new Store(KAPACITA_TRAKTORU);
        this->id = id;
        this->mlaticky = mlaticky;
        this->traktory = traktory;
        this->vzdalenost = vzdalenost;
        this->vykladka = vykladka;
        this->silo = silo;

        this->Activate();
    }

    void Behavior() {

        while(mlaticky->size() > 0) {
            // nalzeni nejplnejsi mlaticky

            while(!kapacita->Full() && mlaticky->size() != 0) {
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

                    // obsazeni mlaticky nakladakem -> pouze jeden nakladak muze mlaticku vyprazdnovat
                    nejplnejsi->jizVyklada = true;

                    cout << "Nakladak: " << id << " vyklada:" << nejplnejsi->id << " vyuziti nakladaku pred nalozenim: " << kapacita->Used()  <<endl;

                    while(!kapacita->Full() && !nejplnejsi->kapacita->Empty()) {
                        nejplnejsi->kapacita->Leave(1);
                        Enter(*kapacita, 1);

                        if(nejplnejsi->stop) {
                            nejplnejsi->Activate();
                        }

                        // vyprazdeni 100kg trva 0.02 minut
                        Wait(0.02);
                    }

                    nejplnejsi->jizVyklada = false;
                }

                Wait(0.01);
            }

            // transport plneho nakladaku, zaokrouhleno na desetiny minuty
            double dobaCesty = ((int)(Uniform(MIN_DOBA_CESTY, MAX_DOBA_CESTY) * 10 ))/ 10.0 ;

            cout << "nakladak odjizdi na " << dobaCesty << " minut" << endl;

            Wait(dobaCesty);

            double zacatekVykladani = Time;

            // pokus o zabrani vykladky
            vykladka->Enter(this, 1);

            while (!kapacita->Empty()) {
                Enter(*silo, 1);
                Leave(*kapacita, 1);

                // doba vykladky trva 0.75 minut
                Wait(0.75);
            }

            vykladka->Leave(1);

            cout << "nakladak " << id << " doba vykladani " << Time - zacatekVykladani << endl;

            // transport prazdnehom nakladaku, zaokrouhleno na desetiny minuty
            dobaCesty = ((int)(Uniform(MIN_DOBA_CESTY, MAX_DOBA_CESTY) * 10 ))/ 10.0 ;

            cout << "nakladak prijizdi za " << dobaCesty << " minut" << endl;

            Wait(dobaCesty);

        }

        traktory->remove(this);
        this->Terminate();
    }
};

int main(int argc, char **argv) {

    Init(0, 15000);
    SetStep(0.001);
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

    // vytvoreni mista pro vykladku
    Vykladka vykladka;
    Silo silo;

    // vytvareni mlaticek
    for(int x=0; x < pocetMlaticek ; x++) {
        mlaticky.push_back(new Mlaticka(x, &hektary, &mlaticky));
    }

    // vytvareni traktoru
    for(int x=0; x < pocetTraktoru ; x++) {
        traktory.push_back(new Traktor(x, &mlaticky, &traktory, vzdalenost, &vykladka, &silo));
    }

    Run();
}

