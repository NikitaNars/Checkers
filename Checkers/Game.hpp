#pragma once
#include "Board.hpp"
class  Game : Board
{
public:
	bool isValidPosition(int row, int col);
	void checkPromotion(int row, int col);
	bool isValidMove(int fromRow, int fromCol, int toRow, int toCol, bool isCapture);
	void makeMove(int fromRow, int fromCol, int toRow, int toCol);
	bool hasValidMoves();
	bool checkWin();
	void  playGame();
private:
	bool currentPlayer = true;

};

