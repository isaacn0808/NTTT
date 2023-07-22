#pragma once
#include <iostream>
#include <cmath>
        //rep. a move done on the board and its position
struct MovePos {
        MovePos(int x, int y, int type = -1) : x(x), y(y), type(type){}
        MovePos(int x1, int y1, int x2, int y2, int type = -1) : x(3 * x1 + x2), y(3 * y1 + y2), type(type){}
        int x;
        int y;
        int type;
        void print() {
            std::cout << (int)floor(x / 3) << " " << (int)floor(y / 3) << " " << x % 3 << " " << y % 3 << " " << type << '\n';
        }
    };
class Board {
    private:
        char numToChar(int num);
    public:
        //rep. state of each nested TTT board
        int as3x3[3][3] = {
            {-1, -1, -1},
            {-1, -1, -1},
            {-1, -1, -1}
        };
        int asFull[9][9] = {
            {-1, -1, -1, -1, -1, -1, -1, -1, -1},
            {-1, -1, -1, -1, -1, -1, -1, -1, -1},
            {-1, -1, -1, -1, -1, -1, -1, -1, -1},
            {-1, -1, -1, -1, -1, -1, -1, -1, -1},
            {-1, -1, -1, -1, -1, -1, -1, -1, -1},
            {-1, -1, -1, -1, -1, -1, -1, -1, -1},
            {-1, -1, -1, -1, -1, -1, -1, -1, -1},
            {-1, -1, -1, -1, -1, -1, -1, -1, -1},
            {-1, -1, -1, -1, -1, -1, -1, -1, -1}
        };
        int checkFilled(MovePos move);
        int checkWin();
        void move(MovePos move);
        void print();
    };


