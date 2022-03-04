#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>

double similarity(std::string string1, std::string string2) {
    std::vector<std::pair<char, char>> s1, s2, sunion;
    for (int i = 0; i < string1.size()-1; i += 1){
        s1.push_back(std::pair<char, char>(string1.at(i), string1.at(i+1)));
    }
    sunion = s1;
    for (int i = 0; i < string2.size()-1; i += 1){
        s2.push_back(std::pair<char, char>(string2.at(i), string2.at(i+1)));
        sunion.push_back(std::pair<char, char>(string2.at(i), string2.at(i+1)));
    }

    std::sort(sunion.begin(), sunion.end());
    sunion.erase(std::unique(sunion.begin(), sunion.end()), sunion.end());

    std::vector<int> f1, f2;
    for (int i = 0; i < sunion.size(); i += 1){
        f1.push_back(std::count(s1.begin(), s1.end(), sunion[i]));
        f2.push_back(std::count(s2.begin(), s2.end(), sunion[i]));
    }

    double jacc = std::inner_product(f1.begin(), f1.end(), f2.begin(), 0.0)
    / std::sqrt(std::inner_product(f1.begin(), f1.end(), f1.begin(), 0.0)
    * std::inner_product(f2.begin(), f2.end(), f2.begin(), 0.0));
    
    return jacc;
}
