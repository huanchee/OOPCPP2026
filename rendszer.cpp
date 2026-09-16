#include "rendszer.hpp"
#include "utils.hpp"
#include <fstream>
#include <cstdlib>   // atoi, atof
#include <cstring>   // strncmp
#include "memtrace.h"




const char Rendszer::CSV_FAJL[] = "eszkozok.csv";
const char Rendszer::TXT_FAJL[] = "rendelesek.txt";




void Rendszer::eszkozHozzaListahoz(const char* nev, double ar, int db, int h) {
    Eszkoz* uj = new Eszkoz(nev, ar, db, h);


    Eszkoz** tmp = new Eszkoz*[eszkozDb + 1];
    for (int i = 0; i < eszkozDb; ++i) tmp[i] = eszkozLista[i];
    tmp[eszkozDb++] = uj;
    delete[] eszkozLista;
    eszkozLista = tmp;
}


void Rendszer::kiegHozzaListahoz(const char* nev, double ar, int db) {
    Kiegeszito* uj = new Kiegeszito(nev, ar, db);


    Kiegeszito** tmp = new Kiegeszito*[kiegDb + 1];
    for (int i = 0; i < kiegDb; ++i) tmp[i] = kiegLista[i];
    tmp[kiegDb++] = uj;
    delete[] kiegLista;
    kiegLista = tmp;
}



void Rendszer::csvBetolt() {
    std::ifstream f(CSV_FAJL);
    if (!f.is_open()) return;
    char sor[256];
    while (f.getline(sor, 256)) {
        if (sor[0] == '\0') continue;


        bool kieg = (sor[0]=='K' && sor[1]=='I' && sor[2]=='E' && sor[3]=='G' && sor[4]==':');
        char* ptr = kieg ? sor + 5 : sor;


        char nev[64] = {};
        int  ni = 0;
        while (*ptr && *ptr != ',' && ni < 63) nev[ni++] = *ptr++;
        if (*ptr == ',') ++ptr;


        double ar = atof(ptr);
        while (*ptr && *ptr != ',') ++ptr;
        if (*ptr == ',') ++ptr;


        int db = atoi(ptr);
        while (*ptr && *ptr != ',') ++ptr;
        if (*ptr == ',') ++ptr;


        int h = atoi(ptr);


        if (kieg) {
            kiegHozzaListahoz(nev, ar, db);
            // A CSV 4. mezojebol a hasznalat beallitasa
            // (a hozzaadas ota a Kiegeszito is tarolja a hasznalatot)
            if (h != 0) kiegLista[kiegDb - 1]->setHasznalat(h);
        }
        else eszkozHozzaListahoz(nev, ar, db, h);
    }
}


// TXT formatum – blokkok:
//   RENDELES:<id>
//   ELETTARTAM:<nap>
//   AKTIV:<0|1>
//   ESZKOZ:<nev>*
//   KIEG:<nev>*
//   END
void Rendszer::txtBetolt() {
    std::ifstream f(TXT_FAJL);
    if (!f.is_open()) return;
    char sor[256];


    while (f.getline(sor, 256)) {
        if (strncmp(sor, "RENDELES:", 9) != 0) continue;
        int id = atoi(sor + 9);
        if (id >= kovetkezoId) kovetkezoId = id + 1;


        int  elT = 0;
        bool akt = false;
        Eszkoz* tmpE[64]; int tmpEDb = 0;
        Kiegeszito* tmpK[64]; int tmpKDb = 0;


        while (f.getline(sor, 256)) {
            if (strncmp(sor, "END", 3) == 0) break;
            if (strncmp(sor, "ELETTARTAM:", 11) == 0) elT = atoi(sor + 11);
            else if (strncmp(sor, "AKTIV:", 6) == 0) akt = (atoi(sor + 6) == 1);
            else if (strncmp(sor, "ESZKOZ:", 7) == 0) {
                for (int i = 0; i < eszkozDb; ++i)
                    if (strEq(eszkozLista[i]->getNev(), sor + 7)) {
                        if (tmpEDb < 64) tmpE[tmpEDb++] = eszkozLista[i];
                        break;
                    }
            } else if (strncmp(sor, "KIEG:", 5) == 0) {
                for (int i = 0; i < kiegDb; ++i)
                    if (strEq(kiegLista[i]->getNev(), sor + 5)) {
                        if (tmpKDb < 64) tmpK[tmpKDb++] = kiegLista[i];
                        break;
                    }
            }
        }


        Rendeles* tmp = new Rendeles[rendelesDb + 1];
        for (int i = 0; i < rendelesDb; ++i) tmp[i] = rendelesek[i];
        tmp[rendelesDb] = Rendeles(id, elT, akt);
        for (int i = 0; i < tmpEDb; ++i) tmp[rendelesDb].addEszkoz(tmpE[i]);
        for (int i = 0; i < tmpKDb; ++i) tmp[rendelesDb].addKiegeszito(tmpK[i]);
        delete[] rendelesek;
        rendelesek = tmp;
        ++rendelesDb;
    }
}


