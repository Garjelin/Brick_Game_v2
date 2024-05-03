#include "mainwindow.h"

s21::GameField* s21::getGamefield() {
        static s21::GameField gameField = s21::GameField(300, 600, 30);
        return &gameField;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setFixedSize(620, 620);
    //m_gameField = new GameField(300, 600, 30);
    s21::GameField* m_gameField = s21::getGamefield();
    m_helpField = new s21::HelpField();
    m_view = new s21::View(this);
    m_gameController = new s21::GameController(this, m_helpField, m_view);

    m_gameField->setUpdateViewFunction([this](){ m_view->updateView(); });
    m_gameField->setUpdateTextFunction([this](std::string text) { m_helpField->ChangeTextSlot(text); });
    m_gameField->setGameOverFunction([this]() { m_gameController->stopTimer(); });
    m_gameField->setSpeedUpFunction([this]() { m_gameController->updateTimer(); });

    m_centralWidget = new QWidget();
    m_gridLayout = new QGridLayout();

    m_view->setFixedSize(300, 600);
    m_helpField->setFixedSize(300, 600);

    m_gridLayout->addWidget(m_view, 0, 0);
    m_gridLayout->addWidget(m_helpField, 0, 1);
    m_centralWidget->setLayout(m_gridLayout);
    setCentralWidget(m_centralWidget);

    // Установка фона для главного окна
    QPalette pal = palette();
    pal.setColor(QPalette::Background, QColor(25, 50, 1));
    setAutoFillBackground(true);
    setPalette(pal);
}

MainWindow::~MainWindow()
{
    delete m_gameController;
    delete m_helpField;
    delete m_view;
    //delete m_gameField;
}

