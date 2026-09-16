

#include <iostream>
#include "rendszer.hpp"
#include "utils.hpp"
#include "memtrace.h"

int main() {
    Rendszer r;

    while (true) {
        vonal('*');
        std::cout << "       VIDEOS RENTAL RENDSZER\n";
        vonal('*');
        std::cout << "  [1] Eszkoz hozzaadasa\n"
                  << "  [2] Eszkoz torlese\n"
                  << "  [3] Elerheto eszkozok listaja\n"
                  << "  [4] Rendeles osszeallitasa\n"
                  << "  [5] Rendelesek lekerese\n"
                  << "  [6] Rendeles lezarasa\n"
                  << "  [7] Statisztikak\n"
                  << "  [0] Kilepes\n";
        vonal('*');
        std::cout << "Valasztas: ";
        int v = readInt();
        std::cout << '\n';

        switch (v) {
            case 1: r.eszkozHozzaad();    break;
            case 2: r.eszkozTorol();      break;
            case 3: r.elerheto_lista();   break;
            case 4: r.ujRendeles();       break;
            case 5: r.rendelesekLeker();  break;
            case 6: r.rendelesTorol();    break;
            case 7: r.statisztika();      break;
            case 0:
                std::cout << "  Viszlat!\n";
                return 0;
            default:
                std::cout << "  Ismeretlen opcio, probald ujra.\n";
        }
        std::cout << '\n';
    }
}
