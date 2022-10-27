#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <cmath>

// Baseline fuzzy comparison of two strings
double cratio(std::string string1, std::string string2, bool strict=false) {
    std::vector<std::pair<char, char> > s1, s2, sunion;
    // If either string is empty, return 0.0
    if (string1.empty()||string2.empty()) {
        return 0.0;
    }
    // Do standard, less expensive comparison, return 1.0 if both strings are the same
    else if (string1 == string2) {
        return 1.0;
    }
    else {
        size_t l1 = string1.size() - 1;
        s1.reserve(l1+2);
        // Add a space at the beggining for capturing transpositions if loose matching
        if (strict == false) {
            s1.push_back(std::pair<char, char>(' ', string1[0]));
        }
        for (int i = 0; i < l1; i += 1){
            s1.push_back(std::pair<char, char>(string1[i], string1[i+1]));
        }
        // Add a space at the end if loose matching
        if (strict == false) {
            s1.push_back(std::pair<char, char>(string1[l1], ' '));
        }
        size_t l2 = string2.size() - 1;
        // Repeat for the second string and take the union of both strings
        sunion.reserve(l1+l2+4);
        sunion = s1;
        s2.reserve(l2+2);
        if (strict == false) {
            s2.push_back(std::pair<char, char>(' ', string2[0]));
            sunion.push_back(std::pair<char, char>(' ', string2[0]));
        }
        for (int i = 0; i < l2; i += 1){
            s2.push_back(std::pair<char, char>(string2[i], string2[i+1]));
            sunion.push_back(std::pair<char, char>(string2[i], string2[i+1]));
        }
        if (strict == false) {
            s2.push_back(std::pair<char, char>(string2[l2], ' '));
            sunion.push_back(std::pair<char, char>(string2[l2], ' '));
        }
        std::sort(sunion.begin(), sunion.end());
        sunion.erase(std::unique(sunion.begin(), sunion.end()), sunion.end());
        
        // Calculate the frequency at which each unique char pairing occurs in both strings
        size_t lu = sunion.size();
        std::vector<int> f1, f2;
        f1.reserve(lu);
        f2.reserve(lu);
        for (int i = 0; i < lu; i += 1){
            std::pair<char, char> bi = sunion[i];
            f1.push_back(std::count(s1.begin(), s1.end(), bi));
            f2.push_back(std::count(s2.begin(), s2.end(), bi));
        }
        
        // Compute similarity score using dot product of both frequency vectors
        double jacc = std::inner_product(f1.begin(), f1.end(), f2.begin(), 0.0)
        / std::sqrt(std::inner_product(f1.begin(), f1.end(), f1.begin(), 0.0)
        * std::inner_product(f2.begin(), f2.end(), f2.begin(), 0.0));
        
        return jacc;
    }
}

