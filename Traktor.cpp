//
// Created by Miro on 06/12/2018.
//

#include "Traktor.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
Traktor::Traktor(int id, int vzdalenost, Vykladka *vykladka, Silo *silo) {
    this->kapacita = new Store(KAPACITA_TRAKTORU);
    this->id = id;
    this->vzdalenost = vzdalenost;
    this->vykladka = vykladka;
    this->silo = silo;
    this->volny = true;

    Traktor::vse.push_back(this);
}

void Traktor::Behavior() {
    while (true) {

        cout << "--------------------------------------------------------" << endl;
        cout << "Time: " << Time << endl;
        cout << "Traktor (" << id << ") vyklada Mlaticku (" << mlaticka->id << "):" << endl;
        cout << "pred:" << endl;
        cout << "\t Traktor [" << kapacita->Used() << "/" << kapacita->Capacity() << "]" << endl;
        cout << "\t Mlaticka [" << mlaticka->kapacita->Used() << "/" << mlaticka->kapacita->Capacity() << "]" << endl;
        cout << endl;

        int vylozeno = VylozMlaticku(mlaticka);


        cout << "--------------------------------------------------------" << endl;
        cout << "Time: " << Time << endl;
        cout << "Traktor (" << id << ") vyklada Mlaticku (" << mlaticka->id << "):" << endl;
        cout << "po:" << endl;
        cout << "\t Traktor [" << kapacita->Used() << "/" << kapacita->Capacity() << "]" << endl;
        cout << "\t Mlaticka [" << mlaticka->kapacita->Used() << "/" << mlaticka->kapacita->Capacity() << "]" << endl;
        cout << "--------------------------------------------------------" << endl;
        cout << endl;

        // pokud je traktor plny, odjizdim, jinak uvolnim
        if(kapacita->Full()) {
            Transport();
            VyprazdniTraktor();
            Transport();
        }

        Uvolni();
    }
}

Mlaticka* Traktor::VybratMlaticku() {
    Mlaticka *vybrana = nullptr;

    for (auto const& aktualni: Traktor::pozadavky) {

        // pokud je mlaticka jiz obsluhovana preskocime
        if(aktualni->jeZabrana()) {
            continue;
        }

        // je-li mlaticka plna, zvolime ji
        if(aktualni->kapacita->Full()) {
            vybrana = aktualni;
            break;
        }

        // pokud je doposud nevybrana, bereme vse
        if(vybrana == nullptr) {
            vybrana = aktualni;
        }

        // jinak bereme pouze pokud je vice zaplnena
        else if(vybrana->kapacita->Used() < aktualni->kapacita->Used()) {
            vybrana = aktualni;
        }
    }

    // pokud je mlaticka vybrana, odeber ji z pozadavku
    if(vybrana != nullptr) {
        Traktor::pozadavky.remove(vybrana);
        //vybrana->Zaber();
    }
    return vybrana;
}

int Traktor::VylozMlaticku(Mlaticka *mlaticka) {
    // obsazeni mlaticky nakladakem -> pouze jeden nakladak muze mlaticku vyprazdnovat
    mlaticka->Zaber();

    int pred = kapacita->Used();
    mlaticka->PridejZaznam();
    PridejZaznam();
    while(!kapacita->Full() && !mlaticka->kapacita->Empty()) {
        mlaticka->kapacita->Leave(1);
        mlaticka->PridejZaznam();
        Enter(*kapacita, 1);
        PridejZaznam();

        if(mlaticka->stop) {
            mlaticka->Activate();
        }

        // vyprazdeni 100kg trva 0.02 minut
        Wait(0.02);
    }

    mlaticka->Uvolni();

    return kapacita->Used() - pred;
}

void Traktor::Transport() {
    // transport plneho nakladaku, zaokrouhleno na desetiny minuty
    double dobaCesty = ((int)(Uniform(MIN_DOBA_CESTY, MAX_DOBA_CESTY) * 10 ))/ 10.0 ;

    cout << "--------------------------------------------------------" << endl;
    cout << "Time: " << Time << endl;
    cout << "Traktor (" << id << ") odjizdi na " << dobaCesty << " minut" << endl;
    cout << "--------------------------------------------------------" << endl;
    cout << endl;

    Wait(dobaCesty);
}

