#include "console_controller.h"

s21::GameField* getGamefield();

void s21::ConsoleController::userInput(s21::UserAction_t action, bool hold) {
    s21::GameField* m_gameField = getGamefield();
    switch (action) {
        case s21::Pause:
            m_gameField->ReversePause();
            if (m_gameField->isPause()) {
                //timeout(-1);
                std::this_thread::sleep_for(std::chrono::milliseconds(-1));
            }
            else {
                //timeout(300);
                std::this_thread::sleep_for(std::chrono::milliseconds(m_gameField->getSpeed()));
            }
            break;
        case s21::Terminate: 
            m_gameField->GameOver();
            break;
        case s21::Up: 
            if (m_gameField->getSnakeDirection() != s21::Snake::SnakeDirection::up && m_gameField->getSnakeDirection() != s21::Snake::SnakeDirection::down) m_gameField->setSnakeDirection(s21::Snake::SnakeDirection::up);
            break;
        case s21::Down: 
            if (m_gameField->getSnakeDirection() != s21::Snake::SnakeDirection::down && m_gameField->getSnakeDirection() != s21::Snake::SnakeDirection::up) m_gameField->setSnakeDirection(s21::Snake::SnakeDirection::down);
            break;
        case s21::Left: 
            if (m_gameField->getSnakeDirection() != s21::Snake::SnakeDirection::left && m_gameField->getSnakeDirection() != s21::Snake::SnakeDirection::right) m_gameField->setSnakeDirection(s21::Snake::SnakeDirection::left);
            break;
        case s21::Right: 
            if (m_gameField->getSnakeDirection() != s21::Snake::SnakeDirection::right && m_gameField->getSnakeDirection() != s21::Snake::SnakeDirection::left) m_gameField->setSnakeDirection(s21::Snake::SnakeDirection::right);
            break;
        case s21::Start:
            break;
        case s21::Action:
            break;
        case s21::None:
            break;
        }
            // napms(400);
        (void)hold;
}

s21::UserAction_t s21::ConsoleController::convertGetchToUserAction(int ch) {
    switch (ch) {
        case 'x':
            return s21::Start;
        case ' ':
            return s21::Pause;
        case 'q':
            return s21::Terminate;
        case 'a':
            return s21::Left;
        case 'd':
            return s21::Right;
        case 'w':
            return s21::Up;
        case 's':
            return s21::Down;
        case 'c':
            return s21::Action;
        default:
            return s21::None;
    }
}

void startGameLoop(s21::GameField* gameField, s21::ConsoleController* consoleController, s21::ConsoleView* consoleView, s21::HelpField* helpField) {
    timeout(1); // Установка времени ожидания ввода
    while (!gameField->isGameOver()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(gameField->getSpeed()));
        s21::UserAction_t action = consoleController->convertGetchToUserAction(getch());
        if (!gameField->isPause() || (action == s21::Pause) || (action == s21::Terminate)) { // Проверяем, не находится ли игра на паузе
            if ((action == s21::Down ) || (action == s21::Up) || (action == s21::Left) || (action == s21::Right) || (action == s21::Pause) || (action == s21::Terminate)) {
                gameField->setIsMoveBlocked_TRUE();
                consoleController->userInput(action, false);
            } 
            gameField->MoveAndCheck();
        }
        consoleView->draw();     
        helpField->draw();
        
        
        // Если игра окончена, выводим "Game Over"
        if (gameField->isGameOver()) {
            consoleView->GameOver();
            while ((action = consoleController->convertGetchToUserAction(getch())) != s21::Pause && action != s21::Terminate) {
            }
            if (action == s21::Terminate) {
                break;
            } else {
                gameField->StartNewGame();
                gameField->ReversePause();
            }
        }
        
    }
    
}
