#include "view.h"
#include <QDebug>

s21::View::View(QWidget *parent) : QWidget(parent) {setFocusPolicy(Qt::StrongFocus);}

void s21::View::paintEvent(QPaintEvent *e)
{
    s21::GameField* m_gameField = getGamefield();
    s21::GameInfo_t info = m_gameField->updateCurrentState();
    Q_UNUSED(e)
    QBrush gameFieldBrush(QColor(180, 240, 90), Qt::SolidPattern);
    QBrush snakeBrush(QColor(60, 125, 250), Qt::SolidPattern);
    QBrush foodBrush(QColor(240, 100, 100), Qt::SolidPattern);
    QPainter painter;
    painter.begin(this);
    if (m_gameField->isGameOver())
    {
        QBrush gameFieldBrush(QColor(0, 40, 0), Qt::SolidPattern);
        painter.setBrush(gameFieldBrush);
        painter.setPen(QColor(50, 50, 50));
        painter.drawRect(0, 0, width()-1, height()-1);
        painter.setPen(QColor(225, 240, 225));
        painter.setFont(QFont("Arial", 25, 75));
        painter.drawText(QRect(0, 0, width(), height()), Qt::AlignCenter, "GAME OVER");
        return;
    }
    // отрисовка игрового поля
    painter.setBrush(gameFieldBrush);
    painter.setPen(QColor(50, 50, 50));
    painter.drawRect(0, 0, width()-1, height()-1);

    for (int y = 0; y < m_gameField->getHeight(); ++y) {
        for (int x = 0; x < m_gameField->getWidth(); ++x) {
            if (info.field[x][y] == 1) {
                // Отрисовка сегмента змеи
                painter.setBrush(snakeBrush);
                painter.setPen(QColor(0, 0, 0));
                painter.drawEllipse(x, y, m_gameField->getSnakeItemSize(), m_gameField->getSnakeItemSize());
            } else if (info.field[x][y] == 2) {
                // Отрисовка еды
                painter.setBrush(foodBrush);
                painter.drawEllipse(x, y, m_gameField->getSnakeItemSize(), m_gameField->getSnakeItemSize());
            }
        }
    }

    painter.end();
    m_gameField->setIsMoveBlocked_FALSE();
}

void s21::View::keyPressEvent(QKeyEvent *event) // Add this method
{
    // Forward the keyPressEvent to the GameController
    keyPressed(event);
}
