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
#include <vector>

using namespace std;

class Traktor;

class Vykladka: public Event {
private:
    vector<double> cas;     //cas zmeni naplneni
    vector<int> naplneni;   //aktualni naplneni
public:
    Kapacita *kapacita;
    Kapacita *mistaNaVykladku;
    list<Traktor *> fronta;

    Vykladka();
    void Behavior() override;
    void ZaberMisto(Traktor *traktor);
    void Uvolni();
    void PridejZaznamKapacita();
    void PrintZaznamy();
};

#endif //MLATICKA_VYKLADKA_H
