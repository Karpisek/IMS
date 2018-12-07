//
// Created by Miro on 06/12/2018.
//

#include "Hektar.h"

Hektar::Hektar() {
    auto vynos = (int)Uniform(43, 49);
    this->doba = ((int)((Uniform(12, 20)/vynos) * 10 ))/ 10.0 ;

    SetCapacity(vynos, vynos);     //nastaveni kapacity

    Hektar::vse.push_back(this);
}