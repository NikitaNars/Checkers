#include "Board.hpp"
void Board::createBoard()
{
	board.resize(size, std::vector<Cell>(size, Cell::EMPTY));
    // Расстановка черных шашек
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < size; ++col) {
            if ((row + col) % 2 == 1) {
                board[row][col] = Cell::BLACK;
            }
        }
    }

    // Расстановка белых шашек
    for (int row = 5; row < size; ++row) {
        for (int col = 0; col < size; ++col) {
            if ((row + col) % 2 == 1) {
                board[row][col] = Cell::WHITE;
            }
        }
    }
}
void Board::display()
{
    std::cout << "  ";
    for (int col = 0; col < size; ++col) {
        std::cout << col << " ";
    }
    std::cout << std::endl;

    for (int row = 0; row < size; ++row) {
        std::cout << row << " ";
        for (int col = 0; col < size; ++col) {
            switch (board[row][col]) {
            case Cell::EMPTY:
                std::cout << ". ";
                break;
            case Cell::WHITE:
                std::cout << "w ";
                break;
            case Cell::WHITE_KING:
                std::cout << "W ";
                break;
            case Cell::BLACK:
                std::cout << "b ";
                break;
            case Cell::BLACK_KING:
                std::cout << "B ";
                break;
            }
        }
        std::cout << std::endl;
    }

}