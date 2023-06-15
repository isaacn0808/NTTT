#pragma once
#include <iostream>
//rep. a move done on the board and its position
struct MovePos{
    int x;
    int y;
    int type;
    void print(){
        std::cout << x << " " << y << " " << type << '\n';
    }
};

class Board{
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