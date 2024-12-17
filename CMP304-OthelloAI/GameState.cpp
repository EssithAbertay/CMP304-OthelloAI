#include "GameState.h"

GameState::GameState()
{
}

void GameState::setAndApplyAction(GameAction newAction)
{
	gameAction = newAction;

	gameBoard.board[gameAction.x][gameAction.y] = gameAction.playerMove;

	FlipPieces();
}

void GameState::FlipPieces()
{
	BOARD_SQUARE_STATE playedColour = gameAction.playerMove;
	BOARD_SQUARE_STATE oppositeColour = (playedColour == BLUE) ? RED : BLUE;	//set the opposite colour as the opposite aas to what was played


	bool flip = false;
	int a = 0;


	//there are 8 different directions to check, up, down, left, right, up left, up right, down left, and down right
	//y is horizontal and x is vertical idk why but we're gonna live with it

	//in future a better implementation of the flips and checks could have been used, this implementation has led to many errors, potentially looping though an array of directions would have worked

	//check left - works

	if (gameAction.y >= 2) //it can only flip cells on the left if its beyond a point
	{
		if (gameBoard.board[gameAction.x][gameAction.y - 1] == oppositeColour)
		{
			for (int i = 2; i <= gameAction.y; i++)
			{
				if (gameBoard.board[gameAction.x][gameAction.y - i] == NONE)
				{
					break;
				}
				if (gameBoard.board[gameAction.x][gameAction.y - i] == playedColour)
				{
					flip = true;
					a = i;
					break;
				}
			}

			if (flip)
			{
				for (int i = 0; i < a; i++)
				{
					gameBoard.board[gameAction.x][gameAction.y - i] = playedColour;
				}
			}

		}
	}

	//reset flip
	flip = false;

	//check right - fixed

	if (gameAction.y <= BOARD_DIMENSION - 2) //

	{
		if (gameBoard.board[gameAction.x][gameAction.y + 1] == oppositeColour)
		{
			for (int i = 2; i <= BOARD_DIMENSION - gameAction.y; i++)
			{

				if (gameBoard.board[gameAction.x][gameAction.y + i] == NONE)
				{
					break;
				}
				if (gameBoard.board[gameAction.x][gameAction.y + i] == playedColour)
				{
					flip = true;
					a = i;
					break;
				}
			}

			if (flip)
			{
				for (int i = 0; i < a; i++)
				{
					gameBoard.board[gameAction.x][gameAction.y + i] = playedColour;
				}
			}

		}
	}

	flip = false;

	//check up - fixed

	if (gameAction.x >= 2) //it can only flip cells on the left if its beyond a point

	{
		if (gameBoard.board[gameAction.x - 1][gameAction.y] == oppositeColour)
		{
			for (int i = 2; i <= gameAction.x; i++)
			{
				if (gameBoard.board[gameAction.x - i][gameAction.y] == NONE)
				{
					break;
				}
				if (gameBoard.board[gameAction.x - i][gameAction.y] == playedColour)
				{
					flip = true;
					a = i;
					break;
				}
			}

			if (flip)
			{
				for (int i = 0; i < a; i++)
				{
					gameBoard.board[gameAction.x - i][gameAction.y] = playedColour;
				}
			}

		}
	}

	flip = false;

	//check down - fixed

	if (gameAction.x <= BOARD_DIMENSION - 2) //

	{
		if (gameBoard.board[gameAction.x + 1][gameAction.y] == oppositeColour)
		{
			for (int i = 2; i <= BOARD_DIMENSION - gameAction.x; i++)
			{
				if (gameBoard.board[gameAction.x + i][gameAction.y] == NONE)
				{
					break;
				}
				if (gameBoard.board[gameAction.x + i][gameAction.y] == playedColour)
				{
					flip = true;
					a = i;
					break;
				}
			}

			if (flip)
			{
				for (int i = 0; i < a; i++)
				{
					gameBoard.board[gameAction.x + i][gameAction.y] = playedColour;
				}
			}

		}
	}

	flip = false;

	//check up left - fixed

	if (gameAction.x >= 2 && gameAction.y >= 2)
	{
		if (gameBoard.board[gameAction.x - 1][gameAction.y - 1] == oppositeColour)
		{
			//use the smaller of the start positions to decide how many times to loop, since that decides how far from and edge you are, if they are the same it doesnt matter
			int iteratorMax = std::min(gameAction.x, gameAction.y);

			for (int i = 2; i <= iteratorMax; i++)
			{
				if (gameBoard.board[gameAction.x - i][gameAction.y - i] == NONE)
				{
					break;
				}
				if (gameBoard.board[gameAction.x - i][gameAction.y - i] == playedColour)
				{
					flip = true;
					a = i;
					break;
				}
			}

			if (flip)
			{
				for (int i = 0; i < a; i++)
				{
					gameBoard.board[gameAction.x - i][gameAction.y - i] = playedColour;
				}
			}

		}
	}

	flip = false;

	//check up right

	if (gameAction.x >= 2 && gameAction.y <= BOARD_DIMENSION - 2 )
	{
		if (gameBoard.board[gameAction.x - 1][gameAction.y + 1] == oppositeColour)
		{

			int iteratorMax = std::min(gameAction.x, BOARD_DIMENSION - gameAction.y);

			for (int i = 2; i <= iteratorMax; i++)
			{
				if (gameBoard.board[gameAction.x - i][gameAction.y + i] == NONE)
				{
					break;
				}
				if (gameBoard.board[gameAction.x - i][gameAction.y + i] == playedColour)
				{
					flip = true;
					a = i;
					break;
				}
			}

			if (flip)
			{
				for (int i = 1; i < a; i++)
				{
					gameBoard.board[gameAction.x - i][gameAction.y + i] = playedColour;
				}
			}

		}
	}

	flip = false;

	//check down left

	if (gameAction.x <= BOARD_DIMENSION - 2 && gameAction.y >= 2)
	{
		if (gameBoard.board[gameAction.x + 1][gameAction.y - 1] == oppositeColour)
		{

			int iteratorMax = std::min(BOARD_DIMENSION - gameAction.x, gameAction.y);

			for (int i = 2; i <= iteratorMax; i++)
			{
				if (gameBoard.board[gameAction.x + i][gameAction.y - i] == NONE)
				{
					break;
				}
				if (gameBoard.board[gameAction.x + i][gameAction.y - i] == playedColour)
				{
					flip = true;
					a = i;
					break;
				}
			}

			if (flip)
			{
				for (int i = 1; i < a; i++)
				{
					gameBoard.board[gameAction.x + i][gameAction.y - i] = playedColour;
				}
			}

		}
	}

	flip = false;

	//check down right

	if (gameAction.x <= BOARD_DIMENSION - 2 && gameAction.y <= BOARD_DIMENSION - 2)
	{
		if (gameBoard.board[gameAction.x + 1][gameAction.y + 1] == oppositeColour)
		{
			//use the smaller of the start positions to decide how many times to loop, since that decides how far from and edge you are, if they are the same it doesnt matter
			int iteratorMax = std::min(BOARD_DIMENSION - gameAction.x, BOARD_DIMENSION - gameAction.y);

			for (int i = 2; i <= iteratorMax; i++)
			{
				if (gameBoard.board[gameAction.x + i][gameAction.y + i] == NONE)
				{
					break;
				}
				if (gameBoard.board[gameAction.x + i][gameAction.y + i] == playedColour)
				{
					flip = true;
					a = i;
					break;
				}
			}

			if (flip)
			{
				for (int i = 0; i < a; i++)
				{
					gameBoard.board[gameAction.x + i][gameAction.y + i] = playedColour;
				}
			}

		}
	}

}

