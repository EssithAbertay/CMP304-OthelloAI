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
	AINode* Expand(int veryBadMovesExclusion);

	void Simulate(BOARD_SQUARE_STATE startingTurn, int veryBadMovesExclusion);
	void SimulateXTurns(BOARD_SQUARE_STATE startingTurn, int veryBadMovesExclusion, int turnsToPlayInSim);
	void Backpropagate(int reward);

	//tree functions
	AINode* findChildUCB(); //used to search/expand the tree
	AINode* findHighestRankingChild(); //used to find the path most explored/best path

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

	std::pair<int, int> veryBadPositions[4] = { {1,1},{1,6},{6,1},{6,6} };//very bad moves are the squares diagonally inwards from the corners as they tend to make it very easy to take the corner

	std::pair<int, int> badPositions[8] = { {0,1},{1,0},{6,0},{7,1},{7,6},{6,7},{0,6},{1,7} }; //bad moves are squares directly adjacent to corners, not as bad as the very bad moves but still bad


};

