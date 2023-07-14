#pragma once
#include "Game.h"
class StateNode
{
	// a node representing all of the state of a game and its players, used in monte carlo tree search
public:
	StateNode(Game g, int currentPlayer);
	StateNode(StateNode* parent, MovePos move);
	bool operator==(const StateNode& other) const;
	void print();
	static int objectCount;
	int ID;
	float Q; // current reward node has gained
	int N; // number of times node has been selected (starts at 1 so we don't get divide by zero errors when evaluating UCT)
	int currentPlayer;
	Game game;

};

