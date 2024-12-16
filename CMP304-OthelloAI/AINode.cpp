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
		return FindHighestRankingChild()->Select();
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

	if (parent != NULL) //the current player is the player after the parent
	{
		playerTurn = getOppositeMove(parent->activePlayer);
	}
	
	bool endState = false;

	//check if this new state is an end of game state
	BOARD_SQUARE_STATE winner = copyOfGameState.checkWin();
	if (winner != BOARD_SQUARE_STATE::NONE) //this needs changed as i have other states, either that or i need to change the types, probably will do the latter
	{
		CalcResult(winner);
		endState = true;
		return;
	}

	//repeat till we get to an end state
	do {

		//get possible moves
		std::vector<std::pair<int, int>> possibleMoves = copyOfGameState.getPossibleMoves(playerTurn); 
		
	
		if (possibleMoves.size() == 0 )
		{
			if (copyOfGameState.getPossibleMoves(getOppositeMove(playerTurn)).size() == 0) //if both players have no moves the game ends
			{ 
				endState = true;
				CalcResult(copyOfGameState.checkWin()); //check who the winner is
				return;
			}
			else //when the other playre can still play but the current player cannot
			{
				// flip marker for next move
				playerTurn = getOppositeMove(playerTurn);
				continue;
			}
		}
		else //if moves available
		{
			// pick a random move and apply it to the simulation state
			int randomMove = rand() % possibleMoves.size();
			GameAction newAction(possibleMoves[randomMove].first, possibleMoves[randomMove].second, playerTurn);
			copyOfGameState.setAndApplyAction(newAction);
		}

		//check the result of the last move
		winner = copyOfGameState.checkWin();


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

	//if (result > 0) {
		ranking += result;
	//}

	// if we're not at the root, update the parent with the result
	if (this->parent != NULL)
	{
		this->parent->Backpropagate(result);
	}
}

AINode* AINode::FindHighestRankingChild()
{
	if (branches.size() == 0)
	{
		return NULL;
	}

	int highIndex = 0;

	float currentHigh = 0;
	float nodeExplorationValue;
	float currHighNodeExplorationValue = 0;
	float explorationParam = 1.4;
	float childWins = 0;
	float childSims = 0;
	float parentSims = visits;

	for (int i = 0; i < branches.size(); i++)
	{
		
			//ucb formula
			childWins = branches[i]->getRanking();
			childSims = branches[i]->getVisits();
			nodeExplorationValue = (childWins / childSims) + (explorationParam * sqrt(log(parentSims / childSims)));


			if (nodeExplorationValue > currHighNodeExplorationValue)
			{
				currHighNodeExplorationValue = nodeExplorationValue;
				highIndex = i;
			}
	
	}

	return branches[highIndex];
}

void AINode::CalcResult(BOARD_SQUARE_STATE winner)
{
	if (winner == BOARD_SQUARE_STATE::RED) //when the ai wins get a reward
	{
		Backpropagate(5);
	}
	else if (winner == BOARD_SQUARE_STATE::BLUE) //when the player wins
	{
		Backpropagate(0);
	}
	else if (winner == BOARD_SQUARE_STATE::DRAW) //when you draw nothing
	{
		Backpropagate(0);
	}
	else // redundancy
	{
		Backpropagate(0);
	}
}

void AINode::resetNode()
{	
	availableMoves.clear();
	branches.clear();
	visits = 0;
	ranking = 0;
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