void Traktor::VyprazdniTraktor() {
    double zacatekVykladani = Time;

    cout << "--------------------------------------------------------" << endl;
    cout << "Time: " << Time << endl;
    cout << "Traktor (" << id << ") dorazil k vykladce"<< endl;
    cout << "\tVykladka ma frontu o velikosti [" << vykladka->queue.size() << "]" << endl;
    cout << "--------------------------------------------------------" << endl;
    cout << endl;

    // pokus o zabrani vykladky
    vykladka->Enter(this, 1);

    cout << "--------------------------------------------------------" << endl;
    cout << "Time: " << Time << endl;
    cout << "Traktor (" << id << ") Zacina vykladku " << endl;
    cout << "--------------------------------------------------------" << endl;
    cout << endl;

    PridejZaznam();
    while (!kapacita->Empty()) {
        Enter(*silo, 1);
        Leave(*kapacita, 1);
        PridejZaznam();

        // doba vykladky trva 0.75 minut
        Wait(0.75);
    }

    vykladka->Leave(1);

}

bool Traktor::jeVolny() {
    return this->volny;
}

void Traktor::Zaber() {
    this->volny = false;
    Activate();
}

void Traktor::Uvolni() {

    cout << "--------------------------------------------------------" << endl;
    cout << "Time: " << Time << endl;
    cout << "Traktor (" << id << ") je volny" << endl;
    cout << "--------------------------------------------------------" << endl;
    cout << endl;

    // pri uvolneni traktoru existuje min. jedna mlaticka ktera je mozna na vylozeni
    if(Traktor::pozadavky.size() > 0) {
        Mlaticka *mlaticka = VybratMlaticku();
        PriradMlaticku(mlaticka);

    } else {
        volny = true;
        PrintZaznamy();
        Passivate();
    }
}

void Traktor::PriradMlaticku(Mlaticka *mlaticka) {
    this->mlaticka = mlaticka;
}

void Traktor::PriradTraktor(Mlaticka* zadatel) {
    Traktor *vybran = nullptr;

    for (auto const& aktualni: Traktor::vse) {

        // pokud je traktor jiz zabran preskocime
        if(!aktualni->jeVolny()) {
            continue;
        }

        // pokud je doposud nevybrana, bereme vse
        if(vybran == nullptr) {
            vybran = aktualni;
        }

        // jinak bereme pouze pokud je vice zaplnena
        else if(vybran->kapacita->Used() < aktualni->kapacita->Used()) {
            vybran = aktualni;
        }
    }

    // pokud je nejaky volny traktor, je zvolen nejplnejsi a prirazen dane malticce
    if(vybran != nullptr) {

        vybran->PriradMlaticku(zadatel);
        vybran->Zaber();
    }

    // pokud zadny traktro neni volny, je mlaticka pridana na seznam pozadavku, jakmile se nejaky traktor vrati, odbavi ji
    else {
        bool zadano = (find(Traktor::pozadavky.begin(), Traktor::pozadavky.end(), zadatel) != Traktor::pozadavky.end());

        if(!zadano) {
            if(!zadatel->jeZabrana()){
                cout << "--------------------------------------------------------" << endl;
                cout << "Time: " << Time << endl;
                cout << "Mlaticka (" << zadatel->id << ") zada pri kapacite [" << zadatel->kapacita->Used() << "]"<< endl;
                cout << "--------------------------------------------------------" << endl;
                cout << endl;

                Traktor::VytvorPozadavek(zadatel);

            }
        }
    }
}

void Traktor::VytvorPozadavek(Mlaticka *mlaticka) {
    Traktor::pozadavky.push_back(mlaticka);
}

void Traktor::PridejZaznam() {
    cas.push_back(Time);
    naplneni.push_back(kapacita->Used());
}

void Traktor::PrintZaznamy() {
    ofstream myfile;
    string jmeno = "traktor" + to_string(this->id) + ".dat";
    myfile.open(jmeno);
    for(unsigned int i = 0;this->cas.size() > i; i++){
        myfile << this->cas[i] << " " <<this->naplneni[i] << endl;
    }
    myfile.close();
}
#pragma clang diagnostic pop