// CSV-mentes
void Rendszer::csvMent() const {
    std::ofstream f(CSV_FAJL);
    char buf[256];
    for (int i = 0; i < eszkozDb; ++i) {
        eszkozLista[i]->csvSor(buf, 256);
        f << buf << '\n';
    }
    for (int i = 0; i < kiegDb; ++i) {
        kiegLista[i]->csvSor(buf, 256);
        f << buf << '\n';
    }
}


void Rendszer::txtMent() const {
    std::ofstream f(TXT_FAJL);
    for (int i = 0; i < rendelesDb; ++i) {
        const Rendeles& r = rendelesek[i];
        f << "RENDELES:"   << r.getId()              << '\n'
          << "ELETTARTAM:" << r.getElettartam()      << '\n'
          << "AKTIV:"      << (r.isAktiv() ? 1 : 0)  << '\n';
        for (int j = 0; j < r.getEszkozDb(); ++j)
            f << "ESZKOZ:" << r.getEszkoz(j)->getNev() << '\n';
        for (int j = 0; j < r.getKiegDb(); ++j)
            f << "KIEG:"   << r.getKiegeszito(j)->getNev() << '\n';
        f << "END\n";
    }
}




Rendszer::Rendszer()
    : eszkozLista(new Eszkoz*[1]),     eszkozDb(0),
      kiegLista (new Kiegeszito*[1]), kiegDb(0),
      rendelesek(new Rendeles[1]),     rendelesDb(0),
      kovetkezoId(1) {
    csvBetolt();
    txtBetolt();
}


Rendszer::~Rendszer() {


    for (int i = 0; i < eszkozDb; ++i) delete eszkozLista[i];
    delete[] eszkozLista;


    for (int i = 0; i < kiegDb; ++i) delete kiegLista[i];
    delete[] kiegLista;


    delete[] rendelesek;
}


void Rendszer::ment() const { csvMent(); txtMent(); }




void Rendszer::eszkozHozzaad() {
    vonal('=');
    std::cout << "  ESZKOZ HOZZAADASA\n";
    vonal('=');


    std::cout << "Tipus: [1] Eszkoz  [2] Kiegeszito\n> ";
    bool kiegE = (readInt() == 2);


    char nev[64];
    std::cout << "Nev: ";        readLine(nev, 64);
    std::cout << "Ar (Ft): ";    double ar = readDouble();
    std::cout << "Darabszam: ";  int    db = readInt();


    if (db <= 0) {
        std::cout << "\n  Ervenytelen darabszam.\n";
        return;
    }


    if (!kiegE) {
        for (int i = 0; i < eszkozDb; ++i) {
            if (strEq(eszkozLista[i]->getNev(), nev)) {
                eszkozLista[i]->novDb(db);
                std::cout << "\n  Meglevo eszkoz frissitve: " << nev
                          << " (+" << db << " db)\n";
                ment();
                return;
            }
        }
        eszkozHozzaListahoz(nev, ar, db, 0);
    } else {
        for (int i = 0; i < kiegDb; ++i) {
            if (strEq(kiegLista[i]->getNev(), nev)) {
                kiegLista[i]->novDb(db);
                std::cout << "\n  Meglevo kiegeszito frissitve: " << nev
                          << " (+" << db << " db)\n";
                ment();
                return;
            }
        }
        kiegHozzaListahoz(nev, ar, db);
    }


    std::cout << "\n  Sikeresen hozzaadva: " << nev << ", " << db << " db\n";
    ment();
}



