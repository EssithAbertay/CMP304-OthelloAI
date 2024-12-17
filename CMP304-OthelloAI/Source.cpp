#include <iostream>

#include "BoardAndAction.h"
#include "AINode.h"

const int MAX_RUNS = 1000;

//globals
int aiwins = 0;
int plyrwins = 0;
int draws = 0;

void displayText(int gameType, std::string text) //only use for text that has a new line following
{
	if (gameType == 0) //only display text in the correct game state
	{
		std::cout << text << std::endl;
	}
}


bool winDisplay(BOARD_SQUARE_STATE winner, int p_gameType)
{
	if (winner == BOARD_SQUARE_STATE::BLUE)
	{
		displayText(p_gameType, "PLAYER WINS!");
		plyrwins++;
		return true;
	}
	else  if (winner == BOARD_SQUARE_STATE::RED)
	{
		displayText(p_gameType, "AI WINS!");
		aiwins++;
		return true;
	}
	else  if (winner == BOARD_SQUARE_STATE::DRAW)
	{
		displayText(p_gameType, "IT'S A DRAW");
		draws++;
		return true;
	}

	return false;
}

void main()
{
	srand(time(NULL));


	BOARD_SQUARE_STATE aiMarker = BOARD_SQUARE_STATE::RED;
	BOARD_SQUARE_STATE playerMarker = BOARD_SQUARE_STATE::BLUE;

	int numberOfGamesToPlay = 1;
	int gameType = 0;

	bool turnPlayed = false;


	std::cout << "Game type, 0 for human, 1 for random moves" << std::endl;
	std::cin >> gameType; 

	if (gameType == 1) //only need to track the number of games when playing angainst the bot
	{
		std::cout << "Number of games to play" << std::endl;
		std::cin >> numberOfGamesToPlay;
	}



	//initial board
	for (int a = 0; a < numberOfGamesToPlay; a++)
	{
		GameState mainGameState;
		AINode* rootNode = new AINode();

		bool gameOver = false;

		do {
			turnPlayed = false;


			displayText(gameType, "Thinking....");

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
					expandedNode->Simulate(playerMarker); //the value put in here is irrelevant, in the simulate it makes sure that the current player is opposite to the parents player
				}

				runCount++;
			} while (runCount < MAX_RUNS); //could use a for loop instead

			BOARD_SQUARE_STATE winner;

			auto availableaimoves = mainGameState.getPossibleMoves(RED);

			if (availableaimoves.size() != 0) //only play if the ai has a valid move
			{
				//perform the decided action
				AINode* highestChild = rootNode->findHighestRankingChild();

				GameAction bestAction = highestChild->getGameState().gameAction;

				for (int i = 0; i < availableaimoves.size(); i++) //when a corner can be taken it must be taken, it is always good!
				{
					if (availableaimoves[i] == std::make_pair(0, 0) || availableaimoves[i] == std::make_pair(7, 7) || availableaimoves[i] == std::make_pair(0, 7) || availableaimoves[i] == std::make_pair(7, 0)) //if an available move is the corner it will be played
					{
						bestAction.x = availableaimoves[i].first;
						bestAction.y = availableaimoves[i].second;
					}

				}

				bestAction.playerMove = aiMarker;

				if (gameType == 0)
				{
					std::cout << "The AI played " << bestAction.y << " " << bestAction.x << std::endl;
				}
				//update the main state with the ai move activePlayer	RED (2)	BOARD_SQUARE_STATE

				mainGameState.setAndApplyAction(bestAction);

				if (gameType == 0)
				{
					mainGameState.printBoard();
				}

				//check if the move finished the game
				winner = mainGameState.checkWin();

				bool temp = winDisplay(winner, gameType);

				if (temp)
				{
					break;
				}

				turnPlayed = true;
			}
			else
			{
				displayText(gameType, "AI CANNOT PLAY!");
				displayText(gameType, "AI TURN SKIPPED!");
			}

			displayText(gameType, "PLAYER TURN!");

			bool validMove = true;


			std::vector<std::pair<int, int>> playerMoves = mainGameState.getPossibleMoves(BLUE);

			int x = -1, y = -1;
			GameAction playerAction;

			if (playerMoves.size() != 0) //if the player has avaialble moves, if they dont then their turn is skipped, already had a win check so we know that neither player has won yet
			{
				if (gameType == 0)
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
					
				}
				else
				{
					playerAction.x = playerMoves[rand() % playerMoves.size()].first;
					playerAction.y = playerMoves[rand() % playerMoves.size()].second;
				}

				playerAction.playerMove = playerMarker;

				mainGameState.setAndApplyAction(playerAction);
				if (gameType == 0)
				{
					mainGameState.printBoard();
				}


				turnPlayed = true;
			}
			else
			{
				displayText(gameType, "PLAYER CANNOT PLAY!");
				displayText(gameType, "PLAYER TURN SKIPPED!");
			}


			//check if the player won the game
			winner = mainGameState.checkWin();

			bool temp = winDisplay(winner, gameType);

			if (temp)
			{
				break;
			}

			if (winner == NONE && !turnPlayed)
			{
				switch (mainGameState.findMore())
				{
				case BLUE: 
					gameOver = true;

					displayText(gameType, "PLAYER WINS!");
					plyrwins++;
					break;
				case RED:
					gameOver = true;
					displayText(gameType, "AI WINS!");
					aiwins++;
					break;
				case DRAW:
					gameOver = true;
					displayText(gameType, "IT'S A DRAW");
					draws++;
					break;
				default:
					break;
				}
			}
			// reset the root node ready for next turn
			rootNode->resetNode();

		} while (!gameOver);


		displayText(gameType, "GAME OVER!");
	}


	if (gameType == 1) //only output the overall results if the game is played against a random bot
	{
		std::cout << "AI: " << aiwins << "   Random: " << plyrwins << "   Draws: " << draws << std::endl;
	}

	return;   
}