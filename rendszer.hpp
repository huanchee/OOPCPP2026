#ifndef RENDSZER_HPP
#define RENDSZER_HPP


#include "rendeles.hpp"

class Rendszer {
    Eszkoz** eszkozLista;
    int eszkozDb;
    Kiegeszito** kiegLista;
    int kiegDb;
    Rendeles* rendelesek;
    int rendelesDb;
    int kovetkezoId;

    static const char CSV_FAJL[];
    static const char TXT_FAJL[];

    void eszkozHozzaListahoz(const char* nev, double ar, int db, int h);
    void kiegHozzaListahoz  (const char* nev, double ar, int db);

    void csvBetolt();
    void txtBetolt();
    void csvMent() const;
    void txtMent() const;

public:
    Rendszer();
    ~Rendszer();

    Rendszer(const Rendszer&) = delete;
    Rendszer& operator=(const Rendszer&) = delete;

    void ment() const;

    // ── menupontok ──
    void eszkozHozzaad();
    void eszkozTorol();
    void elerheto_lista() const;
    void ujRendeles();
    void rendelesekLeker() const;
    void rendelesTorol();
    void statisztika() const;
};

#endif // RENDSZER_HPP
