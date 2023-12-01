#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <ranges>
#include <numeric>
#include <algorithm>
#include <set>

#include "fraction.h"
using namespace std;

vector<uint64_t> readFlasks(const string& line){
    stringstream ss(line);

    vector<uint64_t> res;

    string flask;
    while(getline(ss, flask, ' ')){
        res.push_back(stoull(flask));
    }

    return res;
}

vector<tuple<int, vector<uint64_t>>> readFile(const string& file){
    vector<tuple<int, vector<uint64_t>>> res;
    ifstream filestream(file);

    string line;
    while(getline(filestream, line)){
        size_t idSep = line.find(':');
        int id = stoi(line.substr(0, idSep));

        line = line.substr(idSep + 2);
        size_t sep = line.find('-');

        tuple<int, vector<uint64_t>> trap(id, readFlasks(line.substr(0, sep - 1)));
        res.push_back(trap);
    }

    return res;
}

bool defusableStep(const Fraction& sum, const vector<uint64_t>& flasks, uint64_t start, size_t depth){
    if (depth == 1){
        if (sum.num == 1 && sum.denom >= start &&
            find(flasks.begin(), flasks.end(), sum.denom) == flasks.end()){
            return true;
        }
        return false;
    }

    for (;; start++){
        if (find(flasks.begin(), flasks.end(), start) != flasks.end()){
            continue;
        }

        Fraction maxRemaining(1, start);
        for (uint64_t i = 1; i < depth; i++){
            maxRemaining = maxRemaining + Fraction(1, start + i);
        }

        if (sum > maxRemaining) {
            break;
        }

        Fraction diff = sum - Fraction(1, start);

        if (diff.num < 0){
            continue;
        }

        if (defusableStep(diff, flasks, start + 1, depth - 1)){
            return true;
        }
    }

    return false;
}

bool defusable(const tuple<int, vector<uint64_t>>& trap){
    auto flasks = get<1>(trap);
    Fraction sum;

    for (auto flask: flasks){
        sum = sum + Fraction(1, flask);
    }

    return defusableStep(sum, flasks, 1, flasks.size());
}

int main(){
    auto traps = readFile("inputs/23_trap_right_side.txt");

    uint64_t sum = 0;
    for (auto t: traps){
        if (defusable(t)){
            sum += get<0>(t);
        }
    }
    cout << sum << endl;
    return 0;
}
