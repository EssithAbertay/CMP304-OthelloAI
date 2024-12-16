#include <iostream>

#include "BoardAndAction.h"
#include "AINode.h"

const int MAX_RUNS = 5000;

void AITurn()
{}

void main()
{
	srand(time(NULL));


	BOARD_SQUARE_STATE aiMarker = BOARD_SQUARE_STATE::RED;
	BOARD_SQUARE_STATE playerMarker = BOARD_SQUARE_STATE::BLUE;

	//initial board
	GameState mainGameState;
	AINode* rootNode = new AINode();

	bool gameOver = false;

	do {
		std::cout << "Thinking...." << std::endl;
		rootNode->setActivePlayer(aiMarker); //ai plays first
		rootNode->setGameState(mainGameState);


		// define amount
		int runCount = 0;

		//use mcts to decide ai move
		do {
			//select
			AINode* selectedNode = rootNode->Select();

			//expand
			AINode* expandedNode = selectedNode->Expand();

			//expanded node is null if its a leaf and then sim is not possible
			if (!expandedNode == NULL)
			{
				//sim and backpropagate
				expandedNode->Simulate(aiMarker);
			}

			runCount++;
		} while (runCount < MAX_RUNS); //could use a for loop instead



		//perform the decided action
		AINode* highestChild = rootNode->FindHighestRankingChild();
		GameAction bestAction = highestChild->getGameState().gameAction;
		bestAction.playerMove = aiMarker;
		std::cout << "The AI played " << bestAction.y << " " << bestAction.x << std::endl;

		//update the main state with the ai move		activePlayer	RED (2)	BOARD_SQUARE_STATE

		mainGameState.setAndApplyAction(bestAction);
		mainGameState.printBoard();

		//check if the move finished the game
		BOARD_SQUARE_STATE winner = mainGameState.checkWin();

		if (winner == BOARD_SQUARE_STATE::BLUE)
		{
			gameOver = true;
			std::cout << "PLAYER WINS!" << std::endl;
			break;
		}
		else  if (winner == BOARD_SQUARE_STATE::RED)
		{
			gameOver = true;
			std::cout << "AI WINS!" << std::endl;
			break;
		}
		else  if (winner == BOARD_SQUARE_STATE::DRAW)
		{
			gameOver = true;
			std::cout << "IT'S A DRAW!" << std::endl;
			break;
		}

		std::cout << "PLAYER TURN" << std::endl;

		bool validMove = true;


		std::vector<std::pair<int, int>> playerMoves = mainGameState.getPossibleMoves(BLUE);

		int x = -1, y = -1;
		GameAction playerAction;


		if (playerMoves.size() != 0) //if the player has avaialble moves, if they dont then their turn is skipped, already had a win check so we know that neither player has won yet
		{
			do {
				if (!validMove)
				{
					std::cout << "Enter a valid move!" << std::endl;
				}
				validMove = false;
				x = -1, y = -1;
				std::cout << "Enter your x position: ";  //x and y are flipped i cant be bothered to fix so we will hide that info
				std::cin >> y;

				std::cout << "Enter your y position: ";
				std::cin >> x;


				for (int i = 0; i < playerMoves.size(); i++)
				{
					if (x == playerMoves[i].first && y == playerMoves[i].second)
					{
						validMove = true;
					}
				}
			} while (!validMove);

			playerAction.x = x;
			playerAction.y = y;
			playerAction.playerMove = playerMarker;

			mainGameState.setAndApplyAction(playerAction);
			mainGameState.printBoard();

			//check if the player won the game
			winner = mainGameState.checkWin();

			if (winner == BOARD_SQUARE_STATE::RED)
			{
				gameOver = true;
				std::cout << "AI WINS!" << std::endl;
			}
			else  if (winner == BOARD_SQUARE_STATE::BLUE)
			{
				gameOver = true;
				std::cout << "PLAYER WINS!" << std::endl;
			}

			
		}
		else
		{
			std::cout << "PLAYER CANNOT PLAY!" << std::endl << "PLAYER TURN IS SKIPPED!" << std::endl;
		}
		
		// reset the root node ready for next turn
		rootNode->resetNode();

	}while (!gameOver);
 std::cout << "GAME OVER!" << std::endl;
	return;   
}