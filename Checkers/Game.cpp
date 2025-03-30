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
		int direction = currentPlayer ? -1 : 1; // ����� ����� �����, ������ ����

		// ������� ��� (��� ������)
		if (!isCapture && abs(fromRow - toRow) == 1 && (toRow - fromRow) == direction) {
			return true;
		}

		// ��� �� �������
		if (abs(fromRow - toRow) == 2) {
			int midRow = (fromRow + toRow) / 2;
			int midCol = (fromCol + toCol) / 2;
			Cell midCell = board[midRow][midCol];

			// ��������, ��� ����� �������� ����� ����������
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

		// �������� ��������� ����� from � to
		for (int r = fromRow + rowStep, c = fromCol + colStep;
			r != toRow;
			r += rowStep, c += colStep) {
			Cell cell = board[r][c];
			if (cell != Cell::EMPTY) {
				// ���� ��� ����� �������� ������ - ��� ����������
				if ((currentPlayer && (cell == Cell::WHITE || cell == Cell::WHITE_KING)) ||
					(!currentPlayer && (cell == Cell::BLACK || cell == Cell::BLACK_KING))) {
					return false;
				}
				// ���� ��� ����� ���������� - ����������� �������
				enemyCount++;
			}
		}

		// ��� ����� ��� ������� ���� �� ������ ���� ����� �� ����
		if (!isCapture && enemyCount == 0) {
			return true;
		}
		// ��� ������ ������ ���� ����� ���� ����� ����������
		else if (isCapture && enemyCount == 1) {
			return true;
		}
	}

	return false;
}
void Game::makeMove(int fromRow, int fromCol, int toRow, int toCol) {
	bool isCapture = abs(fromRow - toRow) > 1;

	// ����������� �����
	board[toRow][toCol] = board[fromRow][fromCol];
	board[fromRow][fromCol] = Cell::EMPTY;

	// ���� ��� ���� ��������� �����
	if (isCapture) {
		// ��� ������� �����
		if (abs(fromRow - toRow) == 2) {
			int midRow = (fromRow + toRow) / 2;
			int midCol = (fromCol + toCol) / 2;
			board[midRow][midCol] = Cell::EMPTY;
		}
		// ��� �����
		else {
			int rowStep = (toRow > fromRow) ? 1 : -1;
			int colStep = (toCol > fromCol) ? 1 : -1;
			int enemyRow = -1, enemyCol = -1;

			// ������� ������� ��������� �����
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

			// ������� ��������� �����
			if (enemyRow != -1 && enemyCol != -1) {
				board[enemyRow][enemyCol] = Cell::EMPTY;
			}
		}
	}

	// �������� �� ����������� � �����
	checkPromotion(toRow, toCol);
}
bool Game::hasValidMoves() {
	for (int fromRow = 0; fromRow < size; ++fromRow) {
		for (int fromCol = 0; fromCol < size; ++fromCol) {
			Cell cell = board[fromRow][fromCol];
			// ���� ������ ����������� �������� ������
			if ((currentPlayer && (cell == Cell::WHITE || cell == Cell::WHITE_KING)) ||
				(!currentPlayer && (cell == Cell::BLACK || cell == Cell::BLACK_KING))) {
				// ��������� ��� ��������� ���� ��� ���� �����
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
		std::cout << "������ ��������!" << std::endl;
		return true;
	}
	else if (blackCount == 0) {
		std::cout << "����� ��������!" << std::endl;
		return true;
	}

	// ��������, ���� �� � �������� ������ ����
	if (!hasValidMoves()) {
		std::cout << (currentPlayer ? "�����" : "������") << " �� ����� ������� ���. ";
		std::cout << (currentPlayer ? "������" : "�����") << " ��������!" << std::endl;
		return true;
	}

	return false;
}
void Game::playGame() {
	createBoard();
	display();

	while (true) {
		std::cout << (currentPlayer ? "��� ����� (w/W)" : "��� ������ (b/B)") << std::endl;
		std::cout << "������� ���������� (fromRow fromCol toRow toCol): ";

		int fromRow, fromCol, toRow, toCol;
		std::cin >> fromRow >> fromCol >> toRow >> toCol;

		

		// �������� ����
		if (isValidMove(fromRow, fromCol, toRow, toCol, false) ||
			isValidMove(fromRow, fromCol, toRow, toCol, true)) {
			makeMove(fromRow, fromCol, toRow, toCol);
			display();

			// �������� �� ������
			if (checkWin()) {
				break;
			}

			// ����� ������
			currentPlayer = !currentPlayer;
		}
		else {
			std::cout << "������������ ���. ���������� �����." << std::endl;
		}
	}
}

