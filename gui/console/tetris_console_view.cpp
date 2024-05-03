#include "tetris_console_view.h"

// s21::GameField* getGamefield() {
//         static s21::GameField gameField(10, 20, 1); 
//         return &gameField;
// }

// void s21::HelpField::draw() {
//     s21::GameField* m_gameField = getGamefield();
//     s21::GameInfo_t info = m_gameField->updateCurrentState();
//     mvprintw(0, m_gameField->getWidth() + 2, "Record: %d", info.high_score);
//     mvprintw(2, m_gameField->getWidth() + 2, "Score: %d", info.score);
//     mvprintw(4, m_gameField->getWidth() + 2, "Level: %d", info.level);
//     mvprintw(6, m_gameField->getWidth() + 2, "Pause - SPACE");
//     refresh(); // Обновление экрана
// }

// void s21::ConsoleView::draw() {
void ConsoleView::draw() {
    // s21::GameField* m_gameField = getGamefield();
    // s21::GameInfo_t info = m_gameField->updateCurrentState();
    // clear(); 
    // for (int y = 0; y < m_gameField->getHeight(); ++y) {
    //     for (int x = 0; x < m_gameField->getWidth(); ++x) {
    //         if (info.field[x][y] == 2) {
    //             mvprintw(y, x, "*"); 
    //         } else if (info.field[x][y] == 1) {
    //             mvprintw(y, x, "O"); 
    //         } else {
    //             mvprintw(y, x, "."); 
    //         }
    //     }
    // }
    // m_gameField->setIsMoveBlocked_FALSE();
    // refresh(); 
}

// void s21::ConsoleView::GameOver() {
//     s21::GameField* m_gameField = getGamefield();
//     mvprintw(m_gameField->getHeight() / 2, m_gameField->getWidth() / 2 - 4, "Game Over");
//     refresh();
// }
