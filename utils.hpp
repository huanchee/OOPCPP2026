#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>

void   strCopy   (char* dst, const char* src, int maxLen);
bool   strEq     (const char* a, const char* b);


void   readLine  (char* buf, int maxLen);
int    readInt   ();
double readDouble();

void   vonal(std::ostream& os, char c = '-', int n = 52);
void   vonal(char c = '-', int n = 52);

#endif // UTILS_HPP
