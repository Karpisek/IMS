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
    while(!Hektar::vse.empty()) {
        Hektar *hektar = VybratHektar();
        PoznitHektar(hektar);
    }

    // mlaticka dokoncila praci na hektarech ale neni vyprazdnena -> musi cekat na traktor
    if(kapacita->Used() > 0) {
        Traktor::PriradTraktor(this);

        cout << "--------------------------------------------------------" << endl;
        cout << "Time: " << Time << endl;
        cout << "Mlaticka (" << id << ") ceka na vyprazdneni, pred koncem [" << kapacita->Used() << "]" << endl;
        cout << "--------------------------------------------------------" << endl;
        cout << endl;
    }

    while(kapacita->Used() > 0) {
        this->stop = true;
        this->Passivate();
    }

    Mlaticka::vse.remove(this);
    this->PrintZaznamy();
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
        PridejZaznam();             //pridani zaznamu o zmene kapacity

        // pokud je kapacita nad threshold, volam traktor
        if(kapacita->Used() >= kapacita->Capacity() * MINIMALNI_KAPACITA) {
            Traktor::PriradTraktor(this);
        }

        if(kapacita->Full()) {

            cout << "--------------------------------------------------------" << endl;
            cout << "Time: " << Time << endl;
            cout << "Mlaticka (" << id << ") Je plna"<< endl;
            cout << "--------------------------------------------------------" << endl;
            cout << endl;

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

void Mlaticka::PridejZaznam() {
    cas.push_back(Time);
    naplneni.push_back(kapacita->Used());
}

void Mlaticka::PrintZaznamy() {
    ofstream myfile;
    string jmeno = "mlaticka" + to_string(this->id) + ".dat";
    myfile.open(jmeno);
    for(unsigned int i = 0;this->cas.size() > i; i++){
        myfile << this->cas[i] << " " <<this->naplneni[i] << endl;
    }
    myfile.close();
}