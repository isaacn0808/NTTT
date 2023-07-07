#pragma once
#include <unordered_map>
#include "StateNode.h"
class StateTree
{
    public:
        std::unordered_map<StateNode, std::vector<StateNode>> childMap;
        std::unordered_map<StateNode, float> Q;
        std::unordered_map<StateNode, int> N;
        void expand(StateNode& node);
        StateNode& UCBTSelect(StateNode& node, float explorationConstant = sqrt(2.0f));

};

