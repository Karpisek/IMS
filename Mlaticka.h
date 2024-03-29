#ifndef MLATICKA_MLATICKA_H
#define MLATICKA_MLATICKA_H

#include <vector>
#include <string>
#include <fstream>
#include <list>
#include <iostream>

#include "simlib.h"
#include "macros.h"

#include "Hektar.h"
#include "Traktor.h"
#include "Kapacita.h"

using namespace std;

class Traktor;
class Hektar;

class Mlaticka: public Process {
    using Process::Activate;

private:
    bool zabrana;
    vector<double> cas;     //cas zmeni naplneni
    vector<int> naplneni;   //aktualni naplneni


    vector<double> cas2;     //cas zmeni naplneni
    vector<int> prace;   //aktualni prace



public:
    static list<Mlaticka *> vse;
    static int celkovyUchovanyVynos;

    bool stop;
    Kapacita *kapacita;     // vnitrni kapacita
    int id;

    explicit Mlaticka(int id);
    void Behavior() override;
    void Activate();

    Hektar* VybratHektar();
    void PoznitHektar(Hektar *hektar);
    void Zaber();
    void Uvolni();
    bool jeZabrana();
    void PridejZaznamKapacita();
    void PridejZaznamPrace(bool pracuji);
    void PrintZaznamy();
    void Transport(double vzdalenost);
};

#endif //MLATICKA_MLATICKA_H
