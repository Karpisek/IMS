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

    Traktor::list.push_back(this);
}

void Traktor::Behavior() {
    while(Mlaticka::list.size() > 0) {
        // nalzeni nejplnejsi mlaticky

        while(!kapacita->Full() && Mlaticka::list.size() != 0) {
            // dokud neni traktor plny

            Mlaticka *nejplnejsi = nullptr;

            for (auto const& mlaticka: Mlaticka::list) {
                if(mlaticka->kapacita->Full()) {
                    nejplnejsi = mlaticka;
                    break;
                }

                if(mlaticka->kapacita->Used() < mlaticka->kapacita->Capacity() * MINIMALNI_KAPACITA) {
                    continue;
                }

                if(nejplnejsi == nullptr) {
                    nejplnejsi = mlaticka;
                }

                else if(nejplnejsi->kapacita->Used() < mlaticka->kapacita->Used()) {
                    nejplnejsi = mlaticka;
                }
            }

            // vylozeni nejplnejsi mlaticky
            if(nejplnejsi != nullptr) {

                // obsazeni mlaticky nakladakem -> pouze jeden nakladak muze mlaticku vyprazdnovat
                nejplnejsi->jizVyklada = true;

                cout << "Nakladak: " << id << " vyklada:" << nejplnejsi->id << " vyuziti nakladaku pred nalozenim: " << kapacita->Used()  <<endl;

                while(!kapacita->Full() && !nejplnejsi->kapacita->Empty()) {
                    nejplnejsi->kapacita->Leave(1);
                    Enter(*kapacita, 1);

                    if(nejplnejsi->stop) {
                        nejplnejsi->Activate();
                    }

                    // vyprazdeni 100kg trva 0.02 minut
                    Wait(0.02);
                }

                nejplnejsi->jizVyklada = false;
            }

            Wait(0.01);
        }

        // transport plneho nakladaku, zaokrouhleno na desetiny minuty
        double dobaCesty = ((int)(Uniform(MIN_DOBA_CESTY, MAX_DOBA_CESTY) * 10 ))/ 10.0 ;

        cout << "nakladak odjizdi na " << dobaCesty << " minut" << endl;

        Wait(dobaCesty);

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

        // transport prazdnehom nakladaku, zaokrouhleno na desetiny minuty
        dobaCesty = ((int)(Uniform(MIN_DOBA_CESTY, MAX_DOBA_CESTY) * 10 ))/ 10.0 ;

        cout << "nakladak prijizdi za " << dobaCesty << " minut" << endl;

        Wait(dobaCesty);

    }

    Traktor::list.remove(this);
    this->Terminate();
}