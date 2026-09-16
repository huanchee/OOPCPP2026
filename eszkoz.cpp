
#include "eszkoz.hpp"
#include "utils.hpp"
#include <cstdio>   // snprintf
#include "memtrace.h"

EszkozAlap::EszkozAlap() : ar(0.0), db(0), hasznalat(0) {
    nev[0] = '\0';
}

EszkozAlap::EszkozAlap(const char* n, double a, int d, int h)
    : ar(a), db(d), hasznalat(h) {
    strCopy(nev, n, 64);
}

const char* EszkozAlap::getNev() const { return nev; }
double EszkozAlap::getAr() const { return ar;  }
int EszkozAlap::getDb() const { return db;  }
int EszkozAlap::getHasznalat() const { return hasznalat; }

void EszkozAlap::novDb (int n) { db += n; }
void EszkozAlap::csokkentDb (int n) { db -= n; }
void EszkozAlap::setDb (int d) { db  = d; }

void EszkozAlap::setHasznalat(int h) { hasznalat = h; }
void EszkozAlap::novHasznalat(int n) { hasznalat += n; }
void EszkozAlap::csokHasznalat(int n) { hasznalat -= n; }

std::ostream& operator<<(std::ostream& os, const EszkozAlap& e) {
    e.kiir(os);
    return os;
}

Eszkoz::Eszkoz() : EszkozAlap() {}

Eszkoz::Eszkoz(const char* n, double a, int d, int h) : EszkozAlap(n, a, d, h) {}

void Eszkoz::kiir(std::ostream& os) const {
    os << nev
       << "  " << db << " db {ossz}"
       << "  " << ar << " Ft (Hasznalatban: " << hasznalat << ")";
}

void Eszkoz::csvSor(char* buf, int maxLen) const {
    snprintf(buf, maxLen, "%s,%.2f,%d,%d", nev, ar, db, hasznalat);
}


Kiegeszito::Kiegeszito() : EszkozAlap() {}

Kiegeszito::Kiegeszito(const char* n, double a, int d, int h)
    : EszkozAlap(n, a, d, h) {}

void Kiegeszito::kiir(std::ostream& os) const {
    os << nev
       << "  " << db << " db {ossz}"
       << "  " << ar << " Ft"
       << "  [kiegeszito]" << " (Hasznalatban: " << hasznalat << ")";
}

void Kiegeszito::csvSor(char* buf, int maxLen) const {
    snprintf(buf, maxLen, "KIEG:%s,%.2f,%d,%d", nev, ar, db, hasznalat);
}
