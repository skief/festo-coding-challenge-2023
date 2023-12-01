#include <iostream>
#include <vector>
#include <tuple>
#include <fstream>
#include <string>
#include <algorithm>

#define MAXSIZE 100

using namespace std;

const int NEIGH[2][6][2] = {{
    {0, -1},
    {1, -1},
    {-1, 0},
    {1, 0},
    {0, 1},
    {1, 1}
    },
{
    {-1, -1},
    {0, -1},
    {-1, 0},
    {1, 0},
    {-1, 1},
    {0, 1}
}};

void fill(int bestagon[][MAXSIZE], vector<tuple<size_t, size_t>> lookup){
    int val = 1;

    while (!lookup.empty()){
        vector<tuple<size_t, size_t>> next;

        for (auto& pos: lookup){
            auto row = get<0>(pos);
            auto col = get<1>(pos);

            bool allGrEq = true;
            for (auto neigh: NEIGH[row % 2]){
                int y = (int)row + neigh[1];
                int x = (int)col + neigh[0];

                if (x < 0 || y < 0 || MAXSIZE <= y || MAXSIZE <= x ||
                    bestagon[y][x] < val || bestagon[y][x] - val > 1){
                    allGrEq = false;
                }
            }

            if (allGrEq){
                bestagon[row][col] += 1;

                next.emplace_back(row, col);
            }
        }

        swap(lookup, next);

        val += 1;
    }
}

int sum(int bestagon[][MAXSIZE]){
    int res = 0;

    for (size_t row = 0; row < MAXSIZE; row++){
        for (size_t col = 0; col < MAXSIZE; col++){
            res += bestagon[row][col];
        }
    }

    return res;
}

vector<int> readBestagons(const string& file){
    vector<int> results;
    int bestagons[MAXSIZE][MAXSIZE];

    vector<tuple<size_t, size_t>> positions;

    ifstream filestream(file);

    string line;
    size_t off = 1;
    bool first = true;
    bool pad = false;

    for (auto& r: bestagons){
        for (auto& val: r){
            val = 0;
        }
    }

    size_t row = 0;

    while(getline(filestream, line)){
        if (line.size() == 0){
            fill(bestagons, positions);
            results.emplace_back(sum(bestagons));

            off = 1;
            positions.clear();
            first = true;
            pad = false;
            row = 0;

            for (auto& r: bestagons){
                for (auto& val: r){
                    val = 0;
                }
            }

            continue;
        }

        if (first && line.size() > 0){
            for (off = 0; off < line.size(); off++){
                if (line[off] == '<'){
                    off = off % 2;
                    break;
                }
            }

            if (off == 0){
                pad = true;
            }

            off = 1;
            first = false;
        }

        if (pad){
            line = " " + line;
        }
        line = line.substr(off);

        for (size_t i = 0; i < line.size() / 2; i++){
            if (line[2 * i] == '<' && line[2 * i + 1] == '>'){
                bestagons[row][i] = 1;
                positions.emplace_back(row, i);
            }
        }
        row++;

        off = 1 - off;
    }

    fill(bestagons, positions);
    results.emplace_back(sum(bestagons));

    return results;
}

int main(){
    auto results = readBestagons("inputs/tray_shapes.txt");

    cout << *max_element(results.begin(), results.end()) << endl;
    return 0;
}
