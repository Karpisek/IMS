//
// Created by Miro on 06/12/2018.
//

#include "Traktor.h"

int Traktor::teoratickaKapacita = 0;

Traktor::Traktor(int id, Vykladka *vykladka, int kapacita) {
    this->kapacita = new Kapacita(kapacita * 10);      // nastaveni kapacity pro sto-kilogramy
    this->id = id;
    this->vykladka = vykladka;

    this->volny = false;

    Traktor::teoratickaKapacita += this->kapacita->Capacity();

    Traktor::vse.push_back(this);
    Activate();
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
void Traktor::Behavior() {
    // dorazi na misto urceni a ceka na dalsi akci
    Transport(vzdalenost);
    Uvolni();

    while (true) {

        cout << "--------------------------------------------------------" << endl;
        cout << "Time: " << Time << endl;
        cout << "Traktor (" << id << ") vyklada Mlaticku (" << mlaticka->id << "):" << endl;
        cout << "pred:" << endl;
        cout << "\t Traktor [" << kapacita->Used() << "/" << kapacita->Capacity() << "]" << endl;
        cout << "\t Mlaticka [" << mlaticka->kapacita->Used() << "/" << mlaticka->kapacita->Capacity() << "]" << endl;
        cout << endl;

        VylozMlaticku(mlaticka);

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
            Transport(vzdalenost);
            VyprazdniTraktor();

            // kontrola jestli potencionalni kapacita nakladaku ktere jsou na poli + kteri jiz na pole jedou
            // staci na zbyvajici vynos na poli a v mlatickach
            // todo: otestovat jestli funguje jak ma, pripadne mu tam pridat chybu, aby nebyl v odhadovani kolik
            // todo: zbyva jeste na poli tak presny !
            if((Mlaticka::celkovyUchovanyVynos + Hektar::zbyvajiciVynos) < Traktor::teoratickaKapacita) {
                Traktor::vse.remove(this);

                PrintZaznamy();
                Terminate();
            }

            // v pripade, ze je na poli jeste dostatek vynosu, vraci se traktor zpet
            Traktor::teoratickaKapacita += KAPACITA_TRAKTORU;
            Transport(vzdalenost);
        }

        // pokud traktor neni plny, ale jiz nejsou zadne mlaticky v provozu, jedna se o konec smeny
        else if(Mlaticka::vse.empty()) {
            Transport(vzdalenost);
            VyprazdniTraktor();

            Traktor::vse.remove(this);

            PrintZaznamy();
            Terminate();
        }

        Uvolni();
    }
}

#pragma clang diagnostic pop

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
    }
    return vybrana;

}

void Traktor::VylozMlaticku(Mlaticka *mlaticka) {
    // obsazeni mlaticky nakladakem -> pouze jeden nakladak muze mlaticku vyprazdnovat
    mlaticka->Zaber();

    // pridej zaznam aktualniho stavu mlaticky a nakladaku
    mlaticka->PridejZaznamKapacita();
    PridejZaznam();

    // dokud neni nakladak plny nebo mlaticka prazdna provadej transfer
    while(!kapacita->Full() && !mlaticka->kapacita->Empty()) {

        // vyprazdeni 100kg trva 0.02 minut
        Wait(0.02);

        mlaticka->kapacita->Leave(1);
        mlaticka->PridejZaznamKapacita();
        kapacita->Enter(1);

        Traktor::teoratickaKapacita--;
        Mlaticka::celkovyUchovanyVynos--;

        PridejZaznam();

        if(mlaticka->stop) {
            mlaticka->Activate();
        }
    }

    // po dokonceni vykladky se mlaticka uvolni
    mlaticka->Uvolni();
}

void Traktor::Transport(double vzdalenost) {

    // transport nakladaku na zadanou vzdalenost
    double dobaCesty = vzdalenost/(double)TRAKTOR_SILNICE_RYCHLOST * HODINY_NA_MIN;

    cout << "--------------------------------------------------------" << endl;
    cout << "Time: " << Time << endl;
    cout << "Traktor (" << id << ") odjizdi na " << dobaCesty << " minut" << endl;
    cout << "--------------------------------------------------------" << endl;
    cout << endl;

    Wait(dobaCesty);
}

void Traktor::VyprazdniTraktor() {
    cout << "--------------------------------------------------------" << endl;
    cout << "Time: " << Time << endl;
    cout << "Traktor (" << id << ") dorazil k vykladce"<< endl;
    cout << "\tVykladka ma frontu o velikosti [" << vykladka->fronta.size() << "]" << endl;
    cout << "--------------------------------------------------------" << endl;
    cout << endl;

    // pokus o zabrani vykladky
    vykladka->ZaberMisto(this);

    cout << "--------------------------------------------------------" << endl;
    cout << "Time: " << Time << endl;
    cout << "Traktor (" << id << ") Zacina vykladku " << endl;
    cout << "--------------------------------------------------------" << endl;
    cout << endl;

    PridejZaznam();

    // zvednuti korby nakladaku trva 0.28 minuty
    Wait(0.28);

    // dokud neni nakladak prazdny, vykladej
    while (!kapacita->Empty()) {
        vykladka->kapacita->Enter(1);
        kapacita->Leave(1);

        PridejZaznam();

        // doba vykladky jednoho sto-kilogramu trva 0.075 minut
        Wait(0.075);
    }

    // vraceni korby nakladaku trva 0.28 minuty
    Wait(0.28);

    vykladka->Uvolni();
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
    if(!Traktor::pozadavky.empty()) {
        Mlaticka *mlaticka = VybratMlaticku();
        PriradMlaticku(mlaticka);

    }

    else if(Mlaticka::vse.empty()) {
        Transport(vzdalenost);

        Traktor::vse.remove(this);

        PrintZaznamy();
        Terminate();
    }

    else {
        volny = true;
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
            if(!zadatel->jeZabrana()) {
                cout << "--------------------------------------------------------" << endl;
                cout << "Time: " << Time << endl;
                cout << "Mlaticka (" << zadatel->id << ") zada pri kapacite [" << zadatel->kapacita->Used() << "]"
                     << endl;
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

void Traktor::PridejZaznamKapacita(){
    cas.push_back(Time);
    naplneni.push_back(kapacita->Used());
}

void Traktor::PridejZaznamPrace(bool pracuji) {
    cas2.push_back(Time);
    prace.push_back(pracuji);
}

void Traktor::PrintZaznamy() {
    ofstream myfile;
    string jmeno = "traktor" + to_string(this->id) + ".dat";
    myfile.open(jmeno);
    for(unsigned int i = 0;this->cas.size() > i; i++){
        myfile << this->cas[i] << " " <<this->naplneni[i] << endl;
    }
    myfile.close();


    jmeno = "traktorPrace" + to_string(this->id) + ".dat";
    myfile.open(jmeno);
    for(unsigned int i = 0;this->cas2.size() > i; i++){
        myfile << this->cas2[i] << " " <<this->prace[i] << endl;
    }
    myfile.close();
}
