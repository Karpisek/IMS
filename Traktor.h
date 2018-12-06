//
// Created by Miro on 06/12/2018.
//

#ifndef MLATICKA_NAKLADAK_H
#define MLATICKA_NAKLADAK_H

class Traktor: public Process {
private:
    list<Mlaticka *> *mlaticky;
    list<Traktor *> *traktory;

    Vykladka *vykladka;
    Silo *silo;

    int vzdalenost;

public:
    Store *kapacita;     // vnitrni kapacita
    int id;

    Traktor(int id, list<Mlaticka *> *mlaticky, list<Traktor *> *traktory, int vzdalenost, Vykladka *vykladka, Silo *silo);

    void Behavior();
};


#endif //MLATICKA_NAKLADAK_H
