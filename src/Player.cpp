#include "Player.h"
#include "Board.h"
#include <random>

void Player::move(MovePos pos){
    const MovePos moved = {pos.x, pos.y, type};
    gameptr->board.move(moved);
    gameptr->lastMove = moved;
}

MovePos Player::chooseRandomMove(){
    std::vector<MovePos> avMoves = gameptr->getAvailableMoves();
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, avMoves.size() - 1);
    int randomIndex = dis(gen);
    return avMoves[randomIndex];
}

int randomPlayout(){
    Game g;
    Player p1(&g, 0);
    Player p2(&g, 1);
    while (g.board.checkWin() == -1){
        p1.move(p1.chooseRandomMove());
        g.board.print();
        if (g.board.checkWin() != -1){
            return g.board.checkWin();
        }
        p2.move(p2.chooseRandomMove());
        g.board.print();
    }
    return g.board.checkWin();
}

int main(){
    randomPlayout();
}