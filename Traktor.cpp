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

    this->Activate();

    Traktor::vse.push_back(this);
}

void Traktor::NaplnTraktor() {
    while(!kapacita->Full() && Mlaticka::vse.size() != 0) {
        // dokud neni traktor plny

        // nalezeni nejplnejsi mlaticky co splnuje minimalni threshold
        Mlaticka *mlaticka = VybratMlaticku();
        VylozMlaticku(mlaticka);

        Wait(0.01);
    }
}

void Traktor::Behavior() {
    while(Mlaticka::vse.size() > 0) {

        // postupne plneni traktoru
        NaplnTraktor();

        // transport k silum
        Transport();

        // vyprazdneni
        VyprazdniTraktor();

        // transport zpet na pole
        Transport();
    }

    Traktor::vse.remove(this);
    this->Terminate();
}

Mlaticka* Traktor::VybratMlaticku() {
    Mlaticka *vybrana = nullptr;

    for (auto const& aktualni: Mlaticka::vse) {

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