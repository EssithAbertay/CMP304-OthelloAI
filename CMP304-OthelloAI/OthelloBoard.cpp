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

	/*if (startLocation.x <= BOARD_DIMENSION - 2 && startLocation.y > 1)
	{
		if (board[startLocation.x + 1][startLocation.y - 1] == oppositeColour)
		{

			int iter = 2 + std::min(startLocation.y, 7 - startLocation.x);

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
	}*/

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
