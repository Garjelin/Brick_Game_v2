#ifndef CONSOLE_CONTROLLER_H
#define CONSOLE_CONTROLLER_H

#include <chrono>
#include <thread>
#include "../../brick_game/snake/game.h"
#include "console_view.h"

namespace s21 {
class ConsoleController {
public:
    ConsoleController() {}
    void userInput(s21::UserAction_t action, bool hold);
    s21::UserAction_t convertGetchToUserAction(int ch);
};
};

#endif // CONSOLE_CONTROLLER_H