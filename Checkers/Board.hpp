#pragma once
#ifndef BOARD_HPP
#define BOARD_HPP

#include <iostream>
#include <vector>

class Board {
public:
    enum class Cell {
        EMPTY,      // Пустая клетка
        WHITE,      // Белая шашка
        WHITE_KING, // Белая дамка
        BLACK,      // Черная шашка
        BLACK_KING  // Черная дамка
    };

    Board(); // Конструктор создает доску

    void display() const;
    Cell getCell(int row, int col) const;
    void setCell(int row, int col, Cell value);
    int getSize() const;
    void initializeBoard(); // Вспомогательный метод для инициализации доски
private:
    const int size = 8;
    std::vector<std::vector<Cell>> board;


};


#endif // !BOARD_HPP


