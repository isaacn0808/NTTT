#include "StateNode.h"
#include <chrono>
#include <unordered_map>
using namespace std::chrono;
int StateNode::objectCount = 0;

StateNode::StateNode(Game g, int currentPlayer): 
    game(g), currentPlayer(currentPlayer), Q(0.0f), N(1), ID(objectCount++)
{}

StateNode::StateNode(StateNode* parent, MovePos move): 
    currentPlayer(1 - parent->currentPlayer), game(parent->game), Q(0.0f), N(1), ID(objectCount++)
    {
       // std::cout << parent->currentPlayer << '\n';
        game.move(move);
        //print();
    }

bool StateNode::operator==(const StateNode& other) const
{
    return ID == other.ID;
}

void StateNode::print()
{
    std::cout << "ID: " << ID << " Q: " << (float)Q << " N: " << N << " CURR: " << currentPlayer << " AVG: " << (Q / N) - 0.5 << '\n';
}

/* int main(){
    auto start = high_resolution_clock::now();
    int i = 0;
    Game g;
    while(i<100)
    {
        StateNode node(g, 0);
        ++i;
    }
    auto end = high_resolution_clock::now();
    std::cout << duration_cast<microseconds>(end-start).count() << '\n';
    std::cout << StateNode::objectCount << '\n';
} */