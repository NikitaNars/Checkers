#pragma once
#include <iostream>
#include <vector>

static class Board
{
public:
    
	void createBoard();
    void display();
protected:
    
    const int size = 8;
    enum class Cell {
        EMPTY,      // Пустая клетка
        WHITE,      // Белая шашка
        WHITE_KING, // Белая дамка
        BLACK,      // Черная шашка
        BLACK_KING  // Черная дамка
    };
    std::vector<std::vector<Cell>> board;
};

