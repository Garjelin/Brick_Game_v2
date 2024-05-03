#ifndef T_CONSOLE_CONTROLLER_H
#define T_CONSOLE_CONTROLLER_H

#include "../../brick_game/tetris/tetris_game.h"

class TConsoleController {
public:
    TConsoleController() {}
    ~TConsoleController() {}
    void userInput(UserAction_t action, bool hold);
    UserAction_t convertGetchToUserAction(int ch);
};

#endif // T_CONSOLE_CONTROLLER_H