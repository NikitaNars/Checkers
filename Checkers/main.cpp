#include "Game.hpp"
int main()
{
    setlocale(LC_ALL, "Russian");
    std::cout << "����� ���������� � ���� �����!" << std::endl;
    std::cout << "�������:" << std::endl;
    std::cout << "- ����� (w) ����� �������" << std::endl;
    std::cout << "- ������� ����� ����� ������ ������" << std::endl;
    std::cout << "- ����� ������� ���, ������� ���������� � �������: fromRow fromCol toRow toCol" << std::endl;
    std::cout << "- ����� ������� ���, ������� ���������� � �������: fromRow fromCol toRow toCol" << std::endl;
    std::cout << "��������: 5 0 4 1 - ����������� ����� � (5,0) �� (4,1)" << std::endl << std::endl;
    Game newGame;
    newGame.playGame();

    return 0;
}