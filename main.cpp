//
// Created by Miro on 06/12/2018.
//

#include <iostream>
#include <list>
#include <cmath>
#include <algorithm>
#include <string>

#include "simlib.h"
#include "Traktor.h"
#include "Mlaticka.h"
#include "Hektar.h"
#include "macros.h"

using namespace std;

int main(int argc, char **argv) {

    (void) argc;
    (void) argv;

    list<Traktor *> listTraktoru;
    list<Mlaticka *> listMlaticek;

    Init(0, 15000);
    SetStep(0.001);
    RandomSeed(time(nullptr));

    cout << "Rozloha: " << ROZLOHA_POLE << "ha" << endl;
    cout << "Vzdalenos do stredu pole: " << MAX_VZDALENOST_POLE << "km" << endl;
    cout << "Pocet mlaticek: " << POCET_MLATICEK << endl;
    cout << "Pocet traktoru: " << POCET_TRAKTORU << endl;
    cout << "Vzdalenost zasobniku: " << VZDALENOST_VYKLADKY << "km" << endl;
    cout << "Kapacita nakladaku: " << KAPACITA_TRAKTORU << "t" << endl;

    for(int x = 0; x < ROZLOHA_POLE; x++) {
        new Hektar();
    }

    for(int i = 0; i < POCET_TRAKTORU; i++) {

    }
    cout << "*******************************" << endl << endl;

    // vytvoreni mista pro vykladku
    Vykladka vykladka;

    // vytvareni mlaticek
    for(int x=0; x < POCET_MLATICEK ; x++) {
        new Mlaticka(x);
    }

    // vytvareni traktoru
    for(int x=0; x < POCET_TRAKTORU ; x++) {
        new Traktor(x, &vykladka, KAPACITA_TRAKTORU);

    }

    Run();
    vykladka.PrintZaznamy();
}

