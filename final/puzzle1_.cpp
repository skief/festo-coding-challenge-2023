#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <set>
#include <tuple>
#include <execution>
using namespace std;

#define UNCHARGED 'O'
#define CHARGED 'X'
#define EMPTY '.'

enum Direction{
    RIGHT,
    DOWN,
    LEFT,
    UP
};

class Battery{
public:
    vector<string> map;

    size_t y;
    size_t x;
    Direction dir;

    bool sensor;

    bool allCharged(){
        for (const auto& row: map){
            for (auto c: row){
                if (c == UNCHARGED){
                    return false;
                }
            }
        }

        return true;
    };

    void rotate(){
        if (dir == UP) {
            dir = RIGHT;
        } else if (dir == LEFT) {
            dir = UP;
        } else if (dir == DOWN) {
            dir = LEFT;
        } else if (dir == RIGHT) {
            dir = DOWN;
        }
    };

    void chargingPush(int newY, int newX){
        int deltaX = 0;
        int deltaY = 0;
        size_t h = map.size();
        size_t w = map[0].size();

        if (dir == UP) {
            deltaY = -1;
        } else if (dir == LEFT) {
            deltaX = -1;
        } else if (dir == DOWN) {
            deltaY = 1;
        } else if (dir == RIGHT) {
            deltaX = 1;
        }

        int nextX = newX;
        int nextY = newY;
        bool charge = false;

        while (true){
            nextX += deltaX;
            nextY += deltaY;
            if (nextX < 0 || nextY < 0 || nextX >= w || nextY >= h) {
                // wall
                charge = true;
                nextX -= deltaX;
                nextY -= deltaY;
                break;
            } else if (map[nextY][nextX] == EMPTY){
                // push
                break;
            } else if (map[nextY][nextX] == CHARGED){
                // charge
                charge = true;
                break;
            }
        }

        for (int pY = min(newY, nextY); pY <= max(newY, nextY); pY++){
            for (int pX = min(newX, nextX); pX <= max(newX, nextX); pX++){
                if (charge){
                    map[pY][pX] = CHARGED;
                } else {
                    map[pY][pX] = UNCHARGED;
                }
            }
        }
        if (!charge){
            map[newY][newX] = EMPTY;
            y = newY;
            x = newX;
        } else {
            sensor = true;
        }
    };

    bool normalStep(){
        size_t h = map.size();
        size_t w = map[0].size();

        int newX = (int) x;
        int newY = (int) y;

        if (dir == UP) {
            newY -= 1;
        } else if (dir == LEFT) {
            newX -= 1;
        } else if (dir == DOWN) {
            newY += 1;
        } else if (dir == RIGHT) {
            newX += 1;
        }

        if (newX < 0 || newY < 0 || newX >= w || newY >= h || map[newY][newX] == CHARGED){
            rotate();
        } else if (map[newY][newX] == EMPTY){
            x = newX;
            y = newY;
        } else if (map[newY][newX] == UNCHARGED){
            chargingPush(newY, newX);
            return true;
        }

        return false;
    }

    bool sensorScan(){
        int deltaX = 0;
        int deltaY = 0;
        size_t h = map.size();
        size_t w = map[0].size();

        if (dir == UP) {
            deltaX = 1;
        } else if (dir == LEFT) {
            deltaY = -1;
        } else if (dir == DOWN) {
            deltaX = -1;
        } else if (dir == RIGHT) {
            deltaY = 1;
        }

        int nextX = (int) x;
        int nextY = (int) y;
        bool found = false;

        while (true){
            nextX += deltaX;
            nextY += deltaY;
            if (nextX < 0 || nextY < 0 || nextX >= w || nextY >= h) {
                break;
            } else if (map[nextY][nextX] == UNCHARGED){
                found = true;
                break;
            } else if (map[nextY][nextX] == CHARGED){
                break;
            }
        }

        if (found){
            rotate();
            sensor = false;
        } else {
            return normalStep();
        }

        return false;
    };

    bool step(){
        if (sensor){
            return sensorScan();
        } else {
            return normalStep();
        }
    };
};

vector<Battery> readBatteries(const string& file){
    ifstream filestream(file);
    string line;

    vector<Battery> batteries;

    while (getline(filestream, line))
    {
        if (line[0] == '('){
            batteries.emplace_back();
            batteries[batteries.size() - 1].x = 0;
            batteries[batteries.size() - 1].y = 0;
            batteries[batteries.size() - 1].dir = RIGHT;
            batteries[batteries.size() - 1].sensor = true;
            continue;
        }

        if (!line.empty()){
            if (line[0] == '>'){
                line[0] = EMPTY;
            }
            batteries[batteries.size() - 1].map.emplace_back(line);
        }
    }

    return batteries;
}


int main() {
    vector<Battery> batteries = readBatteries("inputs/batteries.txt");
    //readBatteries("inputs/batteries.txt");

    /*vector<size_t> indices;
    for (size_t i = 0; i < batteries.size(); i++){
        indices.emplace_back(i);
    }

    std::for_each(execution::par_unseq, indices.begin(), indices.end(), [&](auto& i){
        auto b = batteries[i];

        set<tuple<size_t, size_t, Direction>> known;
        while (1){
            if (b.step()){
                known.clear();
            } else if (b.sensor) {
                tuple<size_t, size_t, Direction> state(b.x, b.y, b.dir);
                if (known.contains(state)){
                    break;
                }
                known.insert(state);
            }
        }
        if(b.allCharged()) {
            cout << i + 1 << endl;
        }
    });*/

    for (size_t i = 0; i < batteries.size(); i++){
        auto b = batteries[i];

        set<tuple<size_t, size_t, Direction>> known;
        while (true){
            if (b.step()){
                known.clear();
            } else if (b.sensor) {
                tuple<size_t, size_t, Direction> state(b.x, b.y, b.dir);
                if (known.contains(state)){
                    break;
                }
                known.insert(state);
            }
        }

        if(b.allCharged()) {
            cout << i + 1 << endl;
        }
    }

    return 0;
}