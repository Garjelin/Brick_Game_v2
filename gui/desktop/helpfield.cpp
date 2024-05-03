#include "helpfield.h"

s21::HelpField::HelpField()
{
    setFixedSize(300, 80);
    std::ifstream recordFile("record.txt");
    std::string recordString;
    if (recordFile.is_open())
    {
        std::getline(recordFile, recordString);
        recordFile.close();
    }
    m_text = "record : " + recordString + "\n\n\nscore : 0\n\n\nlevel : 0\n\n\nto start press - SPACE";
}

void s21::HelpField::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e)
    QBrush helpFieldBrush(QColor(0, 40, 0), Qt::SolidPattern);
    QPainter painter;
    painter.begin(this);
    // отрисовка helpField
    painter.setBrush(helpFieldBrush);
    painter.setPen(QColor(50, 50, 50));
    painter.drawRect(0, 0, width()-1, height()-1);
    painter.setPen(QColor(225, 240, 225));
    painter.setFont(QFont("Arial", 20, 75));
    QString q_text = QString::fromStdString(m_text);
    painter.drawText(QRect(0, 0, width(), height()), Qt::AlignCenter, q_text);
    painter.end();
}

void s21::HelpField::ChangeTextSlot(std::string text)
{
    m_text = text;
    repaint();
}
