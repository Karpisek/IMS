//
// Created by Miro on 06/12/2018.
//

#include "Mlaticka.h"

Mlaticka::Mlaticka(int id) {
    this->kapacita = new Store(KAPACITA_MLATICKY);
    this->id = id;

    this->stop = false;
    this->zabrana = false;

    this->Activate();

    Mlaticka::vse.push_back(this);
}

void Mlaticka::Behavior() {
    while(Hektar::vse.size() > 0) {
        Hektar *hektar = VybratHektar();
        PoznitHektar(hektar);
    }

    Mlaticka::vse.remove(this);
    this->Terminate();
}

Hektar* Mlaticka::VybratHektar() {
    Hektar *hektar = Hektar::vse.front();
    Hektar::vse.pop_front();

    return hektar;
}

void Mlaticka::PoznitHektar(Hektar *hektar) {
    while(!hektar->Empty()) {
        hektar->Leave(1);           // vezme jeden sto-kilogram
        Wait(hektar->doba);         // sklizen jednoho sto-kilogramu
        Enter(*kapacita, 1);        // pridani do vlastni kapacity

        // pokud je kapacita nad threshold, volam traktor
        if(kapacita->Used() > kapacita->Capacity() * MINIMALNI_KAPACITA) {
            Traktor::PriradTraktor(this);
        }

        if(kapacita->Full()) {
            cout << "Mlaticka " << id << " je plna v: " << Time << endl;
            this->stop = true;
            this->Passivate();
        }
    }
}

void Mlaticka::Activate() {
    this->stop = false;
    Process::Activate();
}

void Mlaticka::Zaber() {
    zabrana = true;
}

void Mlaticka::Uvolni() {
    zabrana = false;
}

bool Mlaticka::jeZabrana() {
    return zabrana;
}
