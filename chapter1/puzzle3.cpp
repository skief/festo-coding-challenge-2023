#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>
#include <string>
#include <sstream>
#include <numeric>
#include <set>
#include <ranges>

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

vector<tuple<int, vector<uint64_t>, vector<uint64_t>>> readFile(const string& file){
    vector<tuple<int, vector<uint64_t>, vector<uint64_t>>> res;
    ifstream filestream(file);

    string line;
    while(getline(filestream, line)){
        size_t idSep = line.find(':');
        int id = stoi(line.substr(0, idSep));

        line = line.substr(idSep + 2);
        size_t sep = line.find('-');

        tuple<int, vector<uint64_t>, vector<uint64_t>> trap(id, readFlasks(line.substr(0, sep - 1)), readFlasks(line.substr(sep + 2)));
        res.push_back(trap);
    }

    return res;
}

bool equality1(const tuple<int, vector<uint64_t>, vector<uint64_t>>& trap){
    vector<uint64_t> left = get<1>(trap);
    vector<uint64_t> right = get<2>(trap);
    return left.size() == right.size();
}

bool equality2(const tuple<int, vector<uint64_t>, vector<uint64_t>>& trap){
    vector<uint64_t> left = get<1>(trap);
    vector<uint64_t> right = get<2>(trap);

    Fraction l;
    Fraction r;

    for (auto elem: left){
        l = l + Fraction(1, elem);
    }
    for (auto elem: right){
        r = r + Fraction(1, elem);
    }
    return l == r;
}

bool diversity(const tuple<int, vector<uint64_t>, vector<uint64_t>>& trap){
    vector<uint64_t> left = get<1>(trap);
    vector<uint64_t> right = get<2>(trap);
    set<uint64_t> all;
    all.insert(left.begin(), left.end());
    all.insert(right.begin(), right.end());

    return all.size() == (left.size() + right.size());
}

int main(){
    vector<tuple<int, vector<uint64_t>, vector<uint64_t>>> traps = readFile("inputs/13_trap_balance.txt");

    auto res = traps |
            ranges::views::filter(equality1) |
            ranges::views::filter(diversity) |
            ranges::views::filter(equality2) |
            ranges::views::transform([](auto a){ return get<0>(a); });

    cout << reduce(res.begin(), res.end()) << endl;

    return 0;
}
