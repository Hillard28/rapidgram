#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>

std::vector<std::string> bigram(std::string initial_str) {
    int len = initial_str.size();
    std::vector<std::string> tokens;
    for (int i = 0; i < len-1; i += 1){
        tokens.push_back(initial_str.substr(i, 2));
    }
    return tokens;
}

std::vector<std::string> vsunion(std::vector<std::string> s1, std::vector<std::string> s2) {
    std::vector<std::string> union_str(s1);
    union_str.insert(union_str.end(), s2.begin(), s2.end());
    std::sort(union_str.begin(), union_str.end());
    union_str.erase(std::unique(union_str.begin(), union_str.end()), union_str.end());
    return union_str;
}

std::vector<int> ufreq(std::vector<std::string> u, std::vector<std::string> s) {
    int len = u.size();
    std::vector<int> vfreq;
    for (int i = 0; i < len; i += 1){
        int freq = std::count(s.begin(), s.end(), u[i]);
        vfreq.push_back(freq);
    }
    return vfreq;
}

float jaccard(std::vector<int> f1, std::vector<int> f2) {
    float num = std::inner_product(f1.begin(), f1.end(), f2.begin(), 0.0);
    float den1 = std::inner_product(f1.begin(), f1.end(), f1.begin(), 0.0);
    float den2 = std::inner_product(f2.begin(), f2.end(), f2.begin(), 0.0);
    float jacc = num / std::sqrt(den1 * den2);
    return jacc;
}

float similarity(std::string string1, std::string string2) {
    std::vector<std::string> new_str = bigram(string1);
    std::vector<std::string> new_str2 = bigram(string2);
    std::vector<std::string> union_str = vsunion(new_str, new_str2);
    std::vector<int> freq1 = ufreq(union_str, new_str);
    std::vector<int> freq2 = ufreq(union_str, new_str2);
    float score = jaccard(freq1, freq2);
    return score;
}