std::vector<std::pair<int, int>> GameState::getPossibleMoves(BOARD_SQUARE_STATE p_colour)
{
	std::vector<std::pair<int, int>> validMoves;

	GameAction moveBeingChecked;
	moveBeingChecked.playerMove = p_colour;
	//loop for all squares on board
	for (int i = 0; i < BOARD_DIMENSION + 1; i++)
	{
		for (int j = 0; j < BOARD_DIMENSION + 1; j++)
		{
			//logic check to see if a move is valid
			if (gameBoard.board[i][j] == NONE) //1. can only be valid if not played on yet
			{
				//2. must be bordering opposite cell
				//3. must be able to flip bordering cell
				moveBeingChecked.x = i;
				moveBeingChecked.y = j;
				if (CheckMove(moveBeingChecked))
				{
					validMoves.push_back(std::make_pair(i, j)); //add valid move to list of moves
				}
			}

		}
	}
	return validMoves;
}

bool GameState::CheckMove(GameAction moveToCheck)
{
	BOARD_SQUARE_STATE playedColour = moveToCheck.playerMove;
	BOARD_SQUARE_STATE oppositeColour = (playedColour == BLUE) ? RED : BLUE;

	//this code is super lazy, ive copypassted from the flip pieces function, ideally it would just use a  shared function with flip pieces but im too tired for the right now, so gonna have mega code duplication!

	//check if can flip on left
	if (moveToCheck.y >= 2)
	{
		if (gameBoard.board[moveToCheck.x][moveToCheck.y - 1] == oppositeColour)
		{
			for (int i = 2; i <= moveToCheck.y; i++)
			{
				if (gameBoard.board[moveToCheck.x][moveToCheck.y - i] == NONE)
				{
					break;
				}

				if (gameBoard.board[moveToCheck.x][moveToCheck.y - i] == playedColour)
				{
					return true;
				}
			}
		}
	}

	//check if can flip on right
	if (moveToCheck.y <= BOARD_DIMENSION - 2)
	{
		if (gameBoard.board[moveToCheck.x][moveToCheck.y + 1] == oppositeColour)
		{
			for (int i = 2; i <= BOARD_DIMENSION -moveToCheck.y; i++)
			{
				if (gameBoard.board[moveToCheck.x][moveToCheck.y + i] == NONE)
				{
					break;
				}
				if (gameBoard.board[moveToCheck.x][moveToCheck.y + i] == playedColour)
				{
					return true;
				}
			}
		}
	}

	//check if can flip up
	if (moveToCheck.x >= 2) //it can only flip cells on the left if its beyond a point
	{
		if (gameBoard.board[moveToCheck.x - 1][moveToCheck.y] == oppositeColour)
		{
			for (int i = 2; i <= moveToCheck.x; i++)
			{
				if (gameBoard.board[moveToCheck.x - i][moveToCheck.y] == NONE)
				{
					break;
				}
				if (gameBoard.board[moveToCheck.x - i][moveToCheck.y] == playedColour)
				{
					return true;
				}
			}
		}
	}

	//check if can flip down
	if (moveToCheck.x <= BOARD_DIMENSION - 2) //
	{
		if (gameBoard.board[moveToCheck.x + 1][moveToCheck.y] == oppositeColour)
		{
			for (int i = 2; i <= BOARD_DIMENSION - moveToCheck.x; i++)
			{
				if (gameBoard.board[moveToCheck.x + i][moveToCheck.y] == NONE)
				{
					break;
				}
				if (gameBoard.board[moveToCheck.x + i][moveToCheck.y] == playedColour)
				{
					return true;
				}
			}
		}
	}

	//check up left
	if (moveToCheck.x >= 2 && moveToCheck.y >= 2)
	{
		if (gameBoard.board[moveToCheck.x - 1][moveToCheck.y - 1] == oppositeColour)
		{
			//use the smaller of the start positions to decide how many times to loop, since that decides how far from and edge you are, if they are the same it doesnt matter
			int iteratorMax = std::min(moveToCheck.x, moveToCheck.y);

			for (int i = 2; i <= iteratorMax; i++)
			{
				if (gameBoard.board[moveToCheck.x - i][moveToCheck.y - i] == NONE)
				{
					break;
				}
				if (gameBoard.board[moveToCheck.x - i][moveToCheck.y - i] == playedColour)
				{
					return true;
				}
			}

		}
	}

	//check up right
	if (moveToCheck.x >= 2 && moveToCheck.y <= BOARD_DIMENSION - 2)
	{
		if (gameBoard.board[moveToCheck.x - 1][moveToCheck.y + 1] == oppositeColour)
		{

			int iteratorMax = std::min(moveToCheck.x, BOARD_DIMENSION - moveToCheck.y);

			for (int i = 2; i <= iteratorMax; i++)
			{
				if (gameBoard.board[moveToCheck.x - i][moveToCheck.y + i] == NONE)
				{
					break;
				}
				if (gameBoard.board[moveToCheck.x - i][moveToCheck.y + i] == playedColour)
				{
					return true;
				}
			}
		}
	}


	//check down left
	if (moveToCheck.x <= BOARD_DIMENSION - 2 && moveToCheck.y >= 2)
	{
		if (gameBoard.board[moveToCheck.x + 1][moveToCheck.y - 1] == oppositeColour)
		{

			int iteratorMax = std::min(BOARD_DIMENSION - moveToCheck.x, moveToCheck.y);

			for (int i = 2; i <= iteratorMax; i++)
			{
				if (gameBoard.board[moveToCheck.x + i][moveToCheck.y - i] == NONE)
				{
					break;
				}
				if (gameBoard.board[moveToCheck.x + i][moveToCheck.y - i] == playedColour)
				{
					return true;
				}
			}
		}
	}


	//down right
	if (moveToCheck.x <= BOARD_DIMENSION - 2 && moveToCheck.y <= BOARD_DIMENSION - 2)
	{
		if (gameBoard.board[moveToCheck.x + 1][moveToCheck.y + 1] == oppositeColour)
		{
			//use the smaller of the start positions to decide how many times to loop, since that decides how far from and edge you are, if they are the same it doesnt matter
			int iteratorMax = std::min(BOARD_DIMENSION - moveToCheck.x, BOARD_DIMENSION - moveToCheck.y);

			for (int i = 2; i <= iteratorMax; i++)
			{
				if (gameBoard.board[moveToCheck.x + i][moveToCheck.y + i] == NONE)
				{
					break;
				}
				if (gameBoard.board[moveToCheck.x + i][moveToCheck.y + i] == playedColour)
				{
					return true;
				}
			}
		}
	}

	return false;
}

