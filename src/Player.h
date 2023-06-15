#pragma once
#include "Game.h"
class Player{
    public:
        int type;
        Game* const gameptr;
        Player(Game* gptr, int type) : gameptr(gptr), type(type) {};
        void move(MovePos pos);
        MovePos chooseRandomMove();
        //virtual MovePos chooseMove();
};

/*  class RandomPlayer : Player{
    MovePos chooseMove();
};*/