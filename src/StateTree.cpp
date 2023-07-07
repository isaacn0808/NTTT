#include "StateTree.h"
#include <algorithm>
void StateTree::expand(StateNode& node)
{

}

StateNode& StateTree::UCBTSelect(StateNode& node, float explorationConstant = sqrt(2.0f))
{
    if (!childMap.contains(node))
    {
        throw std::out_of_range("UCBTSelect: node is a leaf node, cannot find children to select");
        return;
    }
    std::vector<StateNode> childNodes = childMap.at(node);

    auto UCBT
    {
        []{}
    };
    auto maxIter = std::max_element(childNodes.begin(), childNodes.end(), [=] (const StateNode& child) -> bool
    {
        float exploit = Q.at(child) / (float) N.at(child);
        float explore = explorationConstant * sqrt( log(N.at(node)) / N.at(child));
        return exploit + explore;
    });
    int i = std::distance(childNodes.begin(), maxIter);
    return childNodes[i];
}