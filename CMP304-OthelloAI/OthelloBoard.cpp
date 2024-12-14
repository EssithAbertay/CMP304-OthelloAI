#include "OthelloBoard.h"

OthelloBoard::OthelloBoard()
{
}

OthelloBoard::~OthelloBoard()
{
}

void OthelloBoard::StartGame(BOARD_SQUARE_STATE startingPlayer)
{
	currentPlayer = startingPlayer;
}

void OthelloBoard::Setup()
{


	for (int i = 0; i < BOARD_DIMENSION + 1; i++)
	{
		for (int j = 0; j < BOARD_DIMENSION + 1; j++)
		{
			board[i][j] = NONE;
		}
	}

	//in a game of othello/reversi the middle squares start assigned, so that there are valid moves to being with
	board[3][3] = RED;
	board[4][4] = RED;
	board[3][4] = BLUE;
	board[4][3] = BLUE;
}

void OthelloBoard::Display()
{

	std::cout << ANSI_RESET << " 01234567" << std::endl;
	for (int i = 0; i < BOARD_DIMENSION + 1; i++)
	{
		std::cout << ANSI_RESET << i;
		for (int j = 0; j < BOARD_DIMENSION + 1; j++)
		{
			
			switch (board[i][j])
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
			case (VALID):
				std::cout << ANSI_GREEN << " ";
				break;
			default:
				break;
			}
		}
		std::cout << ANSI_RESET << std::endl;
		
	}
	std::cout << ANSI_RESET << std::endl;
}

void OthelloBoard::PlayTurn(player_move p_move)
{
	board[p_move.x][p_move.y] = currentPlayer;
	
	FlipPieces(p_move);
}

void OthelloBoard::ChangeTurn()
{
	switch (currentPlayer)
	{
	case NONE:
		break;
	case BLUE:
		currentPlayer = RED;
		std::cout << ANSI_RED << "Red Turn!" << std::endl;
		std::cout << ANSI_RESET << std::endl;
		break;
	case RED:
		currentPlayer = BLUE;
		std::cout << ANSI_BLUE << "Blue Turn!" << std::endl;
		std::cout << ANSI_RESET << std::endl;
		break;
	default:
		break;
	}
}

