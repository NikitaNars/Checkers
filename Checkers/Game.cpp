#include "Game.hpp"
bool Game::isValidPosition(int row, int col)
{
	if (row >= 0 && row < size && col >= 0 && col < size)
	{
		return true;
	}
	return false;
}
void Game::checkPromotion(int row, int col)
{
	if (board[row][col] == Cell::WHITE && row == 0)
	{
		board[row][col] = Cell::WHITE_KING;
	}
	else if (board[row][col] == Cell::BLACK && row == size - 1)
	{
		board[row][col] = Cell::BLACK_KING;
	}
}
bool Game::isValidMove(int fromRow, int fromCol, int toRow, int toCol, bool isCapture)
{
	if (!(isValidPosition(fromRow, fromCol) || isValidPosition(toRow, toCol)))
	{
		return false;
	}
	Cell fromCell = board[fromRow][fromCol];
	if ((fromCell == Cell::EMPTY || currentPlayer && fromCell != Cell::WHITE && fromCell != Cell::WHITE_KING) ||
		!currentPlayer && fromCell != Cell::BLACK && fromCell != Cell::BLACK_KING)
	{
		return false;
	}
	if (board[toRow][toCol] != Cell::EMPTY) {
		return false;
	}
	if (abs(fromRow - toRow) != abs(fromCol - toCol)) {
		return false;
	}
	if (fromCell == Cell::WHITE || fromCell == Cell::BLACK) {
		int direction = currentPlayer ? -1 : 1; // Белые ходят вверх, черные вниз

		// Обычный ход (без взятия)
		if (!isCapture && abs(fromRow - toRow) == 1 && (toRow - fromRow) == direction) {
			return true;
		}

		// Ход со взятием
		if (abs(fromRow - toRow) == 2) {
			int midRow = (fromRow + toRow) / 2;
			int midCol = (fromCol + toCol) / 2;
			Cell midCell = board[midRow][midCol];

			// Проверка, что между клетками шашка противника
			if ((currentPlayer && (midCell == Cell::BLACK || midCell == Cell::BLACK_KING)) ||
				(!currentPlayer && (midCell == Cell::WHITE || midCell == Cell::WHITE_KING))) {
				return true;
			}
		}
	}
	else if (fromCell == Cell::WHITE_KING || fromCell == Cell::BLACK_KING) {
		int rowStep = (toRow > fromRow) ? 1 : -1;
		int colStep = (toCol > fromCol) ? 1 : -1;
		int enemyCount = 0;

		// Проверка диагонали между from и to
		for (int r = fromRow + rowStep, c = fromCol + colStep;
			r != toRow;
			r += rowStep, c += colStep) {
			Cell cell = board[r][c];
			if (cell != Cell::EMPTY) {
				// Если это шашка текущего игрока - ход невозможен
				if ((currentPlayer && (cell == Cell::WHITE || cell == Cell::WHITE_KING)) ||
					(!currentPlayer && (cell == Cell::BLACK || cell == Cell::BLACK_KING))) {
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
	board[toRow][toCol] = board[fromRow][fromCol];
	board[fromRow][fromCol] = Cell::EMPTY;

	// Если был взят вражеская шашка
	if (isCapture) {
		// Для обычных шашек
		if (abs(fromRow - toRow) == 2) {
			int midRow = (fromRow + toRow) / 2;
			int midCol = (fromCol + toCol) / 2;
			board[midRow][midCol] = Cell::EMPTY;
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
				Cell cell = board[r][c];
				if (cell != Cell::EMPTY) {
					enemyRow = r;
					enemyCol = c;
					break;
				}
			}

			// Удаляем вражескую шашку
			if (enemyRow != -1 && enemyCol != -1) {
				board[enemyRow][enemyCol] = Cell::EMPTY;
			}
		}
	}

	// Проверка на превращение в дамку
	checkPromotion(toRow, toCol);
}
bool Game::hasValidMoves() {
	for (int fromRow = 0; fromRow < size; ++fromRow) {
		for (int fromCol = 0; fromCol < size; ++fromCol) {
			Cell cell = board[fromRow][fromCol];
			// Если клетка принадлежит текущему игроку
			if ((currentPlayer && (cell == Cell::WHITE || cell == Cell::WHITE_KING)) ||
				(!currentPlayer && (cell == Cell::BLACK || cell == Cell::BLACK_KING))) {
				// Проверяем все возможные ходы для этой шашки
				for (int toRow = 0; toRow < size; ++toRow) {
					for (int toCol = 0; toCol < size; ++toCol) {
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
bool Game::checkWin() {
	int whiteCount = 0, blackCount = 0;
	for (int row = 0; row < size; ++row) {
		for (int col = 0; col < size; ++col) {
			if (board[row][col] == Cell::WHITE || board[row][col] == Cell::WHITE_KING) {
				whiteCount++;
			}
			else if (board[row][col] == Cell::BLACK || board[row][col] == Cell::BLACK_KING) {
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
	createBoard();
	display();

	while (true) {
		std::cout << (currentPlayer ? "Ход белых (w/W)" : "Ход черных (b/B)") << std::endl;
		std::cout << "Введите координаты (fromRow fromCol toRow toCol): ";

		int fromRow, fromCol, toRow, toCol;
		std::cin >> fromRow >> fromCol >> toRow >> toCol;

		

		// Проверка хода
		if (isValidMove(fromRow, fromCol, toRow, toCol, false) ||
			isValidMove(fromRow, fromCol, toRow, toCol, true)) {
			makeMove(fromRow, fromCol, toRow, toCol);
			display();

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

