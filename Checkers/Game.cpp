#include "Game.hpp"
#include <iostream>

Game::Game()  {

    gameBoard.initializeBoard();
}

bool Game::isValidPosition(int row, int col) const {
    return row >= 0 && row < gameBoard.getSize() && col >= 0 && col < gameBoard.getSize();
}

void Game::checkPromotion(int row, int col) {
    Board::Cell cell = gameBoard.getCell(row, col);

    if (cell == Board::Cell::WHITE && row == 0) 
    {
        gameBoard.setCell(row, col, Board::Cell::WHITE_KING);
    }
    else if (cell == Board::Cell::BLACK && row == gameBoard.getSize() - 1) 
    {
        gameBoard.setCell(row, col, Board::Cell::BLACK_KING);
    }
}

bool Game::isValidMove(int fromRow, int fromCol, int toRow, int toCol, bool isCapture) const {
    if (!isValidPosition(fromRow, fromCol) || !isValidPosition(toRow, toCol)) 
    {
        return false;
    }

    Board::Cell fromCell = gameBoard.getCell(fromRow, fromCol);
    if (fromCell == Board::Cell::EMPTY) {
        return false;
    }

    // Проверка принадлежности шашки текущему игроку
    if ((currentPlayer && fromCell != Board::Cell::WHITE && fromCell != Board::Cell::WHITE_KING) ||
        (!currentPlayer && fromCell != Board::Cell::BLACK && fromCell != Board::Cell::BLACK_KING)) {
        return false;
    }

    if (gameBoard.getCell(toRow, toCol) != Board::Cell::EMPTY) {
        return false;
    }

    if (abs(fromRow - toRow) != abs(fromCol - toCol)) {
        return false;
    }

    if (fromCell == Board::Cell::WHITE || fromCell == Board::Cell::BLACK) {
        int direction = currentPlayer ? -1 : 1; // Белые ходят вверх, черные вниз

        // Обычный ход 
        if (!isCapture && abs(fromRow - toRow) == 1 && (toRow - fromRow) == direction) {
            return true;
        }

        // Ход со убийством
        if (abs(fromRow - toRow) == 2) {
            int midRow = (fromRow + toRow) / 2;
            int midCol = (fromCol + toCol) / 2;
            Board::Cell midCell = gameBoard.getCell(midRow, midCol);

            // Проверка, что между клетками шашка противника
            if ((currentPlayer && (midCell == Board::Cell::BLACK || midCell == Board::Cell::BLACK_KING)) ||
                (!currentPlayer && (midCell == Board::Cell::WHITE || midCell == Board::Cell::WHITE_KING))) {
                return true;
            }
        }
    }
    else if (fromCell == Board::Cell::WHITE_KING || fromCell == Board::Cell::BLACK_KING) {
        int rowStep = (toRow > fromRow) ? 1 : -1;
        int colStep = (toCol > fromCol) ? 1 : -1;
        int enemyCount = 0;

        // Проверка диагонали между from и to
        for (int r = fromRow + rowStep, c = fromCol + colStep;  r != toRow; r += rowStep, c += colStep) {
            Board::Cell cell = gameBoard.getCell(r, c);
            if (cell != Board::Cell::EMPTY) {
                // Если это шашка текущего игрока - ход невозможен
                if ((currentPlayer && (cell == Board::Cell::WHITE || cell == Board::Cell::WHITE_KING)) ||
                    (!currentPlayer && (cell == Board::Cell::BLACK || cell == Board::Cell::BLACK_KING))) {
                    return false;
                }
                // Если это шашка противника - увеличиваем счетчик
                enemyCount++;
            }
        }

        // Для дамки при обычном ходе не должно быть шашек на пути
        if (!isCapture && enemyCount == 0) {
            return true;
        }
        // При взятии должна быть ровно одна шашка противника
        else if (isCapture && enemyCount == 1) {
            return true;
        }
    }

    return false;
}

