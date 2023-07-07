#pragma once
#include "Player.h"
#include "Game.h"
class StateNode
{
	// a node representing all of the state of a game and its players, used in monte carlo tree search
public:
	StateNode(StateNode* parent, MovePos move);
	std::vector<StateNode> getChildren();
	int currentPlayer;
	Game game;

};

