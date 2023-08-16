#ifndef CGRAM_H
#define CGRAM_H

#include <string>

// Baseline fuzzy comparison of two strings
double cratio(std::string string1, std::string string2, bool strict=false);

// Modified fuzzy comparison that compares short string against rolling window of long string
double cpartial_ratio(std::string string1, std::string string2, bool strict=false);

double ctoken(std::string string1, std::string string2);

double cpartial_token(std::string string1, std::string string2);

#endif
