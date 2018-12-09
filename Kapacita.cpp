#include "Kapacita.h"

Kapacita::Kapacita(int max, int init) {
    this->max = max;
    this->aktualni = init;
}

bool Kapacita::Empty() {
    return aktualni == 0;
}

bool Kapacita::Full() {
    return aktualni == max;
}

bool Kapacita::Enter(int i) {

    if(aktualni + i > max) {
        return false;
    }

    this->aktualni += i;
    return true;
}

bool Kapacita::Leave(int i) {
    if(aktualni - i < 0) {
        return false;
    }

    this->aktualni -= i;
    return true;
}

int Kapacita::Free() {
    return max - aktualni;
}

int Kapacita::Used() {
    return aktualni;
}

int Kapacita::Capacity() {
    return max;
}

Kapacita::Kapacita() {
    this->max = 0;
    this->aktualni = 0;
}

void Kapacita::SetCapacity(int max, int init) {
    this->max = max;
    this->aktualni = init;
}

