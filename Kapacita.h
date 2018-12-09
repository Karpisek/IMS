//
// Created by Miro on 07/12/2018.
//

#ifndef MLATICKA_KAPACITA_H
#define MLATICKA_KAPACITA_H

class Kapacita {
private:
    int max;
    int aktualni;

public:
    explicit Kapacita(int max, int init=0);
    Kapacita();
    bool Empty();
    bool Full();
    bool Enter(int i);
    bool Leave(int i);
    int Free();
    int Used();
    int Capacity();
    void SetCapacity(int max, int init=0);
};


#endif //MLATICKA_KAPACITA_H
