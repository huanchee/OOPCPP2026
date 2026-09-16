
#include "utils.hpp"
#include <cstdlib>   // atoi, atof
#include "memtrace.h"

void strCopy(char* dst, const char* src, int maxLen) {
    int i = 0;
    while (i < maxLen - 1 && src[i] != '\0') {
        dst[i] = src[i];
        ++i;
    }
    dst[i] = '\0';
}

bool strEq(const char* a, const char* b) {
    while (*a && *b) {
        if (*a != *b) return false;
        ++a; ++b;
    }
    return *a == '\0' && *b == '\0';
}

void readLine(char* buf, int maxLen) {
    buf[0] = '\0';
    while (std::cin.good()) {
        std::cin.getline(buf, maxLen);
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
        }
        if (buf[0] != '\0') return;
    }
}

int readInt() {
    char buf[64];
    readLine(buf, 64);
    return atoi(buf);
}

double readDouble() {
    char buf[64];
    readLine(buf, 64);
    return atof(buf);
}

void vonal(std::ostream& os, char c, int n) {
    for (int i = 0; i < n; ++i) os << c;
    os << '\n';
}

void vonal(char c, int n) {
    vonal(std::cout, c, n);
}
