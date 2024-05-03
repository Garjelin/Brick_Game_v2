#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <QObject>
#include <QTimer>
#include <QApplication>
#include "../../brick_game/snake/game.h"
#include "helpfield.h"
#include "view.h"

namespace s21 {
class GameController : public QObject
{
    Q_OBJECT
public:
    GameController(QObject *parent, HelpField* helpField, View* view);
    ~GameController();

    void stopTimer();
    void updateTimer();

private:
    //GameField* m_gameField;
    HelpField* m_helpField;
    View* m_view;
    s21::UserAction_t action;

    QTimer *m_timer;
    void userInput(s21::UserAction_t action, bool hold);

public slots:
    void convertKeyEventToUserAction(QKeyEvent *e);
};
};

#endif // GAMECONTROLLER_H
