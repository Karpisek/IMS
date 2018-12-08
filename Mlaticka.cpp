//
// Created by Miro on 06/12/2018.
//

#include "Mlaticka.h"

int Mlaticka::celkovyUchovanyVynos = 0;

Mlaticka::Mlaticka(int id) {
    this->kapacita = new Kapacita(KAPACITA_MLATICKY);
    this->id = id;

    this->stop = false;
    this->zabrana = false;

    Mlaticka::vse.push_back(this);
    Activate();
}

void Mlaticka::Behavior() {
    PridejZaznamPrace(true);
    Transport(vzdalenost);

    while(!Hektar::vse.empty()) {
        Hektar *hektar = VybratHektar();
        PoznitHektar(hektar);
    }

    // mlaticka dokoncila praci na hektarech ale neni vyprazdnena -> musi cekat na traktor
    if(kapacita->Used() > 0) {
        Mlaticka::vse.remove(this);
        Traktor::PriradTraktor(this);

        cout << "--------------------------------------------------------" << endl;
        cout << "Time: " << Time << endl;
        cout << "Mlaticka (" << id << ") ceka na vyprazdneni, pred koncem [" << kapacita->Used() << "]" << endl;
        cout << "--------------------------------------------------------" << endl;
        cout << endl;
    }


    PridejZaznamPrace(true);
    PridejZaznamPrace(false);
    while(kapacita->Used() > 0) {
        this->stop = true;
        this->Passivate();
    }
    PridejZaznamPrace(false);
    PridejZaznamPrace(true);

    // presun do zavodu
    Transport(vzdalenost);

    PridejZaznamPrace(true);
    PridejZaznamPrace(false);
    PrintZaznamy();
    Terminate();
}

Hektar* Mlaticka::VybratHektar() {
    Hektar *hektar = Hektar::vse.front();
    Hektar::vse.pop_front();

    return hektar;
}

void Mlaticka::PoznitHektar(Hektar *hektar) {
    while(!hektar->Empty()) {
        //PridejZaznamPrace(true);
        hektar->Leave(1);           // vezme jeden sto-kilogram
        Wait(hektar->doba);         // sklizen jednoho sto-kilogramu
        kapacita->Enter(1);        // pridani do vlastni kapacity

        Mlaticka::celkovyUchovanyVynos++;
        Hektar::zbyvajiciVynos--;

        PridejZaznamKapacita();             //pridani zaznamu o zmene kapacity

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
            PridejZaznamPrace(true);
            PridejZaznamPrace(false);
            this->Passivate();
            PridejZaznamPrace(false);
            PridejZaznamPrace(true);
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

void Mlaticka::PridejZaznamKapacita() {
    cas.push_back(Time);
    naplneni.push_back(kapacita->Used());
}
void Mlaticka::PridejZaznamPrace(bool pracuji) {
    cas2.push_back(Time);
    prace.push_back(pracuji);
}

void Mlaticka::PrintZaznamy() {
    ofstream myfile;
    string jmeno = "mlaticka" + to_string(this->id) + ".dat";
    myfile.open(jmeno);
    for(unsigned int i = 0;this->cas.size() > i; i++){
        myfile << this->cas[i] << " " <<this->naplneni[i] << endl;
    }
    myfile.close();

    jmeno = "mlatickaPrace" + to_string(this->id) + ".dat";
    myfile.open(jmeno);
    for(unsigned int i = 0;this->cas2.size() > i; i++){
        myfile << this->cas2[i] << " " <<this->prace[i] << endl;
    }
    myfile.close();
}

void Mlaticka::Transport(double vzdalenost) {

    // transport mlaticky na danou vzdalenost
    double dobaCesty = vzdalenost/(double)MLATICKA_SILNICE_RYCHLOST * HODINY_NA_MIN;

    cout << "--------------------------------------------------------" << endl;
    cout << "Time: " << Time << endl;
    cout << "Mlaticka (" << id << ") odjizdi na " << dobaCesty << " minut" << endl;
    cout << "--------------------------------------------------------" << endl;
    cout << endl;

    Wait(dobaCesty);
}
