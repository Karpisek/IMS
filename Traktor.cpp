#include "Traktor.h"

int Traktor::teoratickaKapacita = 0;
list<Traktor *> Traktor::vse;
list<Mlaticka *> Traktor::pozadavky;

Traktor::Traktor(int id, Vykladka *vykladka, int kapacita) {
    this->kapacita = new Kapacita(kapacita * TUNA_NA_STOKG);      // nastaveni kapacity pro sto-kilogramy
    this->id = id;
    this->vykladka = vykladka;

    this->volny = false;

    Traktor::teoratickaKapacita += this->kapacita->Capacity();

    Traktor::vse.push_back(this);
    Activate();
}

void Traktor::Behavior() {
    PridejZaznamPrace(true);

    // dorazi na misto urceni a ceka na dalsi akci
    Transport(VZDALENOST_VYKLADKY, TRAKTOR_SILNICE_RYCHLOST);
    Uvolni();

    while (true) {
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

            // presun z pole na silnic
            Transport(Uniform(MIN_VZDALENOST_POLE, MAX_VZDALENOST_POLE), TRAKTOR_POLE_RYCHLOST);

            // presun po silnici k vykladacimmu mistu
            Transport(VZDALENOST_VYKLADKY, TRAKTOR_SILNICE_RYCHLOST);
            VyprazdniTraktor();

            // kontrola jestli potencionalni kapacita nakladaku ktere jsou na poli + kteri jiz na pole jedou
            // staci na zbyvajici vynos na poli a v mlatickach
            if((Mlaticka::celkovyUchovanyVynos + Hektar::zbyvajiciVynos) <= Traktor::teoratickaKapacita) {
                Traktor::vse.remove(this);

                PridejZaznamPrace(true);
                PridejZaznamPrace(false);
                PrintZaznamy();

                Terminate();
            }

            // v pripade, ze je na poli jeste dostatek vynosu, vraci se traktor zpet
            Traktor::teoratickaKapacita += kapacita->Capacity();
            Transport(VZDALENOST_VYKLADKY, TRAKTOR_SILNICE_RYCHLOST);
        }

        // pokud traktor neni plny, ale jiz nejsou zadne mlaticky v provozu, jedna se o konec smeny
        else if(Traktor::pozadavky.empty() && Mlaticka::vse.empty() && (Mlaticka::celkovyUchovanyVynos + Hektar::zbyvajiciVynos) <= Traktor::teoratickaKapacita - kapacita->Free()) {

            Traktor::teoratickaKapacita -= kapacita->Free();

            // presun z pole na silnic
            Transport(Uniform(MIN_VZDALENOST_POLE, MAX_VZDALENOST_POLE), TRAKTOR_POLE_RYCHLOST);

            // presun k vykladce
            Transport(VZDALENOST_VYKLADKY, TRAKTOR_SILNICE_RYCHLOST);

            VyprazdniTraktor();
            Traktor::vse.remove(this);

            PridejZaznamPrace(true);
            PridejZaznamPrace(false);
            PrintZaznamy();

            Terminate();
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
    }

    return vybrana;

}

void Traktor::VylozMlaticku(Mlaticka *mlaticka) {
    // presun nakladak k mlaticce
    Transport(Uniform(MIN_VZDALENOST_POLE, MAX_VZDALENOST_POLE), TRAKTOR_POLE_RYCHLOST);

    cout << "--------------------------------------------------------" << endl;
    cout << "Time: " << Time << endl;
    cout << "Traktor (" << id << ") vyklada Mlaticku (" << mlaticka->id << "):" << endl;
    cout << "pred:" << endl;
    cout << "\t Traktor [" << kapacita->Used() << "/" << kapacita->Capacity() << "]" << endl;
    cout << "\t Mlaticka [" << mlaticka->kapacita->Used() << "/" << mlaticka->kapacita->Capacity() << "]" << endl;
    cout << endl;

    // pridej zaznam aktualniho stavu mlaticky a nakladaku
    mlaticka->PridejZaznamKapacita();
    PridejZaznamKapacita();

    // dokud neni nakladak plny nebo mlaticka prazdna provadej transfer
    while(!kapacita->Full() && !mlaticka->kapacita->Empty()) {

        // vyprazdeni 100kg trva 0.02 minut
        Wait(VYPRAZDNENI_STOKG);

        mlaticka->kapacita->Leave(1);
        mlaticka->PridejZaznamKapacita();
        kapacita->Enter(1);

        Traktor::teoratickaKapacita--;
        Mlaticka::celkovyUchovanyVynos--;

        PridejZaznamKapacita();

        if(mlaticka->stop) {
            mlaticka->Activate();
        }
    }

    // po dokonceni vykladky se mlaticka uvolni
    mlaticka->Uvolni();
}

void Traktor::Transport(double vzdalenost, int rychlost) {

    // transport nakladaku na zadanou vzdalenost
    double dobaCesty = vzdalenost/(double)rychlost * HODINY_NA_MIN;

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

    PridejZaznamKapacita();

    // zvednuti korby nakladaku trva 0.28 minuty
    Wait(RYCHLOST_ZVEDANI_NAVESU);

    // dokud neni nakladak prazdny, vykladej (naklopi korbu a vse vypadne)
    if (!kapacita->Empty()) {
        vykladka->PridejZaznamKapacita();
        vykladka->kapacita->Enter(kapacita->Used());
        vykladka->PridejZaznamKapacita();
        kapacita->Leave(kapacita->Used());

        PridejZaznamKapacita();
    }
    // vraceni korby nakladaku trva 0.28 minuty
    Wait(RYCHLOST_ZVEDANI_NAVESU);

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

        // sice pozadavky existuji, ale jiz jsou obsluhovany -> konec smeny
        if(mlaticka == nullptr) {
            if((Mlaticka::celkovyUchovanyVynos + Hektar::zbyvajiciVynos) <= (Traktor::teoratickaKapacita - kapacita->Free())) {
                Traktor::teoratickaKapacita -= kapacita->Free();

                Transport(VZDALENOST_VYKLADKY, TRAKTOR_SILNICE_RYCHLOST);

                Traktor::vse.remove(this);

                PrintZaznamy();

                Terminate();
            }

            else {
                volny = true;
                PridejZaznamPrace(true);
                PridejZaznamPrace(false);
                Passivate();
                PridejZaznamPrace(false);
                PridejZaznamPrace(true);
            }
        }

        PriradMlaticku(mlaticka);
    }

    // v pripade, ze jiz nejsou dalsi mlaticky konec smeny
    else if(Mlaticka::vse.empty() && ((Mlaticka::celkovyUchovanyVynos + Hektar::zbyvajiciVynos) <= (Traktor::teoratickaKapacita - kapacita->Free()))) {
        Traktor::teoratickaKapacita -= kapacita->Free();

        Transport(VZDALENOST_VYKLADKY, TRAKTOR_SILNICE_RYCHLOST);

        Traktor::vse.remove(this);

        PrintZaznamy();

        Terminate();
    }

    else {
        volny = true;
        PridejZaznamPrace(true);
        PridejZaznamPrace(false);
        Passivate();
        PridejZaznamPrace(false);
        PridejZaznamPrace(true);
    }
}

void Traktor::PriradMlaticku(Mlaticka *mlaticka) {
    // obsazeni mlaticky nakladakem -> pouze jeden nakladak muze mlaticku vyprazdnovat
    mlaticka->Zaber();
    this->mlaticka = mlaticka;
}

void Traktor::PriradTraktor(Mlaticka* zadatel) {
    Traktor *vybran = nullptr;

    // v pripade ze mlaticka zada ale jiz je zabrana traktorem, nevytvori se pozadavek
    if(zadatel->jeZabrana()) {
        return;
    }

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
        else if(vybran->kapacita->Free() > aktualni->kapacita->Free()) {
            vybran = aktualni;
        }
    }

    // pokud je nejaky volny traktor, je zvolen nejplnejsi a prirazen dane malticce
    if(vybran != nullptr) {
        vybran->PriradMlaticku(zadatel);
        vybran->Zaber();
    }

    // pokud zadny traktor neni volny, je mlaticka pridana na seznam pozadavku, jakmile se nejaky traktor vrati, odbavi ji
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
