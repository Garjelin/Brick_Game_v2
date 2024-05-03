#include "gamecontroller.h"
#include <QKeyEvent>
#include <QDebug>

s21::GameController::GameController(QObject *parent, HelpField *helpField, View *view)
    : QObject(parent), m_helpField(helpField), m_view(view)
{
    s21::GameField* m_gameField = s21::getGamefield();
    // Подключение сигнала keyPressEvent к слоту handleKeyPress
    connect(m_view, &View::keyPressed, this, &GameController::convertKeyEventToUserAction);

    auto timerCallback = [=]() {(m_gameField->*(&s21::GameField::MoveAndCheck))();};

    m_timer = new QTimer(this);


    //connect(m_timer, &QTimer::timeout, m_gameField, &GameField::MoveAndCheck);
    connect(m_timer, &QTimer::timeout, timerCallback);

    //m_timer->start(m_gameField->getSpeed());

}

s21::GameController::~GameController(){ delete m_timer; }

void s21::GameController::stopTimer() { m_timer->stop(); }
void s21::GameController::updateTimer() {
    s21::GameField* m_gameField = getGamefield();
    m_timer->start(m_gameField->getSpeed());
}

void s21::GameController::convertKeyEventToUserAction(QKeyEvent *e) {
    s21::UserAction_t action;
    switch (e->key()) {
        case Qt::Key_Enter:
            action = s21::Start;
            break;
        case Qt::Key_Space:
            action = s21::Pause;
            break;
        case Qt::Key_Escape:
            action = s21::Terminate;
            break;
        case Qt::Key_Left:
            action = s21::Left;
            break;
        case Qt::Key_Right:
            action = s21::Right;
            break;
        case Qt::Key_Up:
            action = s21::Up;
            break;
        case Qt::Key_Down:
            action = s21::Down;
            break;
        case Qt::Key_Return:
            action = s21::Action;
            break;
        default:
            action = s21::None;
            break;
    }
    bool hold = false;
    userInput(action, hold);
}

void s21::GameController::userInput(s21::UserAction_t action, bool hold)
{
    s21::GameField* m_gameField = getGamefield();
    if (action == s21::UserAction_t::Terminate)
    {
        if (m_gameField->isGameOver())
        {
            QApplication::quit();
        }
        else {
            m_gameField->GameOver();
            return;
        }
    }
    if (action == s21::Pause)
    {
        if (m_gameField->isGameOver())
        {
            m_gameField->StartNewGame();
            m_gameField->ReversePause();
            m_timer->start(m_gameField->getSpeed());
            return;
        }
        m_gameField->ReversePause();
        std::string text;
        if (m_gameField->isPause()) {
            m_timer->stop();
            text = "record : " + std::to_string(m_gameField->getRecord()) + "\n\n\nscore : " + std::to_string(m_gameField->getScore()) + "\n\n\nlevel : " + std::to_string(m_gameField->getLevel()) + "\n\n\ncontinue - SPACE";
        }
        else {
            m_timer->start(m_gameField->getSpeed());
            text = "record : " + std::to_string(m_gameField->getRecord()) + "\n\n\nscore : " + std::to_string(m_gameField->getScore()) + "\n\n\nlevel : " + std::to_string(m_gameField->getLevel()) + "\n\n\npause - SPACE";
        }
        if (m_gameField->m_updateTextFunction) {m_gameField->m_updateTextFunction(text); }
    }
    if (m_gameField->isMoveBlocked())
    {
        return;
    }
    if (action == s21::UserAction_t::Left && m_gameField->getSnakeDirection() != s21::Snake::SnakeDirection::right && m_gameField->getSnakeDirection() != s21::Snake::SnakeDirection::left)
    {
        m_gameField->setSnakeDirection(s21::Snake::SnakeDirection::left);
    }
    if (action == s21::UserAction_t::Right && m_gameField->getSnakeDirection() != s21::Snake::SnakeDirection::left && m_gameField->getSnakeDirection() != s21::Snake::SnakeDirection::right)
    {
        m_gameField->setSnakeDirection(s21::Snake::SnakeDirection::right);
    }
    if (action == s21::UserAction_t::Down && m_gameField->getSnakeDirection() != s21::Snake::SnakeDirection::up && m_gameField->getSnakeDirection() != s21::Snake::SnakeDirection::down)
    {
        m_gameField->setSnakeDirection(s21::Snake::SnakeDirection::down);
    }
    if (action == s21::UserAction_t::Up && m_gameField->getSnakeDirection() != s21::Snake::SnakeDirection::down && m_gameField->getSnakeDirection() != s21::Snake::SnakeDirection::up)
    {
        m_gameField->setSnakeDirection(s21::Snake::SnakeDirection::up);
    }
    m_gameField->setIsMoveBlocked_TRUE();
    (void)hold;
}
