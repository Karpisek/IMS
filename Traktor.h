//
// Created by Miro on 06/12/2018.
//

#ifndef MLATICKA_NAKLADAK_H
#define MLATICKA_NAKLADAK_H

#include "simlib.h"
#include "Mlaticka.h"
#include "macros.h"
#include <iostream>
#include <list>
#include "Vykladka.h"
#include "Silo.h"

using namespace std;

class Silo;
class Mlaticka;
class Vykladka;

class Traktor: public Process {
private:
    Vykladka *vykladka;
    Silo *silo;
    int vzdalenost;

public:
    static list<Traktor *> vse;
    Store *kapacita;     // vnitrni kapacita
    int id;

    Traktor(int id, int vzdalenost, Vykladka *vykladka, Silo *silo);
    Mlaticka* VybratMlaticku();
    void VylozMlaticku(Mlaticka *mlaticka);
    void NaplnTraktor();
    void Transport();
    void VyprazdniTraktor();
    void Behavior();
};


#endif //MLATICKA_NAKLADAK_H