void Rendszer::eszkozTorol() {
    vonal('=');
    std::cout << "  ESZKOZ TORLESE\n (0 ha eszkoz, 1 ha kiegeszito)\n";
    vonal('=');

    int kiegvagydb = readInt(); //0 ha eszkoz, 1 ha kieg

    if (kiegvagydb == 0) {


        if (eszkozDb == 0) {
            std::cout << "  Nincs eszkoz.\n";
            return;
        }


        for (int i = 0; i < eszkozDb; ++i) {
            std::cout << "  [" << i+1 << "] " << *eszkozLista[i] << '\n';
        }
        vonal();


        std::cout << "Index (1-" << eszkozDb << "): ";
        int idx = readInt() - 1;
        if (idx < 0 || idx >= eszkozDb) {
            std::cout << "  Ervenytelen index.\n";
            return;
        }


        std::cout << "Torlendo db: ";
        int db = readInt();
        if (db <= 0) {
            std::cout << "  Ervenytelen mennyiseg.\n";
            return;
        }
        if (eszkozLista[idx]->getDb() < db) {
            std::cout << "  Nincs elegendo szabad eszkoz ("
                    << eszkozLista[idx]->getDb() << " db elerheto).\n";
            return;
        }


        eszkozLista[idx]->csokkentDb(db);


        if (eszkozLista[idx]->getDb() == 0) {
            delete eszkozLista[idx];
            Eszkoz** tmp = new Eszkoz*[eszkozDb > 1 ? eszkozDb - 1 : 1];
            for (int i = 0, j = 0; i < eszkozDb; ++i)
                if (i != idx) tmp[j++] = eszkozLista[i];
            delete[] eszkozLista;
            eszkozLista = tmp;
            --eszkozDb;
            std::cout << "  Eszkoz teljesen eltavolitva.\n";
        } else {
            std::cout << "  Torles sikeres. Maradek: "
                    << eszkozLista[idx]->getDb() << " db\n";
        }
    } else {

        if (kiegDb == 0) {
            std::cout << "  Nincs kiegeszito.\n";
            return;
        }


        for (int i = 0; i < kiegDb; ++i) {
            std::cout << "  [" << i+1 << "] " << *kiegLista[i] << '\n';
        }
        vonal();


        std::cout << "Index (1-" << kiegDb << "): ";
        int idx = readInt() - 1;
        if (idx < 0 || idx >= kiegDb) {
            std::cout << "  Ervenytelen index.\n";
            return;
        }


        std::cout << "Torlendo db: ";
        int db = readInt();
        if (db <= 0) {
            std::cout << "  Ervenytelen mennyiseg.\n";
            return;
        }
        if (kiegLista[idx]->getDb() < db) {
            std::cout << "  Nincs elegendo szabad eszkoz ("
                    << kiegLista[idx]->getDb() << " db elerheto).\n";
            return;
        }


        kiegLista[idx]->csokkentDb(db);


        if (kiegLista[idx]->getDb() == 0) {
            delete kiegLista[idx];
            Kiegeszito** tmp = new Kiegeszito*[kiegDb > 1 ? kiegDb - 1 : 1];
            for (int i = 0, j = 0; i < kiegDb; ++i)
                if (i != idx) tmp[j++] = kiegLista[i];
            delete[] kiegLista;
            kiegLista = tmp;
            --kiegDb;
            std::cout << "  Kiegeszito teljesen eltavolitva.\n";
        } else {
            std::cout << "  Torles sikeres. Maradek: "
                    << kiegLista[idx]->getDb() << " db\n";
        }

    }

    ment();
}


void Rendszer::elerheto_lista() const {
    vonal('=');
    std::cout << "  ELERHETO ESZKOZOK (NEM KIADVA)\n";
    vonal('=');


    bool volt = false;
    for (int i = 0; i < eszkozDb; ++i) {
        if (eszkozLista[i]->getDb() - eszkozLista[i]->getHasznalat() > 0) {
            std::cout << "  [" << i+1 << "] " << *eszkozLista[i] << '\n';
            volt = true;
        }
    }
    if (!volt) std::cout << "  Nincs szabad eszkoz.\n";


    if (kiegDb > 0) {
        vonal();
        std::cout << "  Kiegeszitok:\n";
        for (int i = 0; i < kiegDb; ++i) {
            if (kiegLista[i]->getDb() - kiegLista[i]->getHasznalat() > 0) {
                std::cout << "  [K" << i+1 << "] " << *kiegLista[i] << '\n';
            }
        }
    }
    vonal();
}



