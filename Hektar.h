#ifndef MLATICKA_HEKTAR_H
#define MLATICKA_HEKTAR_H

#include "simlib.h"
#include <list>
#include "Kapacita.h"

using namespace std;

// TODO mozno deditm se store -> output();
class Hektar: public Kapacita {
public:
    static list<Hektar *> vse;
    static int zbyvajiciVynos;

    double doba;    //  doba sklizne sto-kilogramu v sekundach

    Hektar();
};

#endif //MLATICKA_HEKTAR_H
