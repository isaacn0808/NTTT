#pragma once
#include "Game.h"
class Player{
    public:
        int type;
        Player(int type) : type(type) {};
        void move(MovePos pos, Game* gameptr);
        
};

int randomPlayout(Game& oldGame, Player& p1, Player& p2);

class Strategy{
    private:
         Strategy () {}
    public:
        static MovePos simpleMCEval(Game& game, Player& thisPlayer, Player& otherPlayer, int count);
};