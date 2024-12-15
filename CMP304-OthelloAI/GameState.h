#pragma once

#include <iostream>
#include <vector>
#include "BoardAndAction.h"


class GameState
{
public:
	GameBoard gameBoard;
	GameAction gameAction;

	const char* ANSI_RESET = "\033[0m";
	const char* ANSI_WHITE = "\033[47m";
	const char* ANSI_RED = "\033[41m";
	const char* ANSI_BLUE = "\033[44m";
	const char* ANSI_GREEN = "\033[42m";

	GameState();
	void setAndApplyAction(GameAction newAction);
	void FlipPieces(); // after a move is made flip the ones inbetween own pieces


	std::vector<std::pair<int, int>> getPossibleMoves(GameAction moveBeingChecked);
	bool CheckMove(GameAction moveToCheck);

	void printBoard();
	BOARD_SQUARE_STATE checkWin();
};

