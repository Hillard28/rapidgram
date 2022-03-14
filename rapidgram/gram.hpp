#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>

double cratio(std::string string1, std::string string2) {
    std::vector<std::pair<char, char>> s1, s2, sunion;
    size_t l1 = string1.size() - 1;
    s1.reserve(l1);
    for (int i = 0; i < l1; i += 1){
        s1.push_back(std::pair<char, char>(string1[i], string1[i+1]));
    }
    size_t l2 = string2.size() - 1;
    sunion.reserve(l1+l2);
    sunion = s1;
    s2.reserve(l2);
    for (int i = 0; i < l2; i += 1){
        s2.push_back(std::pair<char, char>(string2[i], string2[i+1]));
        sunion.push_back(std::pair<char, char>(string2[i], string2[i+1]));
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

double cpartial_ratio(std::string string1, std::string string2) {
    std::vector<std::pair<char, char>> ls, ss, sunion;
    size_t l1 = string1.size() - 1;
    size_t l2 = string2.size() - 1;
    double jacc_max = 0.0;
    if (l1 > l2) {
        ss.reserve(l2);
        for (int i = 0; i < l2; i += 1){
            ss.push_back(std::pair<char, char>(string2[i], string2[i+1]));
        }
        for (int d = 0; d < l1 - l2 + 1; d += 1) {
            sunion.reserve(l2*2);
            sunion = ss;
            ls.reserve(l2);
            for (int i = 0 + d; i < l2 + d; i += 1){
                ls.push_back(std::pair<char, char>(string1[i], string1[i+1]));
                sunion.push_back(std::pair<char, char>(string1[i], string1[i+1]));
            }
            std::sort(sunion.begin(), sunion.end());
            sunion.erase(std::unique(sunion.begin(), sunion.end()), sunion.end());

            size_t lu = sunion.size();
            std::vector<int> f1, f2;
            f1.reserve(lu);
            f2.reserve(lu);
            for (int i = 0; i < lu; i += 1){
                std::pair<char, char> bi = sunion[i];
                f1.push_back(std::count(ls.begin(), ls.end(), bi));
                f2.push_back(std::count(ss.begin(), ss.end(), bi));
            }

            double jacc = std::inner_product(f1.begin(), f1.end(), f2.begin(), 0.0)
            / std::sqrt(std::inner_product(f1.begin(), f1.end(), f1.begin(), 0.0)
            * std::inner_product(f2.begin(), f2.end(), f2.begin(), 0.0));

            if (jacc == 1.0) {
                jacc_max = jacc;
                break;
            }
            else if (jacc > jacc_max) {
                jacc_max = jacc;
            }
            ls.clear();
            sunion.clear();
            f1.clear();
            f2.clear();
        }
    }
    else {
        ss.reserve(l1);
        for (int i = 0; i < l1; i += 1){
            ss.push_back(std::pair<char, char>(string1[i], string1[i+1]));
        }
        for (int d = 0; d < l2 - l1 + 1; d += 1) {
            sunion.reserve(l1*2);
            sunion = ss;
            ls.reserve(l1);
            for (int i = 0 + d; i < l1 + d; i += 1){
                ls.push_back(std::pair<char, char>(string2[i], string2[i+1]));
                sunion.push_back(std::pair<char, char>(string2[i], string2[i+1]));
            }
            std::sort(sunion.begin(), sunion.end());
            sunion.erase(std::unique(sunion.begin(), sunion.end()), sunion.end());

            size_t lu = sunion.size();
            std::vector<int> f1, f2;
            f1.reserve(lu);
            f2.reserve(lu);
            for (int i = 0; i < lu; i += 1){
                std::pair<char, char> bi = sunion[i];
                f1.push_back(std::count(ss.begin(), ss.end(), bi));
                f2.push_back(std::count(ls.begin(), ls.end(), bi));
            }

            double jacc = std::inner_product(f1.begin(), f1.end(), f2.begin(), 0.0)
            / std::sqrt(std::inner_product(f1.begin(), f1.end(), f1.begin(), 0.0)
            * std::inner_product(f2.begin(), f2.end(), f2.begin(), 0.0));

            if (jacc == 1.0) {
                jacc_max = jacc;
                break;
            }
            else if (jacc > jacc_max) {
                jacc_max = jacc;
            }
            ls.clear();
            sunion.clear();
            f1.clear();
            f2.clear();
        }
    }
    return jacc_max;
}