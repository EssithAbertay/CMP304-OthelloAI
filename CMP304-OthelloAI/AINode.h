#pragma once

#include <vector>
#include <stack>
#include "BoardAndAction.h"
#include "GameState.h"

class AINode
{
public:
	AINode();
	~AINode();


	AINode* Select();
	AINode* Expand();

	void Simulate(BOARD_SQUARE_STATE startingTurn);
	void Backpropagate(int reward);

	//tree functions
	AINode* FindHighestRankingChild(bool report);
	void CalcResult(BOARD_SQUARE_STATE winner);
	void resetNode();


	//setters and getters
	void setGameState(GameState newState);
	GameState getGameState() { return worldState; }

	int getRanking() { return ranking; }
	int getVisits() { return visits; }

	void setParent(AINode* newParent) { parent = newParent; }
	void setActivePlayer(BOARD_SQUARE_STATE active) { activePlayer = active; }


	//other functions
	void generatePossibleMoves();
private:
	std::vector<std::pair<int, int>> availableMoves;

	AINode* parent;

	std::vector<AINode*> branches; // child nodes



	BOARD_SQUARE_STATE activePlayer; //player active at this point in the tree, the initial player is passed in and the children should use opposite
	GameState worldState; // game state at this node

	int ranking;
	int visits;
	bool isEndState;
};

