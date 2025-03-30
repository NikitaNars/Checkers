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
        EMPTY,      // ������ ������
        WHITE,      // ����� �����
        WHITE_KING, // ����� �����
        BLACK,      // ������ �����
        BLACK_KING  // ������ �����
    };
    std::vector<std::vector<Cell>> board;
};

