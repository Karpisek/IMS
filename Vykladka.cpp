//
// Created by Miro on 06/12/2018.
//

#include "Vykladka.h"

Vykladka::Vykladka() {
    this->kapacita = new Kapacita(KAPACITA_VYKLADKY);
    this->mistaNaVykladku = new Kapacita(MIST_NA_VYLOZENI, MIST_NA_VYLOZENI);

    Activate();
}

void Vykladka::Behavior() {
    if(!kapacita->Empty()) {
        kapacita->Leave(1);
        PridejZaznamKapacita();
    }

    // v pripade, ze neni prazdna fronta a nejsou plna mista, tak kontrola jeslti se dalsi nakladak dokaze vyprazdnit
    if(!fronta.empty() && !mistaNaVykladku->Empty()) {
        if(fronta.front()->kapacita->Used() <= kapacita->Free()) {
            PridejZaznamKapacita();
            mistaNaVykladku->Leave(1);
            fronta.front()->Activate();
            fronta.pop_front();
        }
    }

    Activate(Time + 0.075);
}

void Vykladka::ZaberMisto(Traktor *traktor) {

    // pokud jsou vsechny mista zabrane
    if(mistaNaVykladku->Empty()) {
        fronta.push_back(traktor);
        traktor->Passivate();
    }

    // pokud neni dostatek kapacity na vykladku, jdi taky do fronty
    else if(traktor->kapacita->Used() > kapacita->Free()) {
        fronta.push_back(traktor);
        traktor->Passivate();
    }

    // jinak traktor kona svoji vykladaci cinost a obsad misto na vykladku
    else {
        mistaNaVykladku->Leave(1);
    }


}

void Vykladka::Uvolni() {

    // pokud je fronta na vykladku prazdna nebo neni dostatek kapacity na vylozeni, pridej misto
    if(fronta.empty() || fronta.front()->kapacita->Used() > kapacita->Free()) {
        this->mistaNaVykladku->Enter(1);
    }

    // jinak povolej prvniho na vykladku
    else {
        fronta.front()->Activate();
        fronta.pop_front();
    }
}

void Vykladka::PridejZaznamKapacita(){
    cas.push_back(Time);
    naplneni.push_back(kapacita->Used());
}


void Vykladka::PrintZaznamy() {
    ofstream myfile;
    string jmeno = "vykladka.dat";
    myfile.open(jmeno);
    for(unsigned int i = 0;this->cas.size() > i; i++){
        myfile << this->cas[i] << " " <<this->naplneni[i] << endl;
    }
    myfile.close();

}