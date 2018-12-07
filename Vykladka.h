//
// Created by Miro on 06/12/2018.
//

#ifndef MLATICKA_VYKLADKA_H
#define MLATICKA_VYKLADKA_H

#include "simlib.h"
#include "macros.h"
#include <list>
#include "Traktor.h"
#include "Kapacita.h"

using namespace std;

class Traktor;

class Vykladka: public Event {
public:
    Kapacita *kapacita;
    Kapacita *mistaNaVykladku;
    list<Traktor *> fronta;

    Vykladka();
    void Behavior() override;
    void ZaberMisto(Traktor *traktor);
    void Uvolni();
};

#endif //MLATICKA_VYKLADKA_H
