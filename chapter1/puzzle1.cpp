#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <map>
#include <string>

#include "keyTools.h"

using namespace std;


class Recipe{
public:
    vector<pair<int, int>> steps;

    void addStep(const string& step){
        size_t split = step.find(", ");
        int hammer = stoi(step.substr(1, split - 1));
        int pos = stoi(step.substr(split + 2, step.size() - split + 2 - 1));

        steps.push_back(pair<int, int>(hammer, pos - 1));
    }

    Recipe(const string& line){
        size_t start = 0;
        size_t end;

        while ((end = line.find(" - ", start)) != string::npos){
            addStep(line.substr(start, end - start));
            start = end + 3;
        }

        addStep(line.substr(start));
    };

    string apply(const map<int, Hammer>& hammers){
        string key = "A";

        for (auto step: steps){
            if (!hammers.contains(step.first)){
                return "";
            }

            Hammer hammer = hammers.at(step.first);
            int pos = step.second;
            if (pos > key.size() - 1){
                return "";
            }
            if (key[pos] != hammer.src){
                return "";
            }

            if (pos != 0 && pos != key.length() - 1){
                key = key.substr(0, pos) + hammer.target + key.substr(pos + 1);
            } else if (pos == 0){
                key = hammer.target + key.substr(1);
            } else {
                key.pop_back();
                key = key + hammer.target;
            }
        }

        return key;
    }
};


vector<Recipe> readRecipes(const string& file){
    vector<Recipe> recipes;
    ifstream filestream(file);

    string line;
    while(getline(filestream, line)){
        recipes.push_back(Recipe(line));
    }

    return recipes;
}


int main(){
    map<int, Hammer> hammers = readHammers("inputs/hammer_collection.txt");
    vector<Recipe> recipes = readRecipes("inputs/11_keymaker_recipe.txt");

    for (auto recipe: recipes){
        string key = recipe.apply(hammers);
        if (!key.empty()){
            cout << key << endl;
        }
    }

    return 0;
}
