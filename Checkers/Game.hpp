#pragma once
#ifndef GAME_HPP
#define GAME_HPP
#include "Board.hpp"
class Game {
public:
    Game(); // Конструктор для инициализации игры

    bool isValidPosition(int row, int col) const;
    void checkPromotion(int row, int col);
    bool isValidMove(int fromRow, int fromCol, int toRow, int toCol, bool isCapture) const;
    void makeMove(int fromRow, int fromCol, int toRow, int toCol);
    bool hasValidMoves() const;
    bool checkWin() const;
    void playGame();

private:
    Board gameBoard; // Композиция вместо наследования
    bool currentPlayer = true; // true для одного игрока, false для другого
};
#endif // !GAME_HPP

