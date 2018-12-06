//
// Created by Miro on 06/12/2018.
//

#include "Vykladka.h"

Vykladka::Vykladka() {
    SetCapacity(MIST_NA_VYLOZENI);
}

void Vykladka::Enter(Entity *e, unsigned long rcap) {
    if(Free() > 0) {
        Store::Enter(e, rcap);
    } else {
        queue.InsLast(e);
        e->Passivate();
    }
}

void Vykladka::Leave(unsigned long rcap) {
    if(queue.size() == 0) {
        Store::Leave(rcap);
    }
    else {
        queue.GetFirst()->Activate();
    }

}