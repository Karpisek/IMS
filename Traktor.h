//
// Created by Miro on 06/12/2018.
//

#ifndef MLATICKA_NAKLADAK_H
#define MLATICKA_NAKLADAK_H

#include "simlib.h"
#include "macros.h"
#include <iostream>
#include <list>
#include "Vykladka.h"
#include "Silo.h"
#include "Mlaticka.h"
#include <algorithm>

using namespace std;

class Silo;
class Mlaticka;
class Vykladka;

class Traktor: public Process {
private:
    Vykladka *vykladka;
    Silo *silo;
    int vzdalenost;

    Mlaticka *mlaticka;
    bool volny;

public:
    static list<Traktor *> vse;         // list vsech traktoru
    static list<Mlaticka *> pozadavky;  // list pozadavku

    Store *kapacita;     // vnitrni kapacita
    int id;

    Traktor(int id, int vzdalenost, Vykladka *vykladka, Silo *silo);
    Mlaticka* VybratMlaticku();
    int VylozMlaticku(Mlaticka *mlaticka);
    void NaplnTraktor();
    void Transport();
    void VyprazdniTraktor();
    void Behavior();

    void PriradMlaticku(Mlaticka *mlaticka);

    void Zaber();
    void Uvolni();
    bool jeVolny();

    static void PriradTraktor(Mlaticka *mlaticka);
    static void VytvorPozadavek(Mlaticka *mlaticka);
};


#endif //MLATICKA_NAKLADAK_H
