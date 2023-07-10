#pragma once
#include <unordered_map>
#include "StateNode.h"
class StateTree
{
    public:
        StateTree(float branchingFactor): branchingFactor(branchingFactor){}
        float branchingFactor;
        std::unordered_map<int, std::vector<StateNode>> childMap; //to access a node's children, its index is its ID
        void expand(StateNode& node);
        int UCTSelect(StateNode& node);
        std::vector<StateNode*> selectPath(StateNode& node);
        void backprop(std::vector<StateNode*>& path, float reward);
        void search(StateNode& node);

};

