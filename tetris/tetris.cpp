#include "../brick_game/tetris/tetris_game.h"
#include "../gui/console/tetris_console_controller.h"
GameField* getGamefield() {
    static GameField game(10, 20); 
    return &game;
}

void startGameLoop(GameField* game, TConsoleController* tConsoleController);

int main(void)
{
    initscr(); // инициализация экрана
    cbreak(); // Включение режима cbreak (буферизация не используется)
    noecho(); // Отключение отображения вводимых символов
    curs_set(0); // Скрыть курсор

    GameField* game = getGamefield();
    TConsoleController tConsoleController;
    //TScreen* screen = new TScreen('\0');
    startGameLoop(game, &tConsoleController);
    //

    endwin(); // закрытие экрана
    return 0;
}