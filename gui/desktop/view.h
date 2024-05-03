#ifndef VIEW_H
#define VIEW_H

#include <QWidget>
#include <QPainter>
#include <QKeyEvent>
#include "../../brick_game/snake/game.h"

namespace s21 {
s21::GameField* getGamefield();

class View : public QWidget
{
    Q_OBJECT
public:
    View(QWidget *parent);
    void updateView(){repaint();}


protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

signals:
    void keyPressed(QKeyEvent *event);
};
};

#endif // VIEW_H


