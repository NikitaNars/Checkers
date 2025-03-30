#include "Game.hpp"
int main()
{
    setlocale(LC_ALL, "Russian");
    std::cout << "Добро пожаловать в игру шашки!" << std::endl;
    std::cout << "Правила:" << std::endl;
    std::cout << "- Белые (w) ходят первыми" << std::endl;
    std::cout << "- Обычные шашки ходят только вперед" << std::endl;
    std::cout << "- Чтобы сделать ход, введите координаты в формате: fromRow fromCol toRow toCol" << std::endl;
    std::cout << "- Чтобы сделать ход, введите координаты в формате: fromRow fromCol toRow toCol" << std::endl;
    std::cout << "Например: 5 0 4 1 - переместить шашку с (5,0) на (4,1)" << std::endl << std::endl;
    Game newGame;
    newGame.playGame();

    return 0;
}