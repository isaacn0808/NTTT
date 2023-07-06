#pragma once
#include "Board.h"
#include <random>
#include <chrono>
#include <future>
#include <ctime>
#include <algorithm>
#include "Game.h"
class Player {
public:
    int type;
    Player(int type) : type(type) {};
    void move(MovePos pos, Game* gameptr);

};

int randomPlayout(Game& oldGame, Player& p1, Player& p2);
