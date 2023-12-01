#include "keyTools.h"

#include <map>
#include <regex>
#include <fstream>
#include <immintrin.h>

#define toCoord(a, b, c) ((a) * MAXHAMMER * MAXLEN + (b) * MAXHAMMER + (c))

using namespace std;

Hammer::Hammer(){}
Hammer::Hammer(char src, std::string target): src(src), target(std::move(target)){}


map<int, Hammer> readHammers(const string& file){
    map<int, Hammer> hammers;
    regex rgx("([0-9]+). ([A-Z]) -> ([A-Z]+) ([A-Z]+)");
    ifstream filestream(file);

    string line;
    while(getline(filestream, line)){
        smatch matches;
        if (regex_search(line, matches, rgx)){
            hammers[stoi(matches[1].str())] = Hammer(matches[2].str()[0], matches[3].str() + matches[4].str());
        }
    }

    return hammers;
}

vector<string> readKeys(const string& file, size_t pad){
    vector<string> keys;
    ifstream filestream(file);

    string line;
    while(getline(filestream, line)){
        keys.push_back(line);
    }

    for (size_t i = 0; i < keys.size() % pad; i++){
        keys.emplace_back("");
    }

    return keys;
}

void craftable(const string keys[], const int hammerA[], const int hammerB[], const int hammerC[],
               size_t numHammer, DTYPE result[]) {
    __m256i P[MAXLEN * MAXLEN * MAXHAMMER];
    size_t maxLen = 0;
    for (size_t i = 0; i < KEYS_PER_STEP; i++){
        if (keys[i].length() > maxLen) {
            maxLen = keys[i].length();
        }
    }

    for (auto &i: P) {
        i = _mm256_setzero_si256();
    }

    for (size_t p = 0; p < maxLen; p++) {
        for (size_t k = 0; k < KEYS_PER_STEP; k++){
            if (p >= keys[k].size()){
                continue;
            }
            auto* val = (DTYPE*) &P[toCoord(1, p + 1, keys[k][p] - 'A')];
            val[k] = 1;
        }
    }

    for (size_t l = 2; l <= maxLen; l++) {
        for (size_t p = 1; p <= maxLen - l + 1; p++) {
            for (size_t s = 1; s <= l - 1; s++) {
                for (size_t i = 0; i < numHammer; i++) {
                    auto a = hammerA[i];
                    auto b = hammerB[i];
                    auto c = hammerC[i];

                    __m256i newVal = _mm256_and_si256(P[toCoord(s, p, b)], P[toCoord(l - s, p + s, c)]);
                    P[toCoord(l, p, a)] = _mm256_or_si256(P[toCoord(l, p, a)], newVal);
                }
            }
        }
    }

    for (size_t i = 0; i < KEYS_PER_STEP; i++){
        auto* t = (DTYPE*) &P[toCoord(keys[i].size(), 1, 0)];
        result[i] = t[i];
    }
}