void OthelloBoard::FlipPieces(player_move startLocation)
{
	BOARD_SQUARE_STATE playedColour = currentPlayer;
	BOARD_SQUARE_STATE oppositeColour;
	//set the opposite colour as the opposite aas to what was played
	if (playedColour == BLUE)
	{
		oppositeColour = RED;
	}
	else
	{
		oppositeColour = BLUE;
	}

	bool flip = false;
	int a = 0;
	//there are 8 different directions to check, up, down, left, right, up left, up right, down left, and down right
	//y is horizontal and x is vertical idk why but we're gonna live with it

	//check left - functioning

	if (startLocation.y >= 2) //it can only flip cells on the left if its beyond a point
	{
		if (board[startLocation.x ][startLocation.y- 1] == oppositeColour)
		{
			for (int i = 2; i <= startLocation.y; i++)
			{
				if (board[startLocation.x ][startLocation.y- i] == playedColour)
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
					board[startLocation.x][startLocation.y - i] = playedColour;
				}
			}

		}
	}

	flip = false;

	//check right - functioning

	if (startLocation.y <= BOARD_DIMENSION -2) //

	{
		if (board[startLocation.x][startLocation.y + 1] == oppositeColour)
		{
			for (int i = 2; i < BOARD_DIMENSION + 1; i++)
			{
				if (board[startLocation.x][startLocation.y + i] == playedColour)
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
					board[startLocation.x][startLocation.y + i] = playedColour;
				}
			}

		}
	}

	flip = false;

	//check up - functioning

	if (startLocation.x >= 2) //it can only flip cells on the left if its beyond a point

	{
		if (board[startLocation.x- 1][startLocation.y ] == oppositeColour)
		{
			for (int i = 2; i <= startLocation.x; i++)
			{
				if (board[startLocation.x - i][startLocation.y] == playedColour)
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
					board[startLocation.x- i][startLocation.y ] = playedColour;
				}
			}

		}
	}

	flip = false;

	//check down - functioning

	if (startLocation.x <= BOARD_DIMENSION - 2) //

	{
		if (board[startLocation.x+ 1][startLocation.y] == oppositeColour)
		{
			for (int i = 2; i < BOARD_DIMENSION + 1; i++)
			{
				if (board[startLocation.x+ i][startLocation.y ] == playedColour)
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
					board[startLocation.x + i][startLocation.y] = playedColour;
				}
			}

		}
	}

	flip = false;

	//check up left

	if (startLocation.x >= 2 && startLocation.y >= 2)
	{
		if (board[startLocation.x - 1][startLocation.y - 1] == oppositeColour)
		{
			//use the smaller of the start positions to decide how many times to loop, since that decides how far from and edge you are, if they are the same it doesnt matter
			int loopMax = std::min(startLocation.x, startLocation.y);

			for (int i = 2; i < 2+loopMax; i++)
			{
				if (board[startLocation.x - i][startLocation.y - i] == playedColour)
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
					board[startLocation.x - i][startLocation.y - i] = playedColour;
				}
			}

		}
	}

	flip = false;

	//check up right

	if (startLocation.y <= BOARD_DIMENSION - 2 && startLocation.x > 1)
	{
		if (board[startLocation.x - 1][startLocation.y + 1] == oppositeColour)
		{
		
			int iter = 2 + std::min(startLocation.x, 7 - startLocation.y);

			for (int i = 2; i < iter; i++)
			{
				if (board[startLocation.x - i][startLocation.y + i] == playedColour)
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
					board[startLocation.x - i][startLocation.y + i] = playedColour;
				}
			}

		}
	}

	flip = false;
	//check down left

	if (startLocation.x <= BOARD_DIMENSION - 2 && startLocation.y > 1)
	{
		if (board[startLocation.x + 1][startLocation.y - 1] == oppositeColour)
		{

			int iter = 1 + std::min(startLocation.y, 7 - startLocation.x);

			for (int i = 2; i < iter; i++)
			{
				if (board[startLocation.x + i][startLocation.y - i] == playedColour)
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
					board[startLocation.x + i][startLocation.y - i] = playedColour;
				}
			}

		}
	}

	flip = false;
	//check down right

	if (startLocation.x < BOARD_DIMENSION - 1 && startLocation.y < BOARD_DIMENSION - 1)
	{
		if (board[startLocation.x + 1][startLocation.y + 1] == oppositeColour)
		{
			//use the smaller of the start positions to decide how many times to loop, since that decides how far from and edge you are, if they are the same it doesnt matter
			int loopMax = 2 + std::min(7 - startLocation.x, 7 - startLocation.y);

			for (int i = 2; i < loopMax; i++)
			{
				if (board[startLocation.x + i][startLocation.y + i] == playedColour)
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
					board[startLocation.x + i][startLocation.y + i] = playedColour;
				}
			}

		}
	}

}

