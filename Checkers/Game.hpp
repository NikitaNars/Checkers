#pragma once
#ifndef GAME_HPP
#define GAME_HPP
#include "Board.hpp"
class Game {
public:
    Game(); // ����������� ��� ������������� ����

    bool isValidPosition(int row, int col) const;
    void checkPromotion(int row, int col);
    bool isValidMove(int fromRow, int fromCol, int toRow, int toCol, bool isCapture) const;
    void makeMove(int fromRow, int fromCol, int toRow, int toCol);
    bool hasValidMoves() const;
    bool checkWin() const;
    void playGame();

private:
    Board gameBoard; // ���������� ������ ������������
    bool currentPlayer = true; // true ��� ������ ������, false ��� �������
};
#endif // !GAME_HPP

