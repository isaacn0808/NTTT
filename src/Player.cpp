#include "Player.h"

using namespace std::chrono;
int randomPlayout(Game& game) 
{
    Game gameCopy(game);
    while (gameCopy.board.checkWin() == -1) {
        gameCopy.move(gameCopy.chooseRandomMove());
        if (gameCopy.board.checkWin() != -1) {
            return gameCopy.board.checkWin();
        }
        gameCopy.move(gameCopy.chooseRandomMove());
    }
    return gameCopy.board.checkWin();
}
namespace Strategy {
static std::mutex evalMutex;

void asyncPlayout(MovePos move, Game* game, float* bestEval, int* bestIndex, int i, int count, int type) 
{
    int win, loss, draw;
    win = loss = draw = 0;
    Game gamecpy(*game);
    gamecpy.move(move);
    for (int k = 0; k < count; ++k) {
        const int result = randomPlayout(gamecpy);
        if (result == 2) {
            ++draw;
        }
        else if (result == type) {
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

MovePos simpleMCEvalasync(Game& game, int count, int type) 
{
    std::vector<MovePos> avMoves = game.getAvailableMoves();
    int moveLen = avMoves.size();
    float bestEval = 0.0f;
    int bestIndex = 0;
    std::vector<std::future<void>> asyncFutures;
    for (int i = 0; i < moveLen; ++i) {
        asyncFutures.push_back(std::async(std::launch::async, &asyncPlayout, avMoves[i], &game, &bestEval, &bestIndex, i, count, type));
    }
    for (int i = 0; i < moveLen; ++i) {
        asyncFutures[i].wait();
    }
    return avMoves[bestIndex];

}

MovePos simpleEpsilonGreedy(Game& game, int count, double epsilon, int type)
{
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
        game.move(avMoves[i]);
        const int playResult = randomPlayout(game);
        game.remove(avMoves[i]);
        float result;
        if (playResult == 2) {
            result = 0.5f;
        }
        else if (playResult == type) {
            result = 1.0f;
        }
        else {
            result = 0.0f;
        }
        counts[i]++;
        evals[i] += ((result - evals[i]) / counts[i]);
    }
    auto maxIter = std::max_element(evals.begin(), evals.end());
    int i = std::distance(evals.begin(), maxIter);
    return avMoves[i];
}


MovePos MCTSeval(Game& game, int count, int type, float branchFactor)
{
    StateNode::objectCount = 0;
    StateNode root(game, type);
    StateTree tree(branchFactor);
    int bestIndex{};
    float bestEval = 1.0f;
    const int avLen = game.getAvailableMoves().size();
    int k{};
    while(k++ < count * avLen)
    {
        tree.search(root);
    }
    for(int i = 0; i < tree.childMap[0].size(); ++i)
    {
        StateNode node = tree.childMap[0][i];
       // node.print();
        if((node.Q / (float) node.N) < bestEval)
        {
            bestEval = node.Q / (float) node.N;
            bestIndex = i;
        }
    }
    return tree.childMap[0][bestIndex].game.lastMove;
}
};
 int main() 
{
    auto start = high_resolution_clock::now();
        Game g;
        while (g.board.checkWin() == -1) {
            g.move(Strategy::MCTSeval(g, 1000, 0, 0.5));
          //  g.board.print();
            if (g.board.checkWin() != -1) {
                break;
            }
            g.move(Strategy::MCTSeval(g, 1000, 1, 0.25));
           // g.board.print();
        }
    std::cout << g.board.checkWin() << '\n';
    auto end = high_resolution_clock::now();
    std::cout << duration_cast<milliseconds>(end - start).count() << '\n';
    std::cin.get();
}