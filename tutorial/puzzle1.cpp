#include <iostream>
#include <vector>
#include <string>
#include "keyTools.h"
#include <ranges>

using namespace std;

bool isSorted(string key){
    for (size_t i = 1; i < key.length(); i++){
        if (key[i-1] > key[i]){
            return false;
        }
    }
    return true;
}


int main() {
    vector<string> keys = readKeys("inputs/01_keymaker_ordered.txt");

    for (const auto& key: keys | ranges::views::filter(isSorted)){
        cout << key << endl;
    }

    return 0;
}
