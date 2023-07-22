#include "StateTree.h"
#include "StateNode.h"
#include <algorithm>
#include <memory>
void StateTree::expand(StateNode& node)
{
    const std::vector<MovePos> avMoves = node.game.getAvailableMoves();
    node.children.reserve(avMoves.size());
    for (MovePos m : avMoves)
    {
        node.children.push_back(std::move(std::make_unique<StateNode>(&node, m)));
    }
}


float StateTree::UCT(StateNodePtr& child, StateNode& node)
{
            float exploit = -child->Q / (float) child->N;
            float explore = StateTree::branchingFactor * sqrt( log(node.N) / child->N);
            return exploit + explore;  
}

int StateTree::UCTSelect(StateNode& node)
{
   std::vector<StateNodePtr>* childNodes = &node.children;

   /*  auto UCT
    {
        [=] (StateNodePtr& child) -> float
        {
            
        }
    }; */
    
    float maxUCT = -999999;
    int bestIndex = 0;
    int index = 0;
    for(auto iter = childNodes->begin(); iter != childNodes->end(); iter++) {
        const float currUCT = UCT(*iter, node);
        if (currUCT > maxUCT) {
            maxUCT = currUCT;
            bestIndex = index;
        }
        ++index;
    }
    return bestIndex;
}

std::vector<StateNode*> StateTree::selectPath(StateNode& node)
{
    StateNode* nodeptr = &node;
    const int MAX_DEPTH = 16;
    std::vector<StateNode*> path;
    path.reserve(MAX_DEPTH);
    while(true)
    {
        // Add current node to the path
       // nodeptr->print();
        path.push_back(nodeptr);
        // First check whether the node is expanded or not by searching for it in the childMap - if it is not expanded, we stop the search here
        // We assume that we will never go as far deep as reaching a terminal state, so there is no need to check for a win (it costs extra time)
        if (nodeptr->children.size() == 0 || nodeptr->game.board.checkWin() != -1)
        {
            return path;
        }
        // If this node does have children, it isn't a leaf node. Then, we can run UCTSelect to choose the optimal child node
        // If all child nodes are unexplored, UCTSelect just chooses the first one
        nodeptr = nodeptr->children[UCTSelect(*nodeptr)].get();
    }
}

void StateTree::backprop(std::vector<StateNode*>& path, float reward)
{
    for (StateNode* node: path)
    {
        if (reward == 2)
        {
            node->Q += 0.5;
            ++node->N;
            continue;
        }
        node->Q += (node->currentPlayer == (int)reward ? 1 : 0);
        ++node->N;
    }
}

void StateTree::search(StateNode& node)
{
    std::vector<StateNode*> pathToLeaf = selectPath(node);
    StateNode* leaf = pathToLeaf.back();
    expand(*leaf);
    float reward = randomPlayout(leaf->game);
    backprop(pathToLeaf, reward);
}
