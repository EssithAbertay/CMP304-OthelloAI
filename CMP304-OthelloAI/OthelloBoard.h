#pragma once
#include <iostream>
#include <vector>

#define BOARD_DIMENSION 7 //the board is actually 8*8 but we use 7 because 0 is used as the starting index


enum BOARD_SQUARE_STATE { NONE, BLUE, RED, DRAW }; //draw is only used so there is a return type for a draw

struct player_move
	{
		int x;
		int y;
	};


class OthelloBoard
{
	
const char* ANSI_RESET = "\033[0m";
const char* ANSI_WHITE = "\033[47m";
const char* ANSI_RED = "\033[41m";
const char* ANSI_BLUE = "\033[44m";
const char* ANSI_GREEN = "\033[42m";
	
public:
	//constructor and deconstuctor 
	OthelloBoard();
	~OthelloBoard();

	//functions
	void StartGame(BOARD_SQUARE_STATE startingPlayer); // start a game, the starting player is passed in
	void Setup(); // setup the board
	void Display(); // show the current state of the board
	void PlayTurn(player_move move); // play a turn
	void ChangeTurn(); //change the current player
	void FlipPieces(player_move startLocation); // after a move is made flip the ones inbetween own pieces

	//std::vector<std::pair<int, int>> GetPossibleMoves();  //add all the available moves into an array,mainly used by the ai later

	BOARD_SQUARE_STATE CheckWin(); // check if a player has won, return the winning player

	//getters and setters
	BOARD_SQUARE_STATE getCurrentPlayer();
	BOARD_SQUARE_STATE getSquareState(player_move position);

private:
	//variables go here
	BOARD_SQUARE_STATE board[8][8]{ { BOARD_SQUARE_STATE::NONE } };
	BOARD_SQUARE_STATE currentPlayer;
};

