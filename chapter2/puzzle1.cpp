#include <iostream>
#include <vector>
#include <map>
#include <string>

#include "keyTools.h"

using namespace std;


int main(){
    map<int, Hammer> hammers = readHammers("inputs/hammer_collection.txt");
    vector<string> keys = readKeys("inputs/21_keymaker_forge.txt");

    for (size_t i = 0; i < keys.size() % KEYS_PER_STEP; i++){
        keys.emplace_back("");
    }

    int hammerA[MAXHAMMER];
    int hammerB[MAXHAMMER];
    int hammerC[MAXHAMMER];
    size_t numHammers = hammers.size();

    for (auto [id, hammer]: hammers){
        hammerA[id - 1] = hammer.src - 'A';
        hammerB[id - 1] = hammer.target[0] - 'A';
        hammerC[id - 1] = hammer.target[1] - 'A';
    }

    DTYPE result[KEYS_PER_STEP];

    for (size_t i = 0; i < keys.size() / KEYS_PER_STEP; i++){
        craftable(&(keys[KEYS_PER_STEP * i]), hammerA, hammerB, hammerC, numHammers, result);
        for (size_t j = 0; j < KEYS_PER_STEP; j++){
            if (result[j]){
                cout << keys[KEYS_PER_STEP * i + j] << endl;
            }
        }
    }

    return 0;
}
