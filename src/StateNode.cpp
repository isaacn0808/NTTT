#include "StateNode.h"
StateNode::StateNode(StateNode* parent, MovePos move): 
    currentPlayer(1 - parent->currentPlayer), game(parent->game, move){}




int main(){
    return 0;
}