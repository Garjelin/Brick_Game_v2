#include <iostream>
#include <ncurses.h>


#include "../brick_game/snake/game.h"

// #include "console_view.h"
#include "../gui/console/console_controller.h"

void startGameLoop(s21::GameField* gameField, s21::ConsoleController* consoleController, s21::ConsoleView* consoleView, s21::HelpField* helpField);
s21::GameField* getGamefield();

int main() {
    initscr(); // Инициализация ncurses
    cbreak(); // Включение режима cbreak (буферизация не используется)
    noecho(); // Отключение отображения вводимых символов
    curs_set(0); // Скрыть курсор

    //GameField gameField(10, 20, 1);
    s21::GameField* gameField = getGamefield();
    gameField->StartNewGame();
    s21::ConsoleController consoleController;
    s21::ConsoleView consoleView;
    s21::HelpField helpField;
    startGameLoop(gameField, &consoleController, &consoleView, &helpField);
    
    endwin(); // Завершение работы с ncurses
    return 0;
}
