#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../brick_game/snake/game.h"
#include "../gui/desktop/helpfield.h"
#include "../gui/desktop/gamecontroller.h"
#include "../gui/desktop/view.h"
#include <QMainWindow>
#include <QGridLayout>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    s21::GameField *m_gameField;
    s21::HelpField *m_helpField;
    s21::GameController *m_gameController;
    s21::View *m_view;
    QWidget *m_centralWidget;
    QGridLayout *m_gridLayout;
};
#endif // MAINWINDOW_H
