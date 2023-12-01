#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <regex>
#include <ranges>
#include <map>
#include <numeric>
#include <algorithm>

using namespace std;

vector<tuple<int, int>> readTraps(const string& file) {
    regex rgx("([0-9]+), ([0-9]+):");
    vector<tuple<int, int>> traps;

    ifstream filestream(file);

    string line;
    while(getline(filestream, line)) {
        smatch matches;
        if (regex_search(line, matches, rgx)) {
            string block1 = matches[1].str();
            string block2 = matches[2].str();
            tuple<int, int> trap(stoi(block1), stoi(block2));
            traps.emplace_back(trap);
        }
    }

    return traps;
}

int main(){
    auto traps = readTraps("inputs/destruction_traps.txt");
    map<int, vector<int>> trapMap;

    for (auto& t: traps){
        if (!trapMap.contains(get<0>(t))){
            trapMap[get<0>(t)] = vector<int>();
        }

        trapMap[get<0>(t)].emplace_back(get<1>(t));
    }

    auto kv = views::keys(trapMap);
    int lastStep = *max_element(kv.begin(), kv.end());

    vector<int> available;
    vector<int> exploded;

    for (int t = lastStep; t > 0; t--){
        if (trapMap.contains(t)){
            available.insert(available.end(), trapMap[t].begin(), trapMap[t].end());
            ranges::sort(available, ranges::less{});
        }

        if (!available.empty()){
            exploded.emplace_back(available[available.size() - 1]);
            available.pop_back();
        }
    }

    cout << reduce(exploded.begin(), exploded.end()) << endl;

    return 0;
}
