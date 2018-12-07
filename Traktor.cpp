//
// Created by Miro on 06/12/2018.
//

#include "Traktor.h"

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
        Zaber();

        VylozMlaticku(mlaticka);

        // pokud je traktor plny, odjizdim, jinak uvolnim
        if(kapacita->Full()) {
            Transport();
            VyprazdniTraktor();
            Transport();
        }

        Uvolni();
        Passivate();
    }
}

Mlaticka* Traktor::VybratMlaticku() {
    Mlaticka *vybrana = nullptr;

    for (auto const& aktualni: pozadavky) {

        // pokud je mlaticka jiz obsluhovana preskocime
        if(aktualni->jeZabrana()) {
            continue;
        }

        // je-li mlaticka plna, zvolime ji
        if(aktualni->kapacita->Full()) {
            vybrana = aktualni;
            break;
        }

        // pokud je uroven zaplneni mlaticky nizsi nez threshold pokracujeme
        if(aktualni->kapacita->Used() < aktualni->kapacita->Capacity() * MINIMALNI_KAPACITA) {
            continue;
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

    return vybrana;
}

void Traktor::VylozMlaticku(Mlaticka *mlaticka) {
    // vylozeni nejplnejsi mlaticky
    if(mlaticka != nullptr) {

        // obsazeni mlaticky nakladakem -> pouze jeden nakladak muze mlaticku vyprazdnovat
        mlaticka->Zaber();

        cout << "Nakladak: " << id << " vyklada:" << mlaticka->id << " vyuziti nakladaku pred nalozenim: " << kapacita->Used()  <<endl;

        while(!kapacita->Full() && !mlaticka->kapacita->Empty()) {
            mlaticka->kapacita->Leave(1);
            Enter(*kapacita, 1);

            if(mlaticka->stop) {
                mlaticka->Activate();
            }

            // vyprazdeni 100kg trva 0.02 minut
            Wait(0.02);
        }

        mlaticka->Uvolni();
    }
}

void Traktor::Transport() {
    // transport plneho nakladaku, zaokrouhleno na desetiny minuty
    double dobaCesty = ((int)(Uniform(MIN_DOBA_CESTY, MAX_DOBA_CESTY) * 10 ))/ 10.0 ;

    cout << "nakladak odjizdi na " << dobaCesty << " minut" << endl;

    Wait(dobaCesty);

}

void Traktor::VyprazdniTraktor() {
    double zacatekVykladani = Time;

    // pokus o zabrani vykladky
    vykladka->Enter(this, 1);

    while (!kapacita->Empty()) {
        Enter(*silo, 1);
        Leave(*kapacita, 1);

        // doba vykladky trva 0.75 minut
        Wait(0.75);
    }

    vykladka->Leave(1);

    cout << "nakladak " << id << " doba vykladani " << Time - zacatekVykladani << endl;
}

bool Traktor::jeVolny() {
    return this->volny;
}

void Traktor::Zaber() {
    this->volny = false;
}

void Traktor::Uvolni() {

    // pri uvolneni traktoru existuje min. jedna mlaticka ktera je mozna na vylozeni
    if(Traktor::pozadavky.size() > 0) {
        Mlaticka *mlaticka = VybratMlaticku();
        PriradMlaticku(mlaticka);
        Activate();
    }

    this->volny = true;
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
        vybran->Activate();
    }

    // pokud zadny traktro neni volny, je mlaticka pridana na seznam pozadavku, jakmile se nejaky traktor vrati, odbavi ji
    else {
        Traktor::VytvorPozadavek(zadatel);
    }

}

void Traktor::VytvorPozadavek(Mlaticka *mlaticka) {
    Traktor::pozadavky.push_back(mlaticka);
}