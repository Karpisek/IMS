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
#include "Mlaticka.h"
#include <algorithm>
#include <vector>
#include "Kapacita.h"

using namespace std;

class Mlaticka;
class Vykladka;

class Traktor: public Process {
private:
    Vykladka *vykladka;

    Mlaticka *mlaticka;
    bool volny;


    vector<double> cas;
    vector<int> naplneni;
    vector<double> cas2;     //cas zmeni naplneni
    vector<int> prace;   //aktualni prace

public:

    static list<Traktor *> vse;         // list vsech traktoru
    static list<Mlaticka *> pozadavky;  // list pozadavku
    static int teoratickaKapacita;

    Kapacita *kapacita;     // vnitrni kapacita
    int id;

    Traktor(int id, Vykladka *vykladka, int kapacita);
    Mlaticka* VybratMlaticku();
    void VylozMlaticku(Mlaticka *mlaticka);
    void Transport(double vzdalenost, int rychlost);
    void VyprazdniTraktor();
    void Behavior() override;

    void PriradMlaticku(Mlaticka *mlaticka);

    void Zaber();
    void Uvolni();
    bool jeVolny();
    void PridejZaznamKapacita();
    void PridejZaznamPrace(bool pracuji);
    void PrintZaznamy();

    static void PriradTraktor(Mlaticka *mlaticka);
    static void VytvorPozadavek(Mlaticka *mlaticka);
};


#endif //MLATICKA_NAKLADAK_H
