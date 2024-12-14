#include <iostream>
#include "OthelloBoard.h"



bool validateMove(OthelloBoard *board,player_move p_move)
{
	if (board->getSquareState(p_move) != VALID)
	{
		std::cout << "Choose a valid square (green)" <<std::endl;
		return false;
	}
	return true;
}

void main()
{
	OthelloBoard gameBoard;
	player_move move;

	int x = 9;
	int y = 9;

	gameBoard.StartGame(BLUE);

	gameBoard.Setup();


	std::cout<< "\033[44m" << "Blue Starts!" << std::endl;
	std::cout << "\033[0m";

	gameBoard.UpdateValidMovesToBoard(gameBoard.GetPossibleMoves());
		
	gameBoard.Display();


	//idk why but x and y are flipped,can't bothered dealing with it so will just pretend they are not
	while (gameBoard.CheckWin() == NONE)
	{
		do
		{
			std::cout << "Enter X move:" << std::endl;
			std::cin >> move.y;
			std::cout << "Enter Y move:" << std::endl;
			std::cin >> move.x;
		} while (!validateMove(&gameBoard, move));


		gameBoard.PlayTurn(move);



		gameBoard.ChangeTurn();	
		
		gameBoard.UpdateValidMovesToBoard(gameBoard.GetPossibleMoves());

		gameBoard.Display();

	}
	return;
}