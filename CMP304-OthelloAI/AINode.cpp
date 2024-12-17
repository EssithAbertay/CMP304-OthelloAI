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
	// if there are no child nodes, we expand this node, or if this node hasn't had every move explored yet
	if (branches.size() == 0 || availableMoves.size() > 0)
		return this;
	else
	{
		return findChildUCB()->Select(); //otherwise we find a child to expand using the UCB formula
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
				CalcResult(copyOfGameState.findMore()); //check who the winner is
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
			// pick a random move and apply it to the simulation state, if it's considered a very bad move then dont do it

			bool isPresent = false;
			int randomMove = 0;

			std::pair<int, int> move;

			if (possibleMoves.size() > 1) //if more than one move available choose between them otherwise just use the only move abailable
			{
				do
				{
					isPresent = false;
					randomMove = rand() % possibleMoves.size();

					move = std::pair<int, int>(possibleMoves[randomMove].first, possibleMoves[randomMove].second);

				
						for (int iter = 0; iter < 4; iter++)
						{
							if (move == veryBadPositions[iter]) {
								isPresent = true;
								possibleMoves.erase(possibleMoves.begin() + randomMove); //remove the very bad move from the list of moves
								break;
							}
						}
					

					if (possibleMoves.size() == 1) //if there is only one move left, then it must be played
					{
						isPresent = false;
						randomMove = 0;
					}
				} while (isPresent);


				//if (possibleMoves.size() > 1) //don't want to accidentally erase the last move
				//{

				//	do
				//	{
				//		isPresent = false;
				//		randomMove = rand() % possibleMoves.size();

				//		move = std::pair<int, int>(possibleMoves[randomMove].first, possibleMoves[randomMove].second);
				//	
				//		for (int iter = 0; iter < 8; iter++) //loop for all the bad moves 
				//		{
				//			if (move == badPositions[iter]) {
				//				isPresent = true;
				//				possibleMoves.erase(possibleMoves.begin() + randomMove); //remove the bad move from the list of moves
				//			break;
				//			}
				//		}

				//		if (possibleMoves.size() == 1) //if there is only one move left, then it must be played
				//		{
				//			isPresent = false;
				//			randomMove = 0;
				//		}
				//	} while (isPresent);

				//}
			}

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

AINode* AINode::findChildUCB()
{
	if (branches.size() == 0) //if there are no branches
	{
		return NULL;
	}

	int highIndex = 0;
	float nodeExplorationValue;
	float currHighNodeExplorationValue = 0;
	float explorationParam = 3;
	float childWins = 0;
	float childSims = 0;
	float parentSims = visits;

	//bool set = false;

	//BOARD_SQUARE_STATE  branchMove;
	//BOARD_SQUARE_STATE  branchOppositeMove;
	//GameState branchBoard;

	for (int i = 0; i < branches.size(); i++)
	{

		//ucb formula
		childWins = branches[i]->getRanking();
		childSims = branches[i]->getVisits();
		nodeExplorationValue = (childWins / childSims) + (explorationParam * sqrt(log(parentSims) / childSims));

		//branchMove = branches[i]->activePlayer;
		//branchOppositeMove = getOppositeMove(branchMove);

		//if (branchMove == RED) //if the player of this branch was red (the ai) then we want to look at what moves blue (the player) can make as a result of the ai move, if the ai move allows for the player to take a corner, then it should be discouraged, as corners are very strong.
		//{
		//	branchBoard = branches[i]->getGameState();//get the state at the branch
		//	
		//	auto goodplayermove = branchBoard.getPossibleMoves(branchOppositeMove); //get the moves available

		//	branchBoard.setAndApplyAction(branches[i]->worldState.gameAction); //apply the branches action the this game state

		//	auto branchMoves = branchBoard.getPossibleMoves(branchOppositeMove); //get the moves available

		//	for (int j = 0; j < branchMoves.size(); j++) //moves that make it so the other player can take a corner should not be searched farther
		//	{
		//		if (branchMoves[j] == std::make_pair(1, 1) || branchMoves[j] == std::make_pair(6, 6) || branchMoves[j] == std::make_pair(6, 1) || branchMoves[j] == std::make_pair(1, 6)) //bad move by the ai
		//		{
		//			nodeExplorationValue = -999999; 
		//		}

		//		if (goodplayermove[j] == std::make_pair(0, 0) || goodplayermove[j] == std::make_pair(7, 7) || goodplayermove[j] == std::make_pair(0, 7) || goodplayermove[j] == std::make_pair(7, 0)) //move that allows a very good move for the player
		//		{
		//		nodeExplorationValue = -999999;
		//		}
		//	}

		//	
		//}

		if (nodeExplorationValue > currHighNodeExplorationValue)
		{
				currHighNodeExplorationValue = nodeExplorationValue;
				highIndex = i;
		}
	}

	return branches[highIndex];
}

AINode* AINode::findHighestRankingChild()
{
	int highRank = 0;
	int highIndex = 0;
	for (int i = 0; i < branches.size(); i++)
	{
		if (branches[i]->getRanking() / branches[i]->getVisits() > highRank)
		{
			highIndex = i;
			highRank = branches[i]->getRanking() / branches[i]->getVisits();
		}

	}
	return branches[highIndex];
}

void AINode::CalcResult(BOARD_SQUARE_STATE winner)
{
	if (winner == BOARD_SQUARE_STATE::RED) //when the ai wins get a reward
	{
		Backpropagate(1);
	}
	else if (winner == BOARD_SQUARE_STATE::BLUE) //when the player wins
	{
		Backpropagate(-1);
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
