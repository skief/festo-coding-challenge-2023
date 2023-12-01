#include <iostream>
#include <utility>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <numeric>
#include <algorithm>
using namespace std;

const string DEACTIVATE[] = {
        "inactive",
        "disabled",
        "quiet",
        "standby",
        "idle"
};

const string ACTIVATE[] = {
        "live",
        "armed",
        "ready",
        "primed",
        "active"
};

const string TOGGLE[] = {
        "flipped",
        "toggled",
        "reversed",
        "inverted",
        "switched"
};


vector<int> solve(const string& file){
    vector<int> res;
    ifstream filestream(file);

    string line;
    while(getline(filestream, line)){
        size_t idSep = line.find(": ");

        stringstream ss(line.substr(idSep + 2));
        string action;
        bool state = false;
        while(getline(ss, action, ' ')){
            if (find(begin(DEACTIVATE), end(DEACTIVATE), action) != end(DEACTIVATE)){
                state = false;
            } else if (find(begin(ACTIVATE), end(ACTIVATE), action) != end(ACTIVATE)){
                state = true;
            } else if (find(begin(TOGGLE), end(TOGGLE), action) != end(TOGGLE)){
                state = !state;
            } else {
                cout << "Error " << action << endl;
            }
        }

        if (!state){
            res.push_back(stoi(line.substr(0, idSep)));
        }
    }

    return res;
}


int main() {
    vector<int> safe = solve("inputs/03_trap_logs.txt");

    cout << reduce(safe.begin(), safe.end()) << endl;

    return 0;
}
