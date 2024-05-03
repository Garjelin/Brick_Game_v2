#ifndef HELPFIELD_H
#define HELPFIELD_H

#include <QWidget>
#include <QPainter>
#include <sstream>
#include "../../brick_game/snake/game.h"

namespace s21 {
class HelpField : public QWidget
{
public:
    HelpField();
    void ChangeTextSlot(std::string text);

protected:
    void paintEvent(QPaintEvent *e) override;

private:
    std::string m_text;
};
};

#endif // HELPFIELD_H
