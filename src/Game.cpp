#include "Game.h"
#include "Board.h"
#include <cmath>
#include <random>
//constructor
Game::Game() : lastMove{-1, -1, -1}{  
    }

//finds available moves within a 3x3 box and adds them to the main vector of available moves
void Game::addBoxAvailableMoves(MovePos box, std::vector<MovePos>& vec){
    const int boxStartX = 3 * box.x;
    const int boxStartY = 3 * box.y;
    for (int y = boxStartY; y < boxStartY + 3; ++y){
        for (int x = boxStartX; x < boxStartX + 3; ++x){
            if (board.asFull[y][x] == -1){
                // when an empty cell is found, push a MovePos representing that cell to the main availableMoves vector
                vec.push_back({x, y, -1});
            }
        }
    }
}
//finds available moves given the last move of the game
std::vector<MovePos> Game::getAvailableMoves(){
    // x and y coords of next box (given that it is not filled)
    const int boxX = lastMove.x % 3;
    const int boxY = lastMove.y % 3;
    std::vector<MovePos> availableMoves;
    // if game has just started, we already have a const vector for the available moves at the beginning
    if (lastMove.type == -1){
        return STARTING_MOVES;
    }
    // if next box is not filled, find all available moves within the box
    else if (board.as3x3[boxY][boxX] == -1){
        addBoxAvailableMoves({boxX, boxY, -1}, availableMoves);
        return availableMoves;
    }
    // if there are free moves, then run addBoxAvailableMoves for every box that isn't filled
    else {
        for (int i = 0; i < 3; ++i){
            for (int j = 0; j < 3; ++j){
                if (board.as3x3[i][j] == -1){
                    addBoxAvailableMoves({j, i, -1}, availableMoves);
                }
            }
        }
    }
    return availableMoves;
}

MovePos Game::chooseRandomMove(){
    std::vector<MovePos> avMoves = getAvailableMoves();
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, avMoves.size() - 1);
    int randomIndex = dis(gen);
    return avMoves[randomIndex];
}