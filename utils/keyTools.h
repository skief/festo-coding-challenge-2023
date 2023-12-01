#pragma once

#include <map>
#include <vector>
#include <string>
#include <inttypes.h>

#define MAXLEN 40
#define MAXHAMMER 10
#define KEYS_PER_STEP 32
#define DTYPE uint8_t

class Hammer{
public:
    char src;
    std::string target;

    Hammer();
    Hammer(char src, std::string target);
};

std::map<int, Hammer> readHammers(const std::string& file);
std::vector<std::string> readKeys(const std::string& file, size_t pad=1);
void craftable(const std::string keys[], const int hammerA[], const int hammerB[], const int hammerC[],
               size_t numHammer, DTYPE result[]);
