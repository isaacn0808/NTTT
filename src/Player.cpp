#include "Player.h"
#include "Board.h"
#include <random>
#include <chrono>
using namespace std::chrono;
void Player::move(MovePos pos, Game* gameptr){
    const MovePos moved = {pos.x, pos.y, type};
    gameptr->board.move(moved);
    gameptr->lastMove = moved;
}

int randomPlayout(Game& game, Player& p1, Player& p2){
    Game gameCopy(game);
    while (gameCopy.board.checkWin() == -1){
        p1.move(gameCopy.chooseRandomMove(), &gameCopy);
        if (gameCopy.board.checkWin() != -1){
            return game.board.checkWin();
        }
        p2.move(gameCopy.chooseRandomMove(), &gameCopy);
    }
    return gameCopy.board.checkWin();
}

MovePos Strategy::simpleMCEval(Game& game, Player& thisPlayer, Player& otherPlayer, int count){
    std::vector<MovePos> avMoves = game.getAvailableMoves();
    const int moveLen = avMoves.size();
    int bestEval = 0; 
    int bestIndex = 0;
    for (int i = 0; i < moveLen; ++i){
        thisPlayer.move(avMoves[i], &game);
        int win, loss, draw;
        win = loss = draw = 0;
        for (int k = 0; k < count; ++k){
            const int result = randomPlayout(game, otherPlayer, thisPlayer);
            if (result == 2){
                ++draw;
            }
            else if (result == thisPlayer.type){
                ++win;
            }
            else {
                ++loss;
            }
        }
        const float eval = 100 * (win + ( (float) draw / 2)) / moveLen;
        if (eval > bestEval){
            bestEval = eval;
            bestIndex = i;
        }
        game.board.move(avMoves[i]);
    }
    return avMoves[bestIndex];
}

MovePos simpleMCEvalWorst(Game& game, Player& thisPlayer, Player& otherPlayer, int count){
    std::vector<MovePos> avMoves = game.getAvailableMoves();
    const int moveLen = avMoves.size();
    int bestEval = 10000; 
    int bestIndex = 0;
    for (int i = 0; i < moveLen; ++i){
        thisPlayer.move(avMoves[i], &game);
        int win, loss, draw;
        win = loss = draw = 0;
        for (int k = 0; k < count; ++k){
            const int result = randomPlayout(game, otherPlayer, thisPlayer);
            if (result == 2){
                ++draw;
            }
            else if (result == thisPlayer.type){
                ++win;
            }
            else {
                ++loss;
            }
        }
        const float eval = 100 * (win + ( (float) draw / 2)) / moveLen;
        if (eval < bestEval){
            bestEval = eval;
            bestIndex = i;
        }
        game.board.move(avMoves[i]);
    }
    return avMoves[bestIndex];
}

int main(){
    Game g;
    Player p1(0);
    Player p2(1);
    int k = 0;
    while (g.board.checkWin() == -1){
        p1.move(Strategy::simpleMCEval(g, p1, p2, 5000), &g);
        ++k;
        g.board.print();
        if (g.board.checkWin() != -1){
            break;
        }
        p2.move(simpleMCEvalWorst(g, p2, p1, 5000), &g);
        ++k;
        g.board.print();
    }
    std::cout << k;

}


