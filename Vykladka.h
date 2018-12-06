//
// Created by Miro on 06/12/2018.
//

#ifndef MLATICKA_VYKLADKA_H
#define MLATICKA_VYKLADKA_H

#include "simlib.h"
#include "macros.h"

class Vykladka: public Store {
public:
    Queue queue;

    Vykladka();
    void Enter(Entity *e, unsigned long rcap);
    void Leave(unsigned long rcap);
};

#endif //MLATICKA_VYKLADKA_H