bool OthelloBoard::CheckMove(player_move moveToCheck)
{
	BOARD_SQUARE_STATE playedColour = currentPlayer;
	BOARD_SQUARE_STATE oppositeColour;
	//set the opposite colour as the opposite aas to what was played
	if (playedColour == BLUE)
	{
		oppositeColour = RED;
	}
	else
	{
		oppositeColour = BLUE;
	}

	//this code is super lazy, ive copypassted from the flip pieces function, ideally it would just use a  shared function with flip pieces but im too tired for the right now, so gonna have mega code duplication!

	//check if can flip on left
	if (moveToCheck.y >= 2) 
	{
		if (board[moveToCheck.x][moveToCheck.y - 1] == oppositeColour)
		{
			for (int i = 2; i <= moveToCheck.y; i++)
			{
				if (board[moveToCheck.x][moveToCheck.y - i] == playedColour)
				{
					return true;
				}
			}
		}
	}

	//check if can flip on right
	if (moveToCheck.y <= BOARD_DIMENSION - 2) 
	{
		if (board[moveToCheck.x][moveToCheck.y + 1] == oppositeColour)
		{
			for (int i = 2; i < BOARD_DIMENSION + 1; i++)
			{
				if (board[moveToCheck.x][moveToCheck.y + i] == playedColour)
				{
					return true;
				}
			}
		}
	}

	//check if can flip up
	if (moveToCheck.x >= 2) //it can only flip cells on the left if its beyond a point
	{
		if (board[moveToCheck.x - 1][moveToCheck.y] == oppositeColour)
		{
			for (int i = 2; i <= moveToCheck.x; i++)
			{
				if (board[moveToCheck.x - i][moveToCheck.y] == playedColour)
				{
					return true;
				}
			}
		}
	}

	//check if can flip down
	if (moveToCheck.x <= BOARD_DIMENSION - 2) //
	{
		if (board[moveToCheck.x + 1][moveToCheck.y] == oppositeColour)
		{
			for (int i = 2; i < BOARD_DIMENSION + 1; i++)
			{
				if (board[moveToCheck.x + i][moveToCheck.y] == playedColour)
				{
					return true;
				}
			}
		}
	}

	
	if (moveToCheck.x >= 2 && moveToCheck.y >= 2)
	{
		if (board[moveToCheck.x - 1][moveToCheck.y - 1] == oppositeColour)
		{
			//use the smaller of the start positions to decide how many times to loop, since that decides how far from and edge you are, if they are the same it doesnt matter
			int loopMax = std::min(moveToCheck.x, moveToCheck.y);

			for (int i = 2; i < 2 + loopMax; i++)
			{
				if (board[moveToCheck.x - i][moveToCheck.y - i] == playedColour)
				{
					return true;
				}
			}

		}
	}

	//check up right
	if (moveToCheck.y <= BOARD_DIMENSION - 2 && moveToCheck.x > 1)
	{
		if (board[moveToCheck.x - 1][moveToCheck.y + 1] == oppositeColour)
		{

			int iter = 2 + std::min(moveToCheck.x, 7 - moveToCheck.y);

			for (int i = 2; i < iter; i++)
			{
				if (board[moveToCheck.x - i][moveToCheck.y + i] == playedColour)
				{
					return true;
				}
			}
		}
	}


	//check down left
	if (moveToCheck.x <= BOARD_DIMENSION - 2 && moveToCheck.y > 1)
	{
		if (board[moveToCheck.x + 1][moveToCheck.y - 1] == oppositeColour)
		{

			int iter = 1 + std::min(moveToCheck.y, 7 - moveToCheck.x);

			for (int i = 2; i < iter; i++)
			{
				if (board[moveToCheck.x + i][moveToCheck.y - i] == playedColour)
				{
					return true;
				}
			}
		}
	}


	//down right
	if (moveToCheck.x < BOARD_DIMENSION - 1 && moveToCheck.y < BOARD_DIMENSION - 1)
	{
		if (board[moveToCheck.x + 1][moveToCheck.y + 1] == oppositeColour)
		{
			//use the smaller of the start positions to decide how many times to loop, since that decides how far from and edge you are, if they are the same it doesnt matter
			int loopMax = 1 + std::min(7 - moveToCheck.x, 7 - moveToCheck.y);

			for (int i = 2; i < loopMax; i++)
			{
				if (board[moveToCheck.x + i][moveToCheck.y + i] == playedColour)
				{
					return true;
				}
			}
		}
	}

	return false;
}

std::vector<std::pair<int, int>> OthelloBoard::GetPossibleMoves()
{
	std::vector<std::pair<int, int>> validMoves;

	player_move moveBeingChecked;
	//loop for all squares on board
	for (int i = 0; i < BOARD_DIMENSION + 1; i++)
	{
		for (int j = 0; j < BOARD_DIMENSION + 1; j++)
		{
			//logic check to see if a move is valid
			if(board[i][j] == NONE) //1. can only be valid if not played on yet
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

void OthelloBoard::UpdateValidMovesToBoard(std::vector<std::pair<int, int>> moves)
{
	for (int i = 0; i < moves.size(); i++)
	{
		board[moves[i].first][moves[i].second] = VALID;
	}


}

BOARD_SQUARE_STATE OthelloBoard::CheckWin()
{
	int blueSquares = 0;
	int redSquares = 0;

	bool gameFinished = true;

	for (int i = 0; i < BOARD_DIMENSION + 1; i++)
	{
		for (int j = 0; j < BOARD_DIMENSION + 1; j++)
		{

			switch (board[i][j])
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
		return BLUE;
	}
	if (redSquares > blueSquares)
	{
		return RED;
	}

	//if blue and red squares are tied return as such
	return DRAW;
}

BOARD_SQUARE_STATE OthelloBoard::getCurrentPlayer()
{
	return currentPlayer;
}

BOARD_SQUARE_STATE OthelloBoard::getSquareState(player_move position)
{
	return board[position.x][position.y];
}
