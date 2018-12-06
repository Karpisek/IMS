//
// Created by Miro on 06/12/2018.
//

#ifndef MLATICKA_MLATICKA_H
#define MLATICKA_MLATICKA_H

#include "simlib.h"
#include <list>
#include "macros.h"
#include <iostream>
#include "Hektar.h"

using namespace std;

class Traktor;
class Hektar;

class Mlaticka: public Process {
public:
    static list<Mlaticka *> list;

    bool jizVyklada;
    bool stop;
    Store *kapacita;     // vnitrni kapacita
    int id;

    Mlaticka(int id);
    void Behavior();
    void Activate();

    Hektar* VybratHektar();
    void PoznitHektar(Hektar *hektar);
};

#endif //MLATICKA_MLATICKA_H