// Modified fuzzy comparison that compares short string against rolling window of long string
double cpartial_ratio(std::string string1, std::string string2, bool strict = false) {
    std::vector<std::pair<char, char> > ls, ss, sunion;
    if (string1.empty()||string2.empty()) {
        return 0.0;
    }
    else if (string1 == string2) {
        return 1.0;
    }
    else {
        // Compute length of both strings for determining short/long
        size_t l1 = string1.size() - 1;
        size_t l2 = string2.size() - 1;
        double jacc_max = 0.0;
        // If length is the same do a standard comparison
        if (l1 == l2) {
            ls.reserve(l1+2);
            if (strict == false) {
                ls.push_back(std::pair<char, char>(' ', string1[0]));
            }
            for (int i = 0; i < l1; i += 1){
                ls.push_back(std::pair<char, char>(string1[i], string1[i+1]));
            }
            if (strict == false) {
                ls.push_back(std::pair<char, char>(string1[l1], ' '));
            }
            sunion.reserve(l1+l2+4);
            sunion = ls;
            ss.reserve(l2+2);
            if (strict == false) {
                ss.push_back(std::pair<char, char>(' ', string2[0]));
                sunion.push_back(std::pair<char, char>(' ', string2[0]));
            }
            for (int i = 0; i < l2; i += 1){
                ss.push_back(std::pair<char, char>(string2[i], string2[i+1]));
                sunion.push_back(std::pair<char, char>(string2[i], string2[i+1]));
            }
            if (strict == false) {
                ss.push_back(std::pair<char, char>(string2[l2], ' '));
                sunion.push_back(std::pair<char, char>(string2[l2], ' '));
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

            jacc_max = std::inner_product(f1.begin(), f1.end(), f2.begin(), 0.0)
            / std::sqrt(std::inner_product(f1.begin(), f1.end(), f1.begin(), 0.0)
            * std::inner_product(f2.begin(), f2.end(), f2.begin(), 0.0));
        }
        // If string 1 larger than string 2, compare rolling window of string 1 equivalent to size of string 2
        else if (l1 > l2) {
            ss.reserve(l2+2);
            if (strict == false) {
                ss.push_back(std::pair<char, char>(' ', string2[0]));
            }
            for (int i = 0; i < l2; i += 1){
                ss.push_back(std::pair<char, char>(string2[i], string2[i+1]));
            }
            if (strict == false) {
                ss.push_back(std::pair<char, char>(string2[l2], ' '));
            }
            // Start from beginning of larger string and roll across until end of window reaches last char
            for (int d = 0; d < l1 - l2 + 1; d += 1) {
                sunion.reserve(l2*2+4);
                sunion = ss;
                ls.reserve(l2+2);
                if (strict == false) {
                    ls.push_back(std::pair<char, char>(' ', string1[0+d]));
                    sunion.push_back(std::pair<char, char>(' ', string1[0+d]));
                }
                for (int i = 0 + d; i < l2 + d; i += 1){
                    ls.push_back(std::pair<char, char>(string1[i], string1[i+1]));
                    sunion.push_back(std::pair<char, char>(string1[i], string1[i+1]));
                }
                if (strict == false) {
                    ls.push_back(std::pair<char, char>(string1[l2+d], ' '));
                    sunion.push_back(std::pair<char, char>(string1[l2+d], ' '));
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
                
                // If similarity score of 1.0 is achieved, end loop and return score as it can't be improved
                if (jacc == 1.0) {
                    jacc_max = jacc;
                    break;
                }
                // Otherwise, if score is greater than current max score, replace
                else if (jacc > jacc_max) {
                    jacc_max = jacc;
                }
                ls.clear();
                sunion.clear();
                f1.clear();
                f2.clear();
            }
        }
        // If string 1 smaller than string 2, compare rolling window of string 2 equivalent to size of string 1
        else {
            ss.reserve(l1+2);
            if (strict == false) {
                ss.push_back(std::pair<char, char>(' ', string1[0]));
            }
            for (int i = 0; i < l1; i += 1){
                ss.push_back(std::pair<char, char>(string1[i], string1[i+1]));
            }
            if (strict == false) {
                ss.push_back(std::pair<char, char>(string1[l1], ' '));
            }
            for (int d = 0; d < l2 - l1 + 1; d += 1) {
                sunion.reserve(l1*2+4);
                sunion = ss;
                ls.reserve(l1+2);
                if (strict == false) {
                    ls.push_back(std::pair<char, char>(' ', string2[0+d]));
                    sunion.push_back(std::pair<char, char>(' ', string2[0+d]));
                }
                for (int i = 0 + d; i < l1 + d; i += 1){
                    ls.push_back(std::pair<char, char>(string2[i], string2[i+1]));
                    sunion.push_back(std::pair<char, char>(string2[i], string2[i+1]));
                }
                if (strict == false) {
                    ls.push_back(std::pair<char, char>(string2[l1+d], ' '));
                    sunion.push_back(std::pair<char, char>(string2[l1+d], ' '));
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
}

double ctoken(std::string string1, std::string string2) {
    std::vector<std::pair<char, char> > s1, s2, sunion;
    if (string1.empty()||string2.empty()) {
        return 0.0;
    }
    else if (string1 == string2) {
        return 1.0;
    }
    else {
        size_t l1 = string1.size() - 1;
        s1.reserve(l1);
        for (int i = 0; i < l1; i += 1){
            std::pair<char, char> pair1(string1[i], string1[i+1]);
            if (pair1.first == ' '||pair1.second == ' ') {
                continue;
            }
            else {
                s1.push_back(pair1);
            }
            
        }
        size_t l2 = string2.size() - 1;
        sunion.reserve(l1+l2);
        sunion = s1;
        s2.reserve(l2);
        for (int i = 0; i < l2; i += 1){
            std::pair<char, char> pair2(string2[i], string2[i+1]);
            if (pair2.first == ' '||pair2.second == ' ') {
                continue;
            }
            else {
                s2.push_back(std::pair<char, char>(string2[i], string2[i+1]));
                sunion.push_back(std::pair<char, char>(string2[i], string2[i+1]));
            }
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
}

double cpartial_token(std::string string1, std::string string2) {
    std::vector<std::pair<char, char> > ls, ss, sunion;
    if (string1.empty()||string2.empty()) {
        return 0.0;
    }
    else if (string1 == string2) {
        return 1.0;
    }
    else {
        size_t l1 = string1.size() - 1;
        size_t l2 = string2.size() - 1;
        double jacc_max = 0.0;
        if (l1 == l2) {
            size_t l1 = string1.size() - 1;
            ls.reserve(l1);
            for (int i = 0; i < l1; i += 1){
                std::pair<char, char> pair1(string1[i], string1[i+1]);
                if (pair1.first == ' '||pair1.second == ' ') {
                    continue;
                }
                else {
                    ls.push_back(pair1);
                }
                
            }
            size_t l2 = string2.size() - 1;
            sunion.reserve(l1+l2);
            sunion = ls;
            ss.reserve(l2);
            for (int i = 0; i < l2; i += 1){
                std::pair<char, char> pair2(string2[i], string2[i+1]);
                if (pair2.first == ' '||pair2.second == ' ') {
                    continue;
                }
                else {
                    ss.push_back(std::pair<char, char>(string2[i], string2[i+1]));
                    sunion.push_back(std::pair<char, char>(string2[i], string2[i+1]));
                }
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

            jacc_max = std::inner_product(f1.begin(), f1.end(), f2.begin(), 0.0)
            / std::sqrt(std::inner_product(f1.begin(), f1.end(), f1.begin(), 0.0)
            * std::inner_product(f2.begin(), f2.end(), f2.begin(), 0.0));
        }
        else if (l1 > l2) {
            ss.reserve(l2);
            for (int i = 0; i < l2; i += 1){
                std::pair<char, char> pair2(string2[i], string2[i+1]);
                if (pair2.first == ' '||pair2.second == ' ') {
                    continue;
                }
                else {
                    ss.push_back(pair2);
                }
            }
            for (int d = 0; d < l1 - l2 + 1; d += 1) {
                sunion.reserve(l2*2);
                sunion = ss;
                ls.reserve(l2);
                for (int i = 0 + d; i < l2 + d; i += 1){
                    std::pair<char, char> pair1(string1[i], string1[i+1]);
                    if (pair1.first == ' '||pair1.second == ' ') {
                        continue;
                    }
                    else {
                        ls.push_back(pair1);
                        sunion.push_back(pair1);
                    }
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
                std::pair<char, char> pair1(string1[i], string1[i+1]);
                if (pair1.first == ' '||pair1.second == ' ') {
                    continue;
                }
                else {
                    ss.push_back(pair1);
                }
            }
            for (int d = 0; d < l2 - l1 + 1; d += 1) {
                sunion.reserve(l1*2);
                sunion = ss;
                ls.reserve(l1);
                for (int i = 0 + d; i < l1 + d; i += 1){
                    std::pair<char, char> pair2(string2[i], string2[i+1]);
                    if (pair2.first == ' '||pair2.second == ' ') {
                        continue;
                    }
                    else {
                        ls.push_back(pair2);
                        sunion.push_back(pair2);
                    }
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
}
