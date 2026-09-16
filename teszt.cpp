#include <iostream>
#include "eszkoz.hpp"
#include "rendeles.hpp"
#include "rendszer.hpp"
#include "utils.hpp"
#include "memtrace.h"

// Segédfüggvény: tesztszakasz elválasztó
static void szakasz(const char* cim) {
    vonal('=', 52);
    std::cout << "  " << cim << std::endl;
    vonal('=', 52);
}

int main() {
    szakasz("1. Eszkoz es Kiegeszito alapmuveletek");

    // Konstruktor, getterek
    Eszkoz kamera("Sony A7IV", 1, 3);
    Eszkoz allvany("Manfrotto 055", 1, 5);
    Kiegeszito akku("NP-FZ100", 1, 8);
    Kiegeszito taska("Lowepro M", 1, 4);

    std::cout << "Kamera neve:  " << kamera.getNev() << std::endl;
    std::cout << "Kamera ara:   " << kamera.getAr() << " Ft" << std::endl;
    std::cout << "Kamera db:    " << kamera.getDb() << std::endl;
    std::cout << "Akku hasznalat: " << akku.getHasznalat() << std::endl;

    // novDb, csokkentDb, setDb, setHasznalat, novHasznalat
    kamera.csokkentDb(1);
    std::cout << "Kamera db -1: " << kamera.getDb() << std::endl;
    kamera.novDb(1);
    std::cout << "Kamera db +1: " << kamera.getDb() << std::endl;
    kamera.setDb(10);
    std::cout << "Kamera setDb(10): " << kamera.getDb() << std::endl;
    kamera.setDb(3); // visszaallitas
    akku.novHasznalat(2);
    std::cout << "Akku hasznalat +2: " << akku.getHasznalat() << std::endl;

    // kiir (ostream operator és közvetlen kiir())
    std::cout << "\nEszkoz operator<<:" << std::endl;
    std::cout << kamera;
    std::cout << "\nKiegeszito operator<<:" << std::endl;
    std::cout << akku;

    // csvSor
    char buf[256];
    kamera.csvSor(buf, 256);
    std::cout << "\nEszkoz csvSor: " << buf;
    akku.csvSor(buf, 256);
    std::cout << "Kiegeszito csvSor: " << buf;

    // Polimorfizmus: EszkozAlap*
    szakasz("2. Polimorfizmus (EszkozAlap*)");
    EszkozAlap* poli = &kamera;
    std::cout << "Polimorf Eszkoz kiir:" << std::endl;
    poli->kiir(std::cout);
    poli = &akku;
    std::cout << "Polimorf Kiegeszito kiir:" << std::endl;
    poli->kiir(std::cout);

    szakasz("3. Rendeles: konstruktor, addEszkoz, addKiegeszito, getterek");

    Rendeles rend(1, 7, true);
    rend.addEszkoz(&kamera);
    rend.addEszkoz(&allvany);
    rend.addKiegeszito(&akku);
    rend.addKiegeszito(&taska);

    std::cout << "Rendeles ID:         " << rend.getId()         << std::endl;
    std::cout << "Elettartam (nap):    " << rend.getElettartam() << std::endl;
    std::cout << "Aktiv:               " << (rend.isAktiv() ? "igen" : "nem") << std::endl;
    std::cout << "Eszkozok szama:      " << rend.getEszkozDb()   << std::endl;
    std::cout << "Kiegeszitok szama:   " << rend.getKiegDb()     << std::endl;
    std::cout << "Osszar:              " << rend.getOsszAr()     << " Ft" << std::endl;

    // getEszkoz / getKiegeszito
    std::cout << "1. eszkoz neve:      " << rend.getEszkoz(0)->getNev()      << std::endl;
    std::cout << "1. kiegeszito neve:  " << rend.getKiegeszito(0)->getNev()  << std::endl;

    // operator<< a Rendeles-re
    std::cout << "\nRendeles operator<<:" << std::endl;
    std::cout << rend;

    szakasz("4. Rendeles masolo konstruktor es ertekado operator");

    Rendeles masolt(rend);
    std::cout << "Masolt rendeles ID: " << masolt.getId()     << std::endl;
    std::cout << "Masolt osszar:      " << masolt.getOsszAr() << " Ft" << std::endl;

    Rendeles ujabb(2, 3, false);
    ujabb = rend;
    std::cout << "Ertekado utan ID:   " << ujabb.getId()      << std::endl;
    std::cout << "Ertekado osszar:    " << ujabb.getOsszAr()  << " Ft" << std::endl;

    szakasz("5. Rendeles lezarasa");

    std::cout << "Aktiv lezaras elott: " << (rend.isAktiv() ? "igen" : "nem") << std::endl;
    rend.lezar();
    std::cout << "Aktiv lezaras utan:  " << (rend.isAktiv() ? "igen" : "nem") << std::endl;

    szakasz("6. Utils: strEq, strCopy, vonal");

    char cel[64];
    strCopy(cel, "teszt string", 64);
    std::cout << "strCopy eredmeny: " << cel << std::endl;
    std::cout << "strEq(\"abc\",\"abc\"): " << (strEq("abc", "abc") ? "igaz" : "hamis") << std::endl;
    std::cout << "strEq(\"abc\",\"xyz\"): " << (strEq("abc", "xyz") ? "igaz" : "hamis") << std::endl;
    std::cout << "vonal('-', 30): ";
    vonal('-', 30);
    std::cout << std::endl;

    vonal('=', 52);
    std::cout << "  Teszt vege - minden vizsgalt fuggveny lefutott." << std::endl;
    vonal('=', 52);

    return 0;
}