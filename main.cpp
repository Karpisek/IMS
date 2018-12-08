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

// static listy jednotlivych stroju

list<Hektar *> Hektar::vse;
list<Mlaticka *> Mlaticka::vse;
list<Traktor *> Traktor::vse;
list<Mlaticka *> Traktor::pozadavky;

int vzdalenost;
double stredPole;

int main(int argc, char **argv) {

    (void) argc;

    if(argc < 6) {
        cout << "Struktura parametru: [rozloha pole (ha)] [vzdalenost stredu pole (m) ][pocet mlaticek] [pocet traktoru N] [vzdalenost zavodu (km)] N * [kapacita traktoru (t)]" << endl;
        exit(0);
    }

    list<Traktor *> listTraktoru;
    list<Mlaticka *> listMlaticek;

    Init(0, 15000);
    SetStep(0.001);
    //RandomSeed(time(nullptr));

    int rozloha = atoi(argv[1]);
    stredPole = (double)atoi(argv[2]) / METRY_NA_KM;   // prevod na kilometry
    int pocetMlaticek = atoi(argv[3]);
    int pocetTraktoru = atoi(argv[4]);
    vzdalenost = atoi(argv[5]);

    if(argc < pocetTraktoru + 6) {
        cout << "kapacity pro nakladaky nejsou urceny pro vsechny" << endl;
        exit(0);
    }

    vector<int> kapacity;
    for(int i = 0; i < pocetTraktoru; i++) {
        kapacity.push_back(atoi(argv[6 + i]));
    }

    for(int x = 0; x < rozloha; x++) {
        new Hektar();
    }

    cout << "Pocet hektaru: " << Hektar::vse.size() << endl;
    cout << "Vzdalenos do stredu pole: " << stredPole << endl;
    cout << "Pocet mlaticek: " << pocetMlaticek << endl;
    cout << "Pocet nakladaku: " << pocetTraktoru << endl;
    cout << "Vzdalenost zasobniku: " << vzdalenost << endl;

    for(int i = 0; i < pocetTraktoru; i++) {
        cout << "Kapacita nakladaku [" << i << "]:" << kapacity[i] << "t" << endl;
    }
    cout << "*******************************" << endl << endl;

    // vytvoreni mista pro vykladku
    Vykladka vykladka;

    // vytvareni mlaticek
    for(int x=0; x < pocetMlaticek ; x++) {
        new Mlaticka(x);
    }

    // vytvareni traktoru
    for(int x=0; x < pocetTraktoru ; x++) {
        new Traktor(x, &vykladka, kapacity[x]);

    }

    Run();
    vykladka.PrintZaznamy();
}

