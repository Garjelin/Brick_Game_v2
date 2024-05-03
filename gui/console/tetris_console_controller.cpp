#include "tetris_console_controller.h"

GameField* getGamefield();

UserAction_t TConsoleController::convertGetchToUserAction(int ch) {
    switch (ch) {
        case 'e':
            return Start;
        case ' ':
            return Pause;
        case 'q':
            return Terminate;
        case 'a':
            return Left;
        case 'd':
            return Right;
        case 'r':
            return Up;
        case 's':
            return Down;
        case 'w':
            return Action;
        default:
            return None;
    }
}

void TConsoleController::userInput(UserAction_t action, bool hold) {
    GameField* game = getGamefield();
    switch (action) {
        case Pause:
            break;
        case Terminate: 
            game->setGameOver();
            break;
        case Up: 
            break;
        case Down: 
            game->getFigure()->MoveFig(0, 1);
            break;
        case Left: 
            game->getFigure()->MoveFig(-1, 0);
            break;
        case Right: 
            game->getFigure()->MoveFig(1, 0);
            break;
        case Start:
            break;
        case Action:
            game->getFigure()->TurnSet(game->getFigure()->TurnGet() + 1);
            break;
        case None:
            break;
        }
        (void)hold;
}

void startGameLoop(GameField* game, TConsoleController* tConsoleController) {
    timeout(1);
    while (!game->isGameOver()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        UserAction_t action = tConsoleController->convertGetchToUserAction(getch());
        tConsoleController->userInput(action, false);
        
        //game->MoveGame();

        static int tick = 0;
        tick++;
        if (tick >=50) {
            if (!game->getFigure()->MoveFig(0,1)){
                game->getFigure()->PutField(game->getField()->fld);
                game->getFigure()->Shape(game->shpArr[rand() % game->shpArrCnt]);
                game->getFigure()->Pos(game->getWidth()/2 - shp_width/2, 0);
                if (game->getFigure()->Check() > 0) {
                    game->setGameOver();
                    //field.FillScr();
                }
            }
            game->getField()->Burning();
            tick = 0;
        }

        //game->Show();

        erase();
        game->getField()->Put(game->getScreen()->getScreenMap());
        game->getFigure()->Put(game->getScreen()->getScreenMap());
        game->getScreen()->Show();
        refresh(); 
    }
}