//
// Created by Miro on 06/12/2018.
//

#include "Hektar.h"

int Hektar::zbyvajiciVynos = 0;
list<Hektar *> Hektar::vse;

Hektar::Hektar() {
    auto vynos = (int)Uniform(32, 38);
    this->doba = ((int)((Uniform(12, 20)/vynos) * 10 ))/ 10.0 ;

    SetCapacity(vynos, vynos);     //nastaveni kapacity

    Hektar::zbyvajiciVynos += vynos;
    Hektar::vse.push_back(this);
}