void Rendszer::ujRendeles() {
    vonal('=');
    std::cout << "  UJ RENDELES OSSZEALLITASA\n";
    vonal('=');


    Eszkoz* kivE[32]; int kivEDb = 0;
    Kiegeszito* kivK[32]; int kivKDb = 0;


    bool vanSzabad = false;
    for (int i = 0; i < eszkozDb; ++i)
        if (eszkozLista[i]->getDb() - eszkozLista[i]->getHasznalat() > 0) { vanSzabad = true; break; }
    if (!vanSzabad) {
        std::cout << "  Nincs szabad eszkoz.\n";
        return;
    }


    std::cout << "  Elerheto eszkozok:\n";
    for (int i = 0; i < eszkozDb; ++i) {
        if (eszkozLista[i]->getDb() - eszkozLista[i]->getHasznalat() > 0) {
            std::cout << "  [" << i+1 << "] " << *eszkozLista[i] << '\n';
        }
    }
    vonal();


    while (kivEDb < 32) {
        std::cout << "Eszkoz index (0=kész): ";
        int val = readInt();


        if (val == 0) break;


        int idx = val - 1;


        if (idx < 0 || idx >= eszkozDb) {
            std::cout << "  Ervenytelen index (1-" << eszkozDb << ").\n";
            continue;
        }


        if (eszkozLista[idx]->getDb() <= 0) {
            std::cout << "  Nincs szabad peldany.\n";
            continue;
        }


        //eszkozLista[idx]->csokkentDb(1);
        eszkozLista[idx]->novHasznalat(1);
        kivE[kivEDb++] = eszkozLista[idx];
        std::cout << "  + " << eszkozLista[idx]->getNev()
                  << " (maradek: " << eszkozLista[idx]->getDb() - eszkozLista[idx]->getHasznalat() << " db)\n";
    }


    if (kivEDb == 0) {
        std::cout << "  Nincs kivalasztott eszkoz, megszakitva.\n";
        return;
    }



    if (kiegDb > 0) {
        std::cout << "\n  Kiegeszitok (0=kesz):\n";
        for (int i = 0; i < kiegDb; ++i) {
            if (kiegLista[i]->getDb() > 0) {
                std::cout << "  [" << i+1 << "] " << *kiegLista[i] << '\n';
            }
        }
        vonal();
        while (kivKDb < 32) {
            std::cout << "Kiegészítő index (0=kész): ";
            int val = readInt();
            if (val == 0) break;
            int idx = val - 1;
            if (idx < 0 || idx >= kiegDb) {
                std::cout << "  Érvénytelen index (1-" << kiegDb << ").\n";
                continue;
            }
            if (kiegLista[idx]->getDb() - kiegLista[idx]->getHasznalat() <= 0) {
                std::cout << "  Nincs szabad példány.\n";
                continue;
            }
            //kiegLista[idx]->csokkentDb(1);
            kiegLista[idx]->novHasznalat(1);
            kivK[kivKDb++] = kiegLista[idx];
            std::cout << "  + " << kiegLista[idx]->getNev()
                      << " (maradék: " << kiegLista[idx]->getDb() - kiegLista[idx]->getHasznalat() << " db)\n";
        }

    }


    // ── elettartam es statusz ─────────────────────────────
    std::cout << "Kolcsonzesi idotartam (nap): ";
    int elT = readInt();
    if (elT <= 0) {
        // visszavonás: visszaadjuk a lefoglalt eszközöket ÉS kiegészítőket
        std::cout << "  Ervenytelen idotartam – visszavonas.\n";
        for (int i = 0; i < kivEDb; ++i) {
            kivE[i]->novDb(1);
            kivE[i]->novHasznalat(-1);
        }
        for (int i = 0; i < kivKDb; ++i) {
            kivK[i]->novDb(1);
            kivK[i]->novHasznalat(-1);
        }
        return;
    }


    std::cout << "Aktiv rendeles? [1=igen / 0=nem]: ";
    bool akt = (readInt() == 1);


    Rendeles* tmp = new Rendeles[rendelesDb + 1];
    for (int i = 0; i < rendelesDb; ++i) tmp[i] = rendelesek[i];


    tmp[rendelesDb] = Rendeles(kovetkezoId, elT, akt);


    for (int i = 0; i < kivEDb; ++i) tmp[rendelesDb].addEszkoz(kivE[i]);


    for (int i = 0; i < kivKDb; ++i) tmp[rendelesDb].addKiegeszito(kivK[i]);


    delete[] rendelesek;


    rendelesek = tmp;


    ++rendelesDb;


    const Rendeles& uj = rendelesek[rendelesDb - 1];
    std::cout << "\n  Rendeles #" << kovetkezoId << " rogzitve!"
              << "  Osszar: " << uj.getOsszAr() << " Ft\n";
    ++kovetkezoId;
    ment();
}


// ───────────────────────────────────


void Rendszer::rendelesekLeker() const {
    vonal('=');
    std::cout << "  RENDELESEK LISTAJA\n";
    vonal('=');


    std::cout << "Szuro: [1] Aktiv  [2] Inaktiv  [3] Osszes\n> ";
    int  szuro = readInt();
    bool volt  = false;


    for (int i = 0; i < rendelesDb; ++i) {
        const Rendeles& r = rendelesek[i];
        if (szuro == 1 && !r.isAktiv()) continue;
        if (szuro == 2 &&  r.isAktiv()) continue;
        volt = true;
        std::cout << '\n' << r;
    }
    if (!volt) std::cout << "  Nincs ilyen rendeles.\n";
}


void Rendszer::rendelesTorol() {
    vonal('=');
    std::cout << "  RENDELES LEZARASA\n";
    vonal('=');


    bool volt = false;
    for (int i = 0; i < rendelesDb; ++i) {
        if (rendelesek[i].isAktiv()) {
            std::cout << "  [" << i+1 << "] #" << rendelesek[i].getId()
                      << "  " << rendelesek[i].getElettartam() << " nap"
                      << "  Osszar: " << rendelesek[i].getOsszAr() << " Ft\n";
            volt = true;
        }
    }
    if (!volt) {
        std::cout << "  Nincs aktiv rendeles.\n";
        return;
    }
    vonal();


    std::cout << "Index (1-" << rendelesDb << "): ";
    int idx = readInt() - 1;
    if (idx < 0 || idx >= rendelesDb) {
        std::cout << "  Ervenytelen index.\n";
        return;
    }
    if (!rendelesek[idx].isAktiv()) {
        std::cout << "  Ez a rendeles mar le van zarva.\n";
        return;
    }


    // eszközök ES kiegeszitok visszavetele a keszletbe
    for (int j = 0; j < rendelesek[idx].getEszkozDb(); ++j)
        rendelesek[idx].getEszkoz(j)->csokHasznalat(1);
    for (int j = 0; j < rendelesek[idx].getKiegDb(); ++j)
        rendelesek[idx].getKiegeszito(j)->csokHasznalat(1);


    rendelesek[idx].lezar();
    std::cout << "  Rendeles #" << rendelesek[idx].getId()
              << " lezarva. Eszkozok visszaveve.\n";
    ment();
}



void Rendszer::statisztika() const {
    vonal('=');
    std::cout << "  STATISZTIKAK\n";
    vonal('=');


    if (eszkozDb == 0) {
        std::cout << "  Nincs eszkoz az adatbazisban.\n";
        return;
    }


    int maxIdx = 0, minIdx = 0;
    for (int i = 1; i < eszkozDb; ++i) {
        if (eszkozLista[i]->getHasznalat() > eszkozLista[maxIdx]->getHasznalat()) maxIdx = i;
        if (eszkozLista[i]->getHasznalat() < eszkozLista[minIdx]->getHasznalat()) minIdx = i;
    }


    std::cout << "  Legtobbet rendelt:     " << *eszkozLista[maxIdx] << '\n';
    std::cout << "  Legkevesebbet rendelt: " << *eszkozLista[minIdx] << '\n';


    if (rendelesDb > 0) {
        int ossz = 0;
        for (int i = 0; i < rendelesDb; ++i) ossz += rendelesek[i].getElettartam();
        std::cout << "  Atlagos kolcsonzesi ido: " << (ossz / rendelesDb) << " nap\n";
    } else {
        std::cout << "  Meg nincs rendeles a statisztikahoz.\n";
    }
    vonal();
}