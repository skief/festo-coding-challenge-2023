#include <bitset>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <regex>
#include <fstream>
#include "decoder.h"

using namespace std;

int toInt(const string& str){
    int val = 0;
    for (size_t i = 0; i < str.length(); i++){
        if (str[i] == 'Y'){
            val = val | (1 << (str.length() - i - 1));
        }
    }
    return val;
}

string toBin(int val, size_t len){
    string res = std::bitset<32>(val).to_string();

    res = res.substr(32 - len, len);

    return res;
}

string toHex(int val){
    stringstream hexStream;
    hexStream << hex << val;

    string res = hexStream.str();

    res.erase(0, res.find_first_not_of('0'));
    if (res.length() == 0){
        res = "0";
    }

    return res;
}

string decode(const string& a, const string& b, char op, char out, bool swapped){
    string unswappedA = a;
    string unswappedB = b;

    if (swapped){
        swap(unswappedB[1], unswappedB[2]);
        swap(unswappedA[2], unswappedA[3]);
    }

    int numA = toInt(unswappedA);
    int numB = toInt(unswappedB);

    int result = 0;

    if (op == 'R'){
        result = numA | numB;
    } else if (op == 'N') {
        result = numA & numB;
    }else if (op == 'G'){
        result = numA + numB;
    } else if (op == 'L'){
        result = numA - numB;
    } else if (op == 'W'){
        result = numA * numB;
    } else if (op == 'P'){
        result = (int) round((float)numA / (float)numB);
    } else if (op == 'M'){
        if (numB != 128){
            result = numA >> numB | (numA << (8 - numB));
        } else {
            result = (numA << 1) | (numA >> 7);
        }
        result = result & 0xff;
    }

    if (out == 'Q'){
        return toBin(result, a.length());
    } else if (out == 'E'){
        return to_string(result & 0xff);
    } else if (out == 'F'){
        return toHex(result);
    }

    return "";
}

string solveTask(const string& file, bool swapped){
    regex rgx("([A-Z]+); ([A-Z]+); ([A-Z]+); ([A-Z]+);");

    ifstream filestream(file);

    string res;
    string line;
    while(getline(filestream, line)){
        smatch matches;
        if (regex_search(line, matches, rgx)){
            string block1 = matches[1].str();
            string block2 = matches[2].str();
            char op = matches[3].str()[0];
            char format = matches[4].str()[0];

            res = res + decode(block1, block2, op, format, swapped);
        }
    }
    return res;
}
