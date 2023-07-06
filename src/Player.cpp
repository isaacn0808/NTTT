#include "Player.h"

using namespace std::chrono;
void Player::move(MovePos pos, Game* gameptr) {
    const MovePos moved = { pos.x, pos.y, type };
    gameptr->board.move(moved);
    gameptr->lastMove = moved;
}

int randomPlayout(Game& game, Player& p1, Player& p2) {
    Game gameCopy(game);
    while (gameCopy.board.checkWin() == -1) {
        p1.move(gameCopy.chooseRandomMove(), &gameCopy);
        if (gameCopy.board.checkWin() != -1) {
            return game.board.checkWin();
        }
        p2.move(gameCopy.chooseRandomMove(), &gameCopy);
    }
    return gameCopy.board.checkWin();
}
namespace Strategy {
static std::mutex evalMutex;

void asyncPlayout(MovePos move, Game* game, Player* otherPlayer, Player* thisPlayer, float* bestEval, int* bestIndex, int i, int count) {
    int win, loss, draw;
    win = loss = draw = 0;
    Game gamecpy(*game);
    thisPlayer->move(move, &gamecpy);
    for (int k = 0; k < count; ++k) {
        const int result = randomPlayout(gamecpy, *otherPlayer, *thisPlayer);
        if (result == 2) {
            ++draw;
        }
        else if (result == thisPlayer->type) {
            ++win;
        }
        else {
            ++loss;
        }
    }
    const float eval = 100 * (win + ((float)draw / 2)) / count;
    std::lock_guard<std::mutex> lk(evalMutex);
    if (eval > *bestEval) {
        *bestEval = eval;
        *bestIndex = i;
    }
}

MovePos simpleMCEvalasync(Game& game, Player& thisPlayer, Player& otherPlayer, int count) {
    std::vector<MovePos> avMoves = game.getAvailableMoves();
    int moveLen = avMoves.size();
    float bestEval = 0.0f;
    int bestIndex = 0;
    std::vector<std::future<void>> asyncFutures;
    for (int i = 0; i < moveLen; ++i) {
        asyncFutures.push_back(std::async(std::launch::async, &asyncPlayout, avMoves[i], &game, &otherPlayer, &thisPlayer, &bestEval, &bestIndex, i, count));
    }
    for (int i = 0; i < moveLen; ++i) {
        asyncFutures[i].wait();
    }
    return avMoves[bestIndex];

}

MovePos simpleEpsilonGreedy(Game& game, Player& thisPlayer, Player& otherPlayer, int count, double epsilon){
    std::vector<MovePos> avMoves = game.getAvailableMoves();
    const int moveLen = avMoves.size();
    std::vector<float> evals(moveLen, 1.0f);
    std::vector<int> counts(moveLen, 0);
    int k = 0;
    srand(time(NULL));
    while (k < moveLen * count) {
        int i = 0;
        float r = (float) rand() / RAND_MAX;
        if (r < epsilon) {
            i = rand() % moveLen;
        }
        else {
            auto maxIter = std::max_element(evals.begin(), evals.end());
            i = std::distance(evals.begin(), maxIter);
        }
        thisPlayer.move(avMoves[i], &game);
        const int playResult = randomPlayout(game, otherPlayer, thisPlayer);
        game.board.move(avMoves[i]);
        float result;
        if (playResult == 2) {
            result = 0.5f;
        }
        else if (playResult == thisPlayer.type) {
            result = 1.0f;
        }
        else {
            result = 0.0f;
        }
        counts[i]++;
        evals[i] += ((result - evals[i]) / counts[i]);
        k++;
    }
    auto maxIter = std::max_element(evals.begin(), evals.end());
    int i = std::distance(evals.begin(), maxIter);
    return avMoves[i];
}

};


int main() {
    auto start = high_resolution_clock::now();
    int w = 0; int l = 0; int d = 0;
        Game g;
        Player p1(0);
        Player p2(1);
        p1.move(Strategy::simpleMCEvalasync(g, p1, p2, 1000), &g);
       /*while (g.board.checkWin() == -1) {
            p1.move(Strategy::simpleMCEvalasync(g, p1, p2, 1000), &g);
            if (g.board.checkWin() != -1) {
                break;
            }
            p2.move(Strategy::simpleMCEvalasync(g, p1, p2, 1000), &g);
        }*/
    auto end = high_resolution_clock::now();
    std::cout << duration_cast<milliseconds>(end - start).count() << '\n';
    std::cin.get();
    return 0;
}