//
// Created by Miro on 06/12/2018.
//

#include <iostream>
#include <list>
#include <math.h>
#include <algorithm>
#include <string>

#include "simlib.h"
#include "Traktor.h"
#include "Mlaticka.h"
#include "Hektar.h"
#include "macros.h"

using namespace std;

// static listy jednotlivych stroju

list<Hektar *> Hektar::vse;
list<Mlaticka *> Mlaticka::vse;
list<Traktor *> Traktor::vse;
list<Mlaticka *> Traktor::pozadavky;

int main(int argc, char **argv) {

    Init(0, 15000);
    SetStep(0.001);
    RandomSeed(time(nullptr));

    int rozloha = atoi(argv[1]);
    int pocetMlaticek = atoi(argv[2]);
    int pocetTraktoru = atoi(argv[3]);
    int vzdalenost = atoi(argv[4]);

    for(int x = 0; x < rozloha; x++) {
        new Hektar();
    }

    cout << "Pocet hektaru: " << Hektar::vse.size() << endl;
    cout << "Pocet mlaticek: " << pocetMlaticek << endl;
    cout << "Pocet nakladaku: " << pocetTraktoru << endl;
    cout << "Vzdalenost zasobniku: " << vzdalenost << endl;
    cout << "*******************************" << endl << endl;

    // vytvoreni mista pro vykladku
    Vykladka vykladka;
    Silo silo;

    // vytvareni mlaticek
    for(int x=0; x < pocetMlaticek ; x++) {
        new Mlaticka(x);
    }

    // vytvareni traktoru
    for(int x=0; x < pocetTraktoru ; x++) {
        new Traktor(x, vzdalenost, &vykladka, &silo);
    }

    Run();
}

