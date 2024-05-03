#ifndef CONSOLE_VIEW_H
#define CONSOLE_VIEW_H

#include "../../brick_game/snake/game.h"
#include "../../brick_game/tetris/tetris_game.h"
#include <ncurses.h>

// namespace s21 {

// class HelpField {
// public:
//     HelpField() {}
//     void draw();

// private:
//     //GameField* m_gameField;
// };


class ConsoleView {
public:
    ConsoleView() {}
    void draw();
    // void GameOver();
};
// };

#endif // CONSOLE_VIEW_H