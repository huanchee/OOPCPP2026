
# Videós Rental Rendszer

> Programozás alapjai 2. – NHF

Parancssoros C++ alkalmazás videós kölcsönző cégek kimenő rendeléseinek kezelésére. A rendszer nyilvántartja a bérelhető eszközöket és kiegészítőket, kezeli a készletet és a rendeléseket, valamint alapvető használati statisztikákat készít.

## Tartalom

- [Cél](#cél)
- [Fő funkciók](#fő-funkciók)
- [Adattárolás és I/O](#adattárolás-és-io)
- [Osztálystruktúra](#osztálystruktúra)
- [Működés](#működés)
- [Statisztikák](#statisztikák)

## Cél

A program egy videós rental (kölcsönző) cég kimenő rendeléseit kezeli.

Nyilvántartja:

- A kölcsönözhető eszközöket, például kamerákat, lámpákat, állványokat és más felszereléseket
- Az eszközökhöz kapcsolható kiegészítőket, például akkumulátorokat, táskákat és memóriakártyákat
- Az aktuálisan elérhető, szabad készletet
- Az aktív és lezárt rendeléseket
- Az eszközök használatára vonatkozó statisztikákat

## Fő funkciók

| Funkció | Leírás |
| --- | --- |
| Eszköz hozzáadása | Új eszköz vagy kiegészítő felvétele a készletbe. Azonos név esetén a rendszer a meglévő tétel darabszámát növeli. |
| Eszköz törlése | Egy meglévő eszköz készletének csökkentése. Ha a darabszám nullára csökken, az eszköz kikerül a nyilvántartásból. |
| Elérhető eszközök listája | Kizárólag a jelenleg szabad, pozitív darabszámú eszközök és kiegészítők kiírása. |
| Rendelés összeállítása | Eszközök és opcionális kiegészítők kiválasztása index alapján, kölcsönzési idő és státusz megadásával. A rendszer kiszámítja az összárat. |
| Rendelések lekérése | A korábbi rendelések listázása: csak aktív, csak lezárt vagy az összes rendelés. |
| Rendelés lezárása | Egy aktív rendelés lezárása index alapján és a kapcsolódó eszközök visszavétele a készletbe. |
| Statisztikák | Legtöbbet és legkevesebbet rendelt eszköz, valamint átlagos kölcsönzési idő meghatározása. |

## Adattárolás és I/O

| Tárolási forma | Fájl | Tartalom |
| --- | --- | --- |
| CSV | `eszkozok.csv` | Az eszközök és kiegészítők listája: név, ár, darabszám és statisztikai adatok. |
| Szöveges fájl | `rendelesek.txt` | Az összes rögzített rendelés. |

### Bemenet

A program a standard bemenetet használja. A felhasználó a főmenüből választ funkciót, majd soralapú adatbevitel során adja meg a szükséges adatokat, például:

- Típus: eszköz vagy kiegészítő
- Név
- Ár
- Darabszám
- Index
- Kölcsönzési idő napokban
- Aktív vagy inaktív státusz

### Kimenet

A program a standard kimeneten jeleníti meg a listákat, visszajelzéseket, hibaüzeneteket és statisztikákat. A futás során a CSV- és TXT-fájlok mentése is megtörténik.

## Osztálystruktúra

### `EszkozAlap` – absztrakt ősosztály

Az `EszkozAlap` az eszközök és kiegészítők közös absztrakt őse.

| Attribútum | Típus | Leírás |
| --- | --- | --- |
| `nev` | `char[64]` | Az eszköz vagy kiegészítő neve |
| `ar` | `double` | Egy napra vonatkozó díj |
| `db` | `int` | Az aktuális darabszám |

A két tisztán virtuális függvény:

```cpp
virtual void kiir(std::ostream& os) const = 0;
virtual void csvSor(char* buf, int n) const = 0;
```

A virtuális destruktor biztosítja, hogy az objektumok bázisosztály-pointeren keresztül is helyesen törlődjenek.

### `Eszkoz`

Az `Eszkoz` a kölcsönözhető alapeszközöket reprezentálja, például kamerát, lámpát vagy állványt.

| További attribútum | Típus | Leírás |
| --- | --- | --- |
| `hasznalat` | `int` | Azt tárolja, hányszor szerepelt az eszköz rendelésben |

Az osztály saját `kiir` és `csvSor` implementációval rendelkezik.

### `Kiegeszito`

A `Kiegeszito` egy rendeléshez opcionálisan választható elem, például akkumulátor, táska vagy memóriakártya.

- Az `EszkozAlap` osztályból származik
- Nem rendelkezik külön használatszámlálóval
- Kiíráskor `[kiegészítő]` jelölést kap
- A CSV-fájlban `KIEG:` előtaggal mentődik

### `Rendeles`

A `Rendeles` egy kölcsönzési tranzakció adatait tárolja.

| Attribútum | Típus | Leírás |
| --- | --- | --- |
| `id` | `int` | Egyedi rendelésazonosító |
| `eszkozok` | `Eszkoz**` | Dinamikusan bővülő pointertömb a kiválasztott eszközökhöz |
| `eszkozDb` | `int` | A rendelésben lévő eszközök száma |
| `kiegeszitok` | `Kiegeszito**` | Dinamikusan bővülő pointertömb a kiválasztott kiegészítőkhöz |
| `kiegDb` | `int` | A rendelésben lévő kiegészítők száma |
| `elettartam` | `int` | A kölcsönzés időtartama napokban |
| `aktiv` | `bool` | A rendelés aktív vagy lezárt állapota |

A rendelésben tárolt pointerek a `Rendszer` által kezelt heapen lévő objektumokra mutatnak.

### `Rendszer`

A `Rendszer` a program központi menedzserosztálya.

Feladatai:

- Eszközök kezelése `Eszkoz**` pointertömbben
- Kiegészítők kezelése `Kiegeszito**` pointertömbben
- Rendelések kezelése `Rendeles*` tömbben
- Darabszámok és a következő rendelésazonosító nyilvántartása
- CSV- és TXT-fájlok beolvasása és mentése
- A felhasználói menü funkcióinak kiszolgálása

A másolás szándékosan tiltott:

```cpp
Rendszer(const Rendszer&) = delete;
Rendszer& operator=(const Rendszer&) = delete;
```

## Működés

### Eszköz vagy kiegészítő hozzáadása

1. A rendszer bekéri a típust, nevet, árat és darabszámot.
2. Ha a megadott darabszám legfeljebb nulla, hibaüzenetet ad és megszakítja a műveletet.
3. Eszköz esetén ellenőrzi, hogy szerepel-e már ilyen nevű eszköz a listában.
4. Meglévő név esetén növeli a darabszámot, majd menti az adatokat.
5. Új név esetén új heapen foglalt `Eszkoz` objektumot hoz létre, újraallokálja a pointertömböt, és a tömb végére fűzi az új elemet.
6. Kiegészítő esetén ugyanez a folyamat külön `Kiegeszito` objektummal és kiegészítőlistával történik.
7. A rendszer visszajelzést ad, majd menti az állapotot az `eszkozok.csv` fájlba.

### Eszköz törlése

1. A rendszer indexszel és aktuális darabszámmal kiírja az eszközlistát.
2. A felhasználó megadja a törlendő eszköz indexét és mennyiségét.
3. Érvénytelen index vagy nem pozitív darabszám esetén hibaüzenet jelenik meg.
4. Ha a szabad készlet kisebb a kért mennyiségnél, a rendszer nem hajtja végre a törlést.
5. A rendszer csökkenti a kiválasztott eszköz darabszámát.
6. Nullára csökkenő készlet esetén törli a heapen lévő objektumot, majd egy új pointertömbbe másolja a megmaradt elemeket.
7. A módosított lista mentésre kerül CSV-be.

### Új rendelés összeállítása

1. A rendszer indexekkel kiírja az elérhető, pozitív darabszámú eszközöket.
2. A felhasználó eszközöket választ; a `0` érték lezárja a kiválasztást.
3. Minden érvényesen kiválasztott eszköz esetén a szabad darabszám eggyel csökken, a használatszámláló pedig eggyel nő.
4. Ha egyetlen eszköz sincs kiválasztva, a rendelés létrehozása megszakad.
5. Ezután opcionálisan kiegészítők választhatók. A kiegészítők készlete nem csökken.
6. A felhasználó megadja a kölcsönzési időt és az aktív/inaktív státuszt.
7. Nem pozitív kölcsönzési idő esetén a rendszer visszavonja a már lefoglalt eszközöket: visszanöveli a darabszámot és csökkenti a használatszámlálót.
8. Érvényes adatoknál létrejön az új `Rendeles` objektum egyedi azonosítóval, majd bekerül a rendelések tömbjébe.
9. Az összár képlete:

\[
(\sum \text{eszköz ára} + \sum \text{kiegészítő ára}) \times \text{élettartam}
\]

10. A rendszer frissíti az `eszkozok.csv` és `rendelesek.txt` fájlokat.

### Rendelés lezárása

1. A rendszer csak az aktív rendeléseket listázza, indexszel és összárral.
2. A felhasználó kiválasztja a lezárandó rendelést.
3. Érvénytelen index vagy már lezárt rendelés esetén hibaüzenet jelenik meg.
4. A rendelésben szereplő minden eszköz darabszáma eggyel növekszik, így visszakerül a szabad készletbe.
5. A rendelés állapota inaktívra változik.
6. A rendszer menti a friss adatokat a rendelés- és eszközfájlokba.

## Statisztikák

A statisztikai funkció az alábbi adatokat jeleníti meg:

- A legtöbbet rendelt eszköz
- A legkevesebbet rendelt eszköz
- Az átlagos kölcsönzési idő napokban

A legtöbb és legkevesebb használat meghatározása egyetlen lineáris bejárással történik. Ha nincs egyetlen eszköz sem a rendszerben, a program üres készletre utaló üzenetet ír ki.

Az átlagos kölcsönzési idő számítása:

\[
\text{átlagos idő} = \frac{\sum \text{rendelés élettartama}}{\text{rendelések száma}}
\]

A program ezt csak akkor számolja ki, ha legalább egy rendelés létezik. Az eredmény egész osztással jön létre.

## UML-diagram

Az osztálydiagram fő kapcsolatai:

```text
                    EszkozAlap (absztrakt)
                         /          \
                        /            \
                  Eszkoz          Kiegeszito
                     ^                ^
                     |                |
                     +------ Rendeles +
                              ^
                              |
                           Rendszer
```

- Az `Eszkoz` és a `Kiegeszito` az `EszkozAlap` leszármazottai.
- A `Rendeles` kiválasztott eszközökre és kiegészítőkre mutató pointereket tárol.
- A `Rendszer` az eszközök, kiegészítők és rendelések teljes életciklusát kezeli.
