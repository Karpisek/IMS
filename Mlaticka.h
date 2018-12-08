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
#include "Traktor.h"
#include <vector>
#include <string>
#include <fstream>
#include "Kapacita.h"

using namespace std;

class Traktor;
class Hektar;

extern int vzdalenost;
extern double stredPole;

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
