
#include "rendeles.hpp"
#include "utils.hpp"
#include "memtrace.h"

void Rendeles::masolat(const Rendeles& o) {
    id = o.id;
    eszkozDb = o.eszkozDb;
    kiegDb = o.kiegDb;
    elettartam = o.elettartam;
    aktiv = o.aktiv;

    eszkozok = new Eszkoz*[eszkozDb > 0 ? eszkozDb : 1];
    for (int i = 0; i < eszkozDb; ++i) eszkozok[i] = o.eszkozok[i];

    kiegeszitok = new Kiegeszito*[kiegDb > 0 ? kiegDb : 1];
    for (int i = 0; i < kiegDb; ++i) kiegeszitok[i] = o.kiegeszitok[i];
}

Rendeles::Rendeles()
    : id(0), eszkozok(new Eszkoz*[1]), eszkozDb(0),
      kiegeszitok(new Kiegeszito*[1]), kiegDb(0),
      elettartam(0), aktiv(false) {}

Rendeles::Rendeles(int i, int elT, bool akt)
    : id(i),
      eszkozok(new Eszkoz*[1]), eszkozDb(0),
      kiegeszitok(new Kiegeszito*[1]), kiegDb(0),
      elettartam(elT), aktiv(akt) {}

Rendeles::Rendeles(const Rendeles& o) {
    masolat(o);
}

Rendeles& Rendeles::operator=(const Rendeles& o) {
    if (this == &o) return *this;
    delete[] eszkozok;
    delete[] kiegeszitok;
    masolat(o);
    return *this;
}

Rendeles::~Rendeles() {
    delete[] eszkozok;
    delete[] kiegeszitok;
}


int Rendeles::getId() const { return id;}
int Rendeles::getElettartam() const { return elettartam; }
bool Rendeles::isAktiv() const { return aktiv; }
int Rendeles::getEszkozDb() const { return eszkozDb; }
int Rendeles::getKiegDb() const { return kiegDb; }

Eszkoz* Rendeles::getEszkoz(int i) const { return eszkozok[i];}
Kiegeszito* Rendeles::getKiegeszito(int i) const { return kiegeszitok[i];}

double Rendeles::getOsszAr() const {
    double ossz = 0.0;
    for (int i = 0; i < eszkozDb; ++i) ossz += eszkozok[i]->getAr();
    for (int i = 0; i < kiegDb; ++i) ossz += kiegeszitok[i]->getAr();
    return ossz * elettartam;
}


void Rendeles::addEszkoz(Eszkoz* e) {
    Eszkoz** tmp = new Eszkoz*[eszkozDb + 1];
    for (int i = 0; i < eszkozDb; ++i) tmp[i] = eszkozok[i];
    tmp[eszkozDb++] = e;
    delete[] eszkozok;
    eszkozok = tmp;
}

void Rendeles::addKiegeszito(Kiegeszito* k) {
    Kiegeszito** tmp = new Kiegeszito*[kiegDb + 1];
    for (int i = 0; i < kiegDb; ++i) tmp[i] = kiegeszitok[i];
    tmp[kiegDb++] = k;
    delete[] kiegeszitok;
    kiegeszitok = tmp;
}

void Rendeles::lezar() { aktiv = false; }


void Rendeles::kiir(std::ostream& os) const {
    os << "  Rendeles #" << id
       << "  [" << (aktiv ? "AKTIV " : "LEZART") << "]"
       << "  " << elettartam << " nap\n";

    os << "  Eszkozok:\n";
    for (int j = 0; j < eszkozDb; ++j) {
        os << "    - ";
        eszkozok[j]->kiir(os);
        os << '\n';
    }

    if (kiegDb > 0) {
        os << "  Kiegeszitok:\n";
        for (int j = 0; j < kiegDb; ++j) {
            os << "    - ";
            kiegeszitok[j]->kiir(os);
            os << '\n';
        }
    }

    os << "  Osszar: " << getOsszAr() << " Ft\n";
    vonal(os);
}

std::ostream& operator<<(std::ostream& os, const Rendeles& r) {
    r.kiir(os);
    return os;
}
