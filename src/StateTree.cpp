#include "StateTree.h"
#include <algorithm>
void StateTree::expand(StateNode& node)
{
    const std::vector<MovePos> avMoves = node.game.getAvailableMoves();
    std::vector<StateNode> childNodes;
    childNodes.reserve(avMoves.size());
    for (MovePos m : avMoves)
    {
        childNodes.emplace_back(&node, m);
    }
    childMap.insert({node.ID, std::move(childNodes)});
}

int StateTree::UCTSelect(StateNode& node)
{
    std::vector<StateNode> childNodes = childMap.at(node.ID);

    auto UCT
    {
        [=] (const StateNode& child) -> float 
        {
            float exploit = -child.Q / (float) child.N;
            float explore = branchingFactor * sqrt( log(node.N) / child.N);
            return exploit + explore;  
        }
    };
    auto maxIter = std::max_element(childNodes.begin(), childNodes.end(), [=] (const StateNode& a, const StateNode& b) -> bool
    {
        return UCT(a) < UCT(b);
    });
    //std::cout << UCT(*maxIter) << '\n';
    int i = std::distance(childNodes.begin(), maxIter);
    return i;
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
        if (!childMap.contains(nodeptr->ID))
        {
            return path;
        }
        // If this node does have children, it isn't a leaf node. Then, we can run UCTSelect to choose the optimal child node
        // If all child nodes are unexplored, UCTSelect just chooses the first one
        nodeptr = &childMap[nodeptr->ID][UCTSelect(*nodeptr)];
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

int main(){
    int i = 0;
    Game g;
  //  g.move({4,4,-1});
    StateNode root(g, 0);
    StateNode* rootptr = &root;
    StateTree tree{0.5};
    while(i++ < 100000){
    tree.search(*rootptr);
   // std::cout << "------\n";
    }
    std::vector<StateNode> children = tree.childMap[0];
    for(StateNode node : children){
        node.print();
        node.game.lastMove.print();
    }
    std::cout << tree.childMap.size() << '\n';
    std::cout << StateNode::objectCount << '\n';
}