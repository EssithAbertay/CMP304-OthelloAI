#include "AINode.h"

//functions used in this cpp not available outside


BOARD_SQUARE_STATE getOppositeMove(BOARD_SQUARE_STATE state)
{
	// Flip to the other player
	if (state == BOARD_SQUARE_STATE::RED)
	{
		return BOARD_SQUARE_STATE::BLUE;
	}
	else if (state == BOARD_SQUARE_STATE::BLUE)
	{
		return BOARD_SQUARE_STATE::RED;
	}
	else
		return BOARD_SQUARE_STATE::NONE;
}

//end of these functions



AINode::AINode()
{
	ranking = 0;
	visits = 0;
}

AINode::~AINode()
{
}


//Select the node to be expanded
AINode* AINode::Select()
{
	// if there are no child nodes, we expand this node
	if (branches.size() == 0 || availableMoves.size() > 0)
		return this;
	else
	{
		// this should be extended later to have an exploration function
		//	return branches[randomBranch]->Select();
		return FindHighestRankingChild(true)->Select();
	}
}

//Expand the selected node (create a child by making a move)
AINode* AINode::Expand()
{
	if (isEndState || availableMoves.size() == 0) // no moves available or end of game, nothing to be expanded
	{
		return NULL;
	}
	else
	{
		int randomMove = rand() % availableMoves.size();
		std::pair<int, int> move = availableMoves[randomMove];

		//delete move from available moves
		availableMoves.erase(availableMoves.begin() + randomMove);

		//create new child with move
		AINode* childNode = new AINode();
		childNode->setParent(this);

		BOARD_SQUARE_STATE nextTurn = getOppositeMove(activePlayer);
		childNode->setActivePlayer(nextTurn);

		//create the childs world state/game state
		GameState newWorldState;
		newWorldState.gameBoard = this->worldState.gameBoard;
		GameAction newAction(move.first, move.second, nextTurn);

		//apply the move to the childs board
		newWorldState.setAndApplyAction(newAction);
		childNode->setGameState(newWorldState);

		//add this child as a child of this node
		branches.push_back(childNode);

		//return the child for simulation
		return childNode; 
	}
	return nullptr;
}

//simulate the new node till an end state is reached
void AINode::Simulate(BOARD_SQUARE_STATE startingTurn)
{
	GameState copyOfGameState = getGameState();

	BOARD_SQUARE_STATE playerTurn = startingTurn;

	bool endState = false;

	//check if this new state is an end of game state
	BOARD_SQUARE_STATE winner = copyOfGameState.checkWin();
	if (winner != BOARD_SQUARE_STATE::NONE) //this needs changed as i have other states, either that or i need to change the types, probably will do the latter
	{
		CalcResult(winner);
		return;
	}

	//repeat till we get to an end state
	do {
		//get possible moves
		std::vector<std::pair<int, int>> possibleMoves = copyOfGameState.getPossibleMoves(playerTurn); 
		
		if (possibleMoves.size() == 0)
		{
			//no possible moves, either a draw or just no available moves, need to add logic for the latter, maybe just not return and add a toggle?
			endState = true;
			CalcResult(BOARD_SQUARE_STATE::NONE);
			return;
		}
		else
		{
			// pick a random move and apply it to the simulation state
			int randomMove = rand() % possibleMoves.size();
			GameAction newAction(possibleMoves[randomMove].first, possibleMoves[randomMove].second, playerTurn);
			copyOfGameState.setAndApplyAction(newAction);
			possibleMoves.clear();
		}

		//check if the last move was a winning one
		BOARD_SQUARE_STATE winner = copyOfGameState.checkWin();


		if (winner != BOARD_SQUARE_STATE::NONE)
		{
			// end state, game has been won
			CalcResult(winner);
			endState = true;
			return;
		}

		// flip marker for next move
		playerTurn = getOppositeMove(playerTurn);

	}while (!endState);

}

//return result to parent
void AINode::Backpropagate(int result)
{
	visits++;

	if (result > 0) {
		ranking += result;
	}

	// if we're not at the root, update the parent with the result
	if (this->parent != NULL)
		this->parent->Backpropagate(result);
}

AINode* AINode::FindHighestRankingChild(bool report)
{
	if (branches.size() == 0)
		return NULL;

	float currHighWR = 0;
	float nodeWR = 0;
	int highIndex = 0;

	//float maxRanking = 0;
	//int maxIndex = 0;

	for (int i = 0; i < branches.size(); i++)
	{
		nodeWR = branches[i]->getRanking() / branches[i]->getVisits();

		if (nodeWR > currHighWR)
		{
			currHighWR = nodeWR;
			highIndex = i;
		}
	}

	return branches[highIndex];
}

void AINode::CalcResult(BOARD_SQUARE_STATE winner)
{
	if (winner == BOARD_SQUARE_STATE::RED)
	{
		Backpropagate(1);
	}
	else if (winner == BOARD_SQUARE_STATE::BLUE)
	{
		Backpropagate(-1);
	}
	else
	{
		Backpropagate(0);
	}
}

void AINode::resetNode()
{
	branches.clear();
	visits = 0;
	ranking = 0;
	availableMoves.clear();
}

void AINode::setGameState(GameState newState)
{
	worldState = newState;
	generatePossibleMoves();
}

void AINode::generatePossibleMoves()
{
	availableMoves = worldState.getPossibleMoves(activePlayer);
}
