#pragma once

// define enum's and structs used in the game
enum BOARD_SQUARE_STATE { NONE, BLUE, RED, DRAW};

#define BOARD_DIMENSION 7 //the board is actually 8*8 but we use 7 because 0 is used as the starting index


struct GameBoard
{
	BOARD_SQUARE_STATE board[8][8] =  { NONE, NONE, NONE, NONE,NONE, NONE, NONE, NONE,
										NONE, NONE, NONE, NONE,NONE, NONE, NONE, NONE,
										NONE, NONE, NONE, NONE,NONE, NONE, NONE, NONE,
										NONE, NONE, NONE, BLUE,RED, NONE, NONE, NONE,
										NONE, NONE, NONE, RED,BLUE, NONE, NONE, NONE,
										NONE, NONE, NONE, NONE,NONE, NONE, NONE, NONE,
										NONE, NONE, NONE, NONE,NONE, NONE, NONE, NONE,
										NONE, NONE, NONE, NONE,NONE, NONE, NONE, NONE,
	}; 
};

struct GameAction
{
	int x, y;
	BOARD_SQUARE_STATE playerMove;

public:
	GameAction(int newX, int newY, BOARD_SQUARE_STATE move)
	{
		x = newX;
		y = newY;
		playerMove = move;
	}

	GameAction()
	{
		playerMove = BOARD_SQUARE_STATE::NONE;
		x = 0;
		y = 0;
	}

};

