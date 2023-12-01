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

vector<uint64_t> readEmptyFlasks(const string& line){
    vector<uint64_t> res;

    size_t startPos;
    size_t endPos = 0;
    while((startPos = line.find('(', endPos)) != string::npos){
        endPos = line.find(')', startPos);
        res.push_back(stoull(line.substr(startPos + 1, endPos - startPos - 1)));
    }

    return res;
}

vector<tuple<int, vector<uint64_t>, vector<uint64_t>>> readFile(const string& file){
    vector<tuple<int, vector<uint64_t>, vector<uint64_t>>> res;
    ifstream filestream(file);

    string line;
    while(getline(filestream, line)){
        size_t idSep = line.find(':');
        int id = stoi(line.substr(0, idSep));

        line = line.substr(idSep + 2);
        size_t sep = line.find('-');

        tuple<int, vector<uint64_t>, vector<uint64_t>> trap(id, readFlasks(line.substr(0, sep - 1)), readEmptyFlasks(line.substr(sep + 1)));
        res.push_back(trap);
    }

    return res;
}

bool defusableSteps(uint64_t denom, const vector<uint64_t>& flasks){
    if (find(flasks.begin(), flasks.end(), denom) != flasks.end()){
        return true;
    }

    uint64_t smallest = flasks[flasks.size() - 1];

    for (auto f: flasks){
        if (f < denom){
            continue;
        } else if (f == denom){
            return true;
        }

        uint64_t diff = lcm(denom, f);

        if (diff > smallest){
            return false;
        }

        if (defusableSteps(diff, flasks)){
            return true;
        }
    }

    return false;
}

bool defusable(const tuple<int, vector<uint64_t>, vector<uint64_t>>& trap){
    auto flasks = get<1>(trap);
    Fraction sum;

    for (auto flask: flasks){
        sum = sum + Fraction(1, flask);
    }

    auto emptyFlasks = get<2>(trap);
    set<uint64_t> available;

    for (auto e: emptyFlasks){
        available.insert(e);
    }

    while (true){
        set<uint64_t> newFlasks;

        for (auto e1: available){
            for (auto e2: available){
                if (e1 == e2){
                    continue;
                }

                newFlasks.insert(lcm(e1, e2));
            }
        }

        size_t before = available.size();
        available.insert(newFlasks.begin(), newFlasks.end());
        if (available.size() == before){
            break;
        }
    }

    vector<uint64_t> availableSorted(available.begin(), available.end());
    sort(availableSorted.begin(), availableSorted.end());

    return defusableSteps(sum.denom, availableSorted);
}

int main(){
    auto traps = readFile("inputs/33_trap_water.txt");

    auto res = traps |
               ranges::views::filter(defusable) |
               ranges::views::transform([](auto a){ return get<0>(a); });

    cout << reduce(res.begin(), res.end()) << endl;

    return 0;
}