void GameState::printBoard()
{
	std::cout << ANSI_RESET << " 01234567" << std::endl;
	for (int i = 0; i < BOARD_DIMENSION + 1; i++)
	{
		std::cout << ANSI_RESET << i;
		for (int j = 0; j < BOARD_DIMENSION + 1; j++)
		{

			switch (gameBoard.board[i][j])
			{
			case (NONE):
				std::cout << ANSI_WHITE << " ";
				break;
			case (BLUE):
				std::cout << ANSI_BLUE << "B";
				break;
			case (RED):
				std::cout << ANSI_RED << "R";
				break;
			//case (VALID):
			//	std::cout << ANSI_GREEN << " ";
			//	break;
			default:
				break;
			}
		}
		std::cout << ANSI_RESET << std::endl;

	}
	std::cout << ANSI_RESET << std::endl;
}

BOARD_SQUARE_STATE GameState::checkWin()
{
	int blueSquares = 0;
	int redSquares = 0;

	bool gameFinished = true;

	for (int i = 0; i < BOARD_DIMENSION + 1; i++)
	{
		for (int j = 0; j < BOARD_DIMENSION + 1; j++)
		{

			switch (gameBoard.board[i][j])
			{
			case (NONE):
				gameFinished = false;
				break;
			case (BLUE):
				blueSquares++;
				break;
			case (RED):
				redSquares++;
				break;
			default:
				break;
			}


		}
	}

	//if not every square hass a colour the game is not finished so none is returned
	if (!gameFinished)
	{
		return NONE;
	}

	//when more blue or red squares return which
	if (blueSquares > redSquares)
	{	
		//std::cout << "P";
		return BLUE;
	}
	if (redSquares > blueSquares)
	{
		//std::cout << "A";
		return RED;
	}
	//std::cout << "D";
	//if blue and red squares are tied return as such
	return DRAW;
}

BOARD_SQUARE_STATE GameState::findMore()
{
	int blueSquares = 0;
	int redSquares = 0;

	bool gameFinished = true;

	for (int i = 0; i < BOARD_DIMENSION + 1; i++)
	{
		for (int j = 0; j < BOARD_DIMENSION + 1; j++)
		{

			switch (gameBoard.board[i][j])
			{
			case (BLUE):
				blueSquares++;
				break;
			case (RED):
				redSquares++;
				break;
			default:
				break;
			}


		}
	}

	//when more blue or red squares return which
	if (blueSquares > redSquares)
	{
		//std::cout << "P";
		return BLUE;
	}
	if (redSquares > blueSquares)
	{
		//std::cout << "A";
		return RED;
	}

	return DRAW;
}

