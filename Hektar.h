//
// Created by Miro on 06/12/2018.
//

#ifndef MLATICKA_HEKTAR_H
#define MLATICKA_HEKTAR_H

#include "simlib.h"
#include <list>

using namespace std;

// TODO mozno deditm se store -> output();
class Hektar: public Store {
public:
    static list<Hektar *> vse;

    int vynos;      //  v sto-kilogramech
    double doba;    //  doba sklizne sto-kilogramu v sekundach

    Hektar();
};

#endif //MLATICKA_HEKTAR_H
