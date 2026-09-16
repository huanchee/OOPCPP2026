#ifndef RENDELES_HPP
#define RENDELES_HPP

#include "eszkoz.hpp"

class Rendeles {
    int id;
    Eszkoz** eszkozok;
    int eszkozDb;
    Kiegeszito** kiegeszitok;
    int kiegDb;
    int elettartam;   // napokban
    bool aktiv;

    void masolat(const Rendeles& o);

public:
    Rendeles();
    Rendeles(int i, int elT, bool akt);
    Rendeles(const Rendeles& o);
    Rendeles& operator=(const Rendeles& o);
    ~Rendeles();

    int  getId() const;
    int  getElettartam() const;
    bool isAktiv() const;
    int  getEszkozDb() const;
    int  getKiegDb() const;

    Eszkoz* getEszkoz(int i) const;
    Kiegeszito* getKiegeszito(int i) const;

    double getOsszAr() const;

    void addEszkoz(Eszkoz* e);
    void addKiegeszito(Kiegeszito* k);
    void lezar();
    void kiir(std::ostream& os) const;
    void kiir() const { kiir(std::cout); }

    friend std::ostream& operator<<(std::ostream& os, const Rendeles& r);
};

#endif // RENDELES_HPP