void Game::makeMove(int fromRow, int fromCol, int toRow, int toCol) {
    bool isCapture = abs(fromRow - toRow) > 1;

    // Перемещение шашки
    gameBoard.setCell(toRow, toCol, gameBoard.getCell(fromRow, fromCol));
    gameBoard.setCell(fromRow, fromCol, Board::Cell::EMPTY);

    // Если был взят вражеская шашка
    if (isCapture) {
        // Для обычных шашек
        if (abs(fromRow - toRow) == 2) {
            int midRow = (fromRow + toRow) / 2;
            int midCol = (fromCol + toCol) / 2;
            gameBoard.setCell(midRow, midCol, Board::Cell::EMPTY);
        }
        // Для дамок
        else {
            int rowStep = (toRow > fromRow) ? 1 : -1;
            int colStep = (toCol > fromCol) ? 1 : -1;
            int enemyRow = -1, enemyCol = -1;

            // Находим позицию вражеской шашки
            for (int r = fromRow + rowStep, c = fromCol + colStep;
                r != toRow;
                r += rowStep, c += colStep) {
                Board::Cell cell = gameBoard.getCell(r, c);
                if (cell != Board::Cell::EMPTY) {
                    enemyRow = r;
                    enemyCol = c;
                    break;
                }
            }

            // Удаляем вражескую шашку
            if (enemyRow != -1 && enemyCol != -1) {
                gameBoard.setCell(enemyRow, enemyCol, Board::Cell::EMPTY);
            }
        }
    }

    // Проверка на превращение в дамку
    checkPromotion(toRow, toCol);
}

bool Game::hasValidMoves() const {
    for (int fromRow = 0; fromRow < gameBoard.getSize(); ++fromRow) {
        for (int fromCol = 0; fromCol < gameBoard.getSize(); ++fromCol) {
            Board::Cell cell = gameBoard.getCell(fromRow, fromCol);
            // Если клетка принадлежит текущему игроку
            if ((currentPlayer && (cell == Board::Cell::WHITE || cell == Board::Cell::WHITE_KING)) ||
                (!currentPlayer && (cell == Board::Cell::BLACK || cell == Board::Cell::BLACK_KING))) {
                // Проверяем все возможные ходы для этой шашки
                for (int toRow = 0; toRow < gameBoard.getSize(); ++toRow) {
                    for (int toCol = 0; toCol < gameBoard.getSize(); ++toCol) {
                        if (isValidMove(fromRow, fromCol, toRow, toCol, false) ||
                            isValidMove(fromRow, fromCol, toRow, toCol, true)) {
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}

bool Game::checkWin()const {
    int whiteCount = 0, blackCount = 0;
    for (int row = 0; row < gameBoard.getSize(); ++row) {
        for (int col = 0; col < gameBoard.getSize(); ++col) {
            Board::Cell cell = gameBoard.getCell(row, col);
            if (cell == Board::Cell::WHITE || cell == Board::Cell::WHITE_KING) {
                whiteCount++;
            }
            else if (cell == Board::Cell::BLACK || cell == Board::Cell::BLACK_KING) {
                blackCount++;
            }
        }
    }

    if (whiteCount == 0) {
        std::cout << "Черные победили!" << std::endl;
        return true;
    }
    else if (blackCount == 0) {
        std::cout << "Белые победили!" << std::endl;
        return true;
    }

    // Проверка, есть ли у текущего игрока ходы
    if (!hasValidMoves()) {
        std::cout << (currentPlayer ? "Белые" : "Черные") << " не могут сделать ход. ";
        std::cout << (currentPlayer ? "Черные" : "Белые") << " победили!" << std::endl;
        return true;
    }

    return false;
}

void Game::playGame() {
    gameBoard.display();

    while (true) {
        std::cout << (currentPlayer ? "Ход белых (w/W)" : "Ход черных (b/B)") << std::endl;
        std::cout << "Введите координаты (fromRow fromCol toRow toCol): ";

        int fromRow, fromCol, toRow, toCol;
        std::cin >> fromRow >> fromCol >> toRow >> toCol;

        // Проверка хода
        if (isValidMove(fromRow, fromCol, toRow, toCol, false) ||
            isValidMove(fromRow, fromCol, toRow, toCol, true)) {
            makeMove(fromRow, fromCol, toRow, toCol);
            gameBoard.display();

            // Проверка на победу
            if (checkWin()) {
                break;
            }

            // Смена игрока
            currentPlayer = !currentPlayer;
        }
        else {
            std::cout << "Некорректный ход. Попробуйте снова." << std::endl;
        }
    }
}
