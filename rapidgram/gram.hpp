#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>

double similarity(std::string string1, std::string string2) {
    std::vector<std::pair<char, char>> s1, s2, sunion;
    size_t l1 = string1.size() - 1;
    s1.reserve(l1);
    for (int i = 0; i < l1; i += 1){
        s1.push_back(std::pair<char, char>(string1.at(i), string1.at(i+1)));
    }
    size_t l2 = string2.size() - 1;
    sunion.reserve(l1+l2);
    sunion = s1;
    s2.reserve(l2);
    for (int i = 0; i < l2; i += 1){
        s2.push_back(std::pair<char, char>(string2.at(i), string2.at(i+1)));
        sunion.push_back(std::pair<char, char>(string2.at(i), string2.at(i+1)));
    }

    std::sort(sunion.begin(), sunion.end());
    sunion.erase(std::unique(sunion.begin(), sunion.end()), sunion.end());

    size_t lu = sunion.size();
    std::vector<int> f1, f2;
    f1.reserve(lu);
    f2.reserve(lu);
    for (int i = 0; i < lu; i += 1){
        std::pair<char, char> bi = sunion[i];
        f1.push_back(std::count(s1.begin(), s1.end(), bi));
        f2.push_back(std::count(s2.begin(), s2.end(), bi));
    }

    double jacc = std::inner_product(f1.begin(), f1.end(), f2.begin(), 0.0)
    / std::sqrt(std::inner_product(f1.begin(), f1.end(), f1.begin(), 0.0)
    * std::inner_product(f2.begin(), f2.end(), f2.begin(), 0.0));
    
    return jacc;
}