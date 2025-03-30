#pragma once
#ifndef BOARD_HPP
#define BOARD_HPP

#include <iostream>
#include <vector>

class Board {
public:
    enum class Cell {
        EMPTY,      // ������ ������
        WHITE,      // ����� �����
        WHITE_KING, // ����� �����
        BLACK,      // ������ �����
        BLACK_KING  // ������ �����
    };

    Board(); // ����������� ������� �����

    void display() const;
    Cell getCell(int row, int col) const;
    void setCell(int row, int col, Cell value);
    int getSize() const;
    void initializeBoard(); // ��������������� ����� ��� ������������� �����
private:
    const int size = 8;
    std::vector<std::vector<Cell>> board;


};


#endif // !BOARD_HPP


