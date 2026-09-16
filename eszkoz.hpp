#ifndef ESZKOZ_HPP
#define ESZKOZ_HPP



#include <iostream>


class EszkozAlap {
protected:
    char nev[64];
    double ar;
    int db;
    int hasznalat;

public:
    EszkozAlap();
    EszkozAlap(const char* n, double a, int d, int h = 0);
    virtual ~EszkozAlap() {}

    const char* getNev() const;
    double getAr()  const;
    int getDb()  const;
    int getHasznalat() const;

    void novDb(int n = 1);
    void csokkentDb(int n = 1);
    void setDb(int d);

    void setHasznalat(int h);
    void novHasznalat(int n = 1);
    void csokHasznalat(int n = 1);

    virtual void kiir(std::ostream& os) const = 0;

    void kiir() const { kiir(std::cout); }

    virtual void csvSor(char* buf, int maxLen) const = 0;

    friend std::ostream& operator<<(std::ostream& os, const EszkozAlap& e);
};


class Eszkoz : public EszkozAlap {
public:
    Eszkoz();
    Eszkoz(const char* n, double a, int d, int h = 0);

    void kiir (std::ostream& os) const;          // override
    void csvSor (char* buf, int maxLen) const;     // override
};


class Kiegeszito : public EszkozAlap {
public:
    Kiegeszito();
    Kiegeszito(const char* n, double a, int d, int h = 0);

    void kiir (std::ostream& os) const;          // override
    void csvSor (char* buf, int maxLen) const;     // override
};

#endif // ESZKOZ_HPP
