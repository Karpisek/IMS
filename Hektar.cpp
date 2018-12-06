//
// Created by Miro on 06/12/2018.
//

#include "Hektar.h"

Hektar::Hektar() {
    int vynos = (int)Uniform(43, 49);
    this->doba = ((int)((Uniform(12, 20)/vynos) * 10 ))/ 10.0 ;

    SetCapacity(vynos);     //nastaveni kapacity
    Enter(nullptr, vynos);  //naplneni kapacity

    Hektar::list.push_back(this);
}