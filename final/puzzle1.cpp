#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <tuple>
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
    string map[25];

    size_t y;
    size_t x;
    Direction dir;

    size_t w;
    size_t h;
    int deltaX;
    int deltaY;

    bool sensor;

    void setDelta(){
        if (dir == UP) {
            deltaX = 0;
            deltaY = -1;
        } else if (dir == LEFT) {
            deltaX = -1;
            deltaY = 0;
        } else if (dir == DOWN) {
            deltaX = 0;
            deltaY = 1;
        } else if (dir == RIGHT) {
            deltaX = 1;
            deltaY = 0;
        }
    }

    void calcSize(){
        w = map[0].size();

        setDelta();
    }

    bool allCharged(){
        for (size_t row = 0; row < h; row++){
            for (size_t col = 0; col < w; col++){
                if (map[row][col] == UNCHARGED){
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

        setDelta();
    };

    void chargingPush(int newY, int newX){
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
        int newX = (int) x;
        int newY = (int) y;

        while (true){
            newX = newX + deltaX;
            newY = newY + deltaY;

            if (newX < 0 || newY < 0 || newX >= w || newY >= h || map[newY][newX] == CHARGED){
                x = newX - deltaX;
                y = newY - deltaY;

                rotate();
                break;

            } else if (sensor && map[newY][newX] == EMPTY){
                x = newX;
                y = newY;
                break;

            } else if (map[newY][newX] == UNCHARGED){
                x = newX - deltaX;
                y = newY - deltaY;

                chargingPush(newY, newX);
                return true;
            }

        }

        return false;
    }

    bool sensorScan(){
        int sensorDeltaX = -deltaY;
        int sensorDeltaY = deltaX;

        int nextX = (int) x;
        int nextY = (int) y;
        bool found = false;

        while (true){
            nextX += sensorDeltaX;
            nextY += sensorDeltaY;
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
            x = nextX - sensorDeltaX;
            y = nextY - sensorDeltaY;
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

    void print(){
        for (size_t row = 0; row < h; row++){
            for (size_t col = 0; col < w; col++){
                if (row == y && col == x){
                    if (dir == UP) {
                        cout << "^";
                    } else if (dir == LEFT) {
                        cout << "<";
                    } else if (dir == DOWN) {
                        cout << "v";
                    } else if (dir == RIGHT) {
                        cout << ">";
                    }
                } else {
                    cout << map[row][col];
                }
            }
            cout << endl;
        }
        cout << endl;
    };

    bool chargeable(){
        calcSize();

        set<int> known;
        while (true){
            //b.print();

            if (step()){
                known.clear();
            } else if (sensor && dir == RIGHT) {
                int state = (x & 0xff) | ((y & 0xff) << 8);

                if (!get<1>(known.insert(state))){
                    break;
                }

            }
        }

        return allCharged();
    }
};

void readAndSolve(const string& file){
    ifstream filestream(file);
    string line;

    Battery battery;
    size_t i = 0;
    while (getline(filestream, line))
    {
        if (line[0] == '('){
            if (i > 0){
                if (battery.chargeable()){
                    cout << i << endl;
                }
            }

            battery.x = 0;
            battery.y = 0;
            battery.dir = RIGHT;
            battery.sensor = true;
            battery.h = 0;

            i++;
            continue;
        }

        if (!line.empty()){
            if (line[0] == '>'){
                line[0] = EMPTY;
            }

            battery.map[battery.h] = line;
            battery.h++;
        }
    }

    if (battery.chargeable()){
        cout << i << endl;
    }
}

int main() {
    readAndSolve("inputs/batteries.txt");

    return 0;
}