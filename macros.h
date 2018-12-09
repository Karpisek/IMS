// Pomocna makra pro vypocet,
// sekundy je treba zadavat v minutach tzn. 2s = 2/60 ~ 0.03 minut

#ifndef MLATICKA_MACROS_H
#define MLATICKA_MACROS_H

/*
 * VSTUPNI PARAMETRY
 */

#define ROZLOHA_POLE        20      // v hektarech
#define MAX_VZDALENOST_POLE 0.5     // vzdalenost do stredu pole v kilometrech (pouzito pro vypocet transportu stroju po poli)
#define POCET_MLATICEK      2       // pocet ucastnicich se mlaticek
#define POCET_TRAKTORU      3       // pocet traktoru
#define KAPACITA_TRAKTORU   8       // kapacita traktoru v tunach
#define KAPACITA_MLATICKY   7.5     // v tunach
#define VZDALENOST_VYKLADKY 15      // vzdalenost vykladkoveho mista od pole v kilometrech

/*
 * POKROCILA MAKRA -> netreba menit, ale lze
 */

#define MLATICKA_SILNICE_RYCHLOST   25      // prumerna rychlost mlaticky na silnici km/h (85% maximalni) [km/h]
#define TRAKTOR_SILNICE_RYCHLOST    34      // prumerna rychlost traktoru na silnici km/h (85% maximalni) [km/h]
#define TRAKTOR_POLE_RYCHLOST       8       // prumerna rychlost pohybu Traktoru po poli [km/h]
#define MINIMALNI_KAPACITA  0.5     // 0...1 minimalni kapacita pro vyprazdneni mlaticky

#define VYPRAZDNENI_STOKG   0.02    // doba vyprazdneni 100kg z mlaticky [min]
#define KAPACITA_VYKLADKY   40      // kapacita zasobniku vykladky [t]
#define MIST_NA_VYLOZENI    2       // pocet mist na vylozeni u vykladky
#define RYCHLOST_ZVEDANI_NAVESU     0.28    // [min]
#define RYCHLOST_ODBERU_VYKLADKY    0.075   // rychlost odebirani z mista pro vykladku (jak dlouho trva zpracovat 100kg) [min]
#define MIN_VZDALENOST_POLE 0     // min vzdalenost na poli v kilometrech (pouzito pro vypocet transportu stroju po poli) [km/h]

/*
 * POMOCNA MAKRA –> pouze prevody
 */

#define HODINY_NA_MIN       60      // pomocne makro pro prevod hodin na minuty
#define TUNA_NA_STOKG       10      // pomocne makro pro prevod tuny na sto-kilogramy











#endif //MLATICKA_MACROS_H
