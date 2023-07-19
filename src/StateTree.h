#pragma once
#include <unordered_map>
#include "StateNode.h"
class StateTree
{
    public:
        StateTree(float branchingFactor): branchingFactor(branchingFactor){}
        float branchingFactor;
        static int expandCallCount;
        static int randomPlayoutCallCount;
        void expand(StateNode& node);
        int UCTSelect(StateNode& node);
        std::vector<StateNode*> selectPath(StateNode& node);
        void backprop(std::vector<StateNode*>& path, float reward);
        void search(StateNode& node);
        float UCT(StateNodePtr& child, StateNode& node);

};

