#include <iostream>

#include "BoardAndAction.h"
#include "AINode.h"


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

	int maxRuns = 0;

	float explorationValue;

	int useVeryBadMoveExclusion = 0;
	int cornerGrabbing = 0;

	int playTillEndOfGameInSim = 0;
	int turnsToPlayInSim = -1;

	bool turnPlayed = false;


	std::cout << "Game type, 0 for human, 1 for random moves" << std::endl;
	std::cin >> gameType; 

	if (gameType == 1) //only need to track the number of games when playing angainst the bot
	{
		std::cout << "Number of games to play" << std::endl;
		std::cin >> numberOfGamesToPlay;
	}

	std::cout << "Maximum number of runs that the AI will perform? (default 10000 for human, 1000 for random)" << std::endl;
	std::cin >> maxRuns;

	std::cout << "Enter the exploration value for the AI (default 1.6)" << std::endl;
	std::cin >> explorationValue;

	std::cout << "When simulating should the AI avoid very bad moves? no = 0, yes = 1 (default yes)" << std::endl;
	std::cin >> useVeryBadMoveExclusion;

	std::cout << "When playing should the AI grab corners? no = 0, yes = 1 (default yes)" << std::endl;
	std::cin >> cornerGrabbing;
	
	std::cout << "When simulating should the AI play till the end of the game? no = 0, yes = 1 (default yes)" << std::endl;
	std::cin >> playTillEndOfGameInSim;

	if(playTillEndOfGameInSim == 0)
	{
		std::cout << "How many turns should the ai play in sim" << std::endl;
		std::cin >> turnsToPlayInSim;
	}

	std::cout << "Starting..." << std::endl;

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

			//generate the tree
			do {
				//select
				AINode* selectedNode = rootNode->Select();

				//expand
				AINode* expandedNode = selectedNode->Expand(useVeryBadMoveExclusion);

				//expanded node is null if its a leaf and then sim is not possible
				if (!expandedNode == NULL)
				{
					//sim and backpropagate

					if (turnsToPlayInSim == -1) //if simulating to the end of the game
					{
						expandedNode->Simulate(playerMarker, useVeryBadMoveExclusion);
					}
					else
					{
						expandedNode->SimulateXTurns(playerMarker, useVeryBadMoveExclusion, turnsToPlayInSim); //the value put in here is irrelevant, in the simulate it makes sure that the current player is opposite to the parents player
					}
				}

				runCount++;
			} while (runCount < maxRuns); //explore and expand the tree this many times

			BOARD_SQUARE_STATE winner;

			auto availableaimoves = mainGameState.getPossibleMoves(RED);

			if (availableaimoves.size() != 0) //only play if the ai has a valid move
			{
				//perform the decided action
				AINode* highestChild = rootNode->findHighestRankingChild(); //find the child with the highest wr, ucb not used here, we just want the best move

				GameAction bestAction = highestChild->getGameState().gameAction;

				//when a corner can be taken it must be taken, it is almost always good!
				if (cornerGrabbing == 1)
				{
					for (int i = 0; i < availableaimoves.size(); i++)
					{
						if (availableaimoves[i] == std::make_pair(0, 0) || availableaimoves[i] == std::make_pair(7, 7) || availableaimoves[i] == std::make_pair(0, 7) || availableaimoves[i] == std::make_pair(7, 0)) //if an available move is the corner it will be played
						{
							bestAction.x = availableaimoves[i].first;
							bestAction.y = availableaimoves[i].second;
						}

					}
				}

				bestAction.playerMove = aiMarker;

				if (gameType == 0) //the cout has multiple variables so i manually check if the gametype warrants text to be displayed
				{
					std::cout << "The AI played " << bestAction.y << " " << bestAction.x << std::endl;
				}

				//update the main state with the ai move activePlayer	RED (2)	BOARD_SQUARE_STATE
				mainGameState.setAndApplyAction(bestAction);

				if (gameType == 0) //only need to display the board if playing a real person
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

			//if the game finished, i.e all squares played then display and break
			if (winDisplay(winner, gameType))
			{
				break;
			}


			//if winner was none (i.e squares are still free) and no turns were played (i.e no valid moves for either player) we do a different win check and just see who has more tiles
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
			rootNode->resetNode(); //cause we only reset the root, all the other nodes still exist, icredibly memory inefficient :/

		} while (!gameOver);


		displayText(gameType, "GAME OVER!"); //once game is done say so
	}


	if (gameType == 1) //only output the overall results if the game is played against a random bot
	{
		std::cout << "AI: " << aiwins << "   Random: " << plyrwins << "   Draws: " << draws << std::endl;
	}

	return;   
}