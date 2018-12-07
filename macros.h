//
// Created by Miro on 06/12/2018.
//

#ifndef MLATICKA_MACROS_H
#define MLATICKA_MACROS_H

#define HODINY_NA_MIN 60

#define KAPACITA_TRAKTORU 120   // ve sto-kilogramech
#define KAPACITA_MLATICKY 80    // ve sto-kilogramech
#define MINIMALNI_KAPACITA 0.5  // 0...1 minimalni kapacita pro vyprazdneni

#define MIN_DOBA_CESTY 10      // minimalni doba cesty v minutach
#define MAX_DOBA_CESTY 20      // maximalni doba cesty //todo prevest na exp (vsechno je exp)

#define MIST_NA_VYLOZENI 2
#define SILO_CAPACTY 30000      // kapacita sila ve stokilogramech

#define MLATICKA_SILNICE_RYCHLOST   25  // prumerna rychlost mlaticky na silnici km/h

#define TRAKTOR_POLE_RYCHLOST           // pouzivame pri vypoctu pro pohyb mezi mlatickami km/h
#define TRAKTOR_SILNICE_RYCHLOST    35  // prumerna rychlost traktoru na silnici km/h

#define KAPACITA_VYKLADKY     400       // kapacita zasobniku vykladky ve stokilogramech
#define RYCHLOST_ODBERU_VYKLADKY        // rychlost odebirani z mista pro vykladku

#endif //MLATICKA_MACROS_H
