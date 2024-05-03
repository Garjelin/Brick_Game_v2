#include "game.h"
//#include <QDebug>
#include <iostream>



s21::GameField::GameField(int width, int height, int itemSize) : m_width(width), m_height(height), m_snakeItemSize(itemSize)
{
    createField(); 
    m_fieldSize_hor = m_width/m_snakeItemSize;
    m_fieldSize_ver = m_height/m_snakeItemSize;
    StartNewGame();
}

void s21::GameField::setSnakeDirection(Snake::SnakeDirection direction)
{
    if (m_snake != nullptr)
    {
        m_snake->setDirection(direction);
    }
}

void s21::GameField::levelUp()
{
    if (m_score % 5 == 0) {
        m_level++;
        m_speed /= 1.1;
        if (m_speedUpFunction) {m_speedUpFunction(); }
    }
}

void s21::GameField::GameOver()
{
    m_isGameOver = true;

    if (m_score > m_record)
    {
        // Если превышает, записываем новый рекорд в файл record.txt
        std::ofstream recordFile("record.txt");
        if (recordFile.is_open())
        {
            recordFile << m_score << "\n";
            recordFile.close();
        }
    }
    updateField();
    std::string text = "record : " + std::to_string(m_record) + "\n\n\nscore : " + std::to_string(m_score) + "\n\n\nlevel : " + std::to_string(m_level) + "\n\n\nstart again - SPACE";
    if (m_updateTextFunction) {m_updateTextFunction(text); }
    if (m_gameOverFunction) {m_gameOverFunction(); }
    if (m_updateViewFunction) {m_updateViewFunction(); }
}

void s21::GameField::StartNewGame()
{
    m_speed = 300;
    m_level = 1;
    m_record = 0;
    m_isPause = true;
    m_isMoveBlocked = false;
    m_isGameOver = false;
    m_collision = false;
    m_snake = new Snake();
    m_food = new SnakeItem(m_fieldSize_hor/2, m_fieldSize_ver/2);
    m_score = 0;

    std::ifstream recordFile("record.txt");
    if (recordFile.is_open())
    {
        // Если файл открыт, считываем значение из него в m_record
        std::string recordString;
        std::getline(recordFile, recordString);
        std::istringstream iss(recordString);
        iss >> m_record;
        recordFile.close();
    }
    else
    {
        // Если файл не открыт, создаем его и устанавливаем m_record в 0
        std::ofstream newRecordFile("record.txt");
        if (newRecordFile.is_open())
        {
            newRecordFile << "0\n"; // Записываем начальное значение рекорда
            newRecordFile.close();
        }
    }

    std::string text = "record : " + std::to_string(m_record) + "\n\n\nscore : " + std::to_string(m_score) + "\n\n\nlevel : " + std::to_string(m_level) + "\n\n\npause - SPACE";
    if (m_updateTextFunction) {m_updateTextFunction(text); }
}

void s21::GameField::CreateFood()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> disX(0, m_fieldSize_hor - 1);
    std::uniform_int_distribution<> disY(0, m_fieldSize_ver - 1);
    m_food->m_x = disX(gen);
    m_food->m_y = disY(gen);

    for (std::vector<SnakeItem*>::size_type i = 0; i < m_snake->m_snakeBody.size(); i++)
    {
        if (m_food->m_x == m_snake->m_snakeBody[i]->m_x && m_food->m_y == m_snake->m_snakeBody[i]->m_y)
        {
            return CreateFood();
        }
    }
}

void s21::GameField::MoveAndCheck()
{
    SnakeItem *newSnakeItem = MoveSnake();
    updateField();
    //printField();
    if (m_updateViewFunction) {m_updateViewFunction(); }
    CheckCollisions(newSnakeItem);
}

int** s21::GameField::updateField() {
    clearField();
    for (const auto& segment : m_snake->getSnakeBody()) {
        setCell(segment->getX() * getSnakeItemSize(), segment->getY() * getSnakeItemSize(), 1); 
    }
    setCell(m_food->getX() * getSnakeItemSize(), m_food->getY() * getSnakeItemSize(), 2); 
    return m_field;
}
// void s21::GameField::printField() const {
//     for (int y = 0; y < m_height; ++y) {
//         for (int x = 0; x < m_width; ++x) {
//             std::cout << m_field[x][y] << " ";
//         }
//         std::cout << std::endl;
//     }
//     std::cout << std::endl;
// }
    
s21::GameInfo_t s21::GameField::updateCurrentState() {
    gameInfo.field = updateField();
    gameInfo.next = nullptr;
    gameInfo.score = m_score;
    gameInfo.high_score = m_record;
    gameInfo.level = m_level;
    gameInfo.speed = m_speed;
    gameInfo.pause = m_isPause;

    return gameInfo;
}

void s21::GameField::createField() {
    // Выделяем память для поля
    m_field = new int*[m_width];
    for (int i = 0; i < m_width; ++i) {
        m_field[i] = new int[m_height];
    }
    // Инициализируем поле (в данном примере заполняем нулями)
    for (int i = 0; i < m_width; ++i) {
        for (int j = 0; j < m_height; ++j) {
            m_field[i][j] = 0;
        }
    }
}

void s21::GameField::deleteField() {
    // Освобождаем память, выделенную для поля
    for (int i = 0; i < m_width; ++i) {
        delete[] m_field[i];
    }
    delete[] m_field;
}

void s21::GameField::clearField() {
    // Очищаем поле (заполняем нулями)ы
    for (int i = 0; i < m_width; ++i) {
        for (int j = 0; j < m_height; ++j) {
            m_field[i][j] = 0;
        }
    }
}

void s21::GameField::setCell(int x, int y, int value) {
    if (x >= 0 && x < m_width && y >= 0 && y < m_height) {
        m_field[x][y] = value;
    }
}

s21::SnakeItem* s21::GameField::MoveSnake()
{

    SnakeItem *newSnakeItem;
    if (m_snake->m_snakeDirection == Snake::SnakeDirection::right)
    {
        newSnakeItem = new SnakeItem(m_snake->m_snakeBody[0]->m_x + 1, m_snake->m_snakeBody[0]->m_y);
    }
    else if (m_snake->m_snakeDirection == Snake::SnakeDirection::left)
    {
        newSnakeItem = new SnakeItem(m_snake->m_snakeBody[0]->m_x - 1, m_snake->m_snakeBody[0]->m_y);
    }
    else if (m_snake->m_snakeDirection == Snake::SnakeDirection::up)
    {
        newSnakeItem = new SnakeItem(m_snake->m_snakeBody[0]->m_x, m_snake->m_snakeBody[0]->m_y-1);
    }
    else
    {
        newSnakeItem = new SnakeItem(m_snake->m_snakeBody[0]->m_x, m_snake->m_snakeBody[0]->m_y+1);
    }

    // получение еды
    if(newSnakeItem->m_x == m_food->m_x && newSnakeItem->m_y == m_food->m_y)
    {
        m_score++;
        levelUp();
        CreateFood();
        std::string text = "record : " + std::to_string(m_record) + "\n\n\nscore : " + std::to_string(m_score) + "\n\n\nlevel : " + std::to_string(m_level) + "\n\n\npause - SPACE";
        if (m_updateTextFunction) {m_updateTextFunction(text); }
    }
    else
    {
        delete m_snake->m_snakeBody.back();
        m_snake->m_snakeBody.pop_back();
    }
    m_snake->m_snakeBody.insert(m_snake->m_snakeBody.begin(), newSnakeItem);
    return newSnakeItem;
}

void s21::GameField::CheckCollisions(SnakeItem* newSnakeItem) {
//    qDebug() << "newSnakeItem " << newSnakeItem;
// ограничение игрового поля

    if (newSnakeItem->m_x >= m_fieldSize_hor)
    {
        m_collision = true;
    }
    else if (newSnakeItem->m_x < 0)
    {
        m_collision = true;
    }
    else if (newSnakeItem->m_y < 0)
    {
        m_collision = true;
    }
    else if (newSnakeItem->m_y >= m_fieldSize_ver)
    {
        m_collision = true;
    }
//    // проигрыш

    for (std::vector<SnakeItem*>::size_type i = 1; i < m_snake->m_snakeBody.size(); i++)
    {
        if (m_snake->m_snakeBody[0]->m_x == m_snake->m_snakeBody[i]->m_x && m_snake->m_snakeBody[0]->m_y == m_snake->m_snakeBody[i]->m_y)
        {
            m_collision = true;
            break;
        }
    }
    if (m_collision)
    {
        GameOver();
        return;
    }
}

s21::SnakeItem::SnakeItem(int x, int y)
{
    m_x = x;
    m_y = y;
}

s21::Snake::Snake()
{
    m_snakeBeginSize = 4;
    for (int i = 0; i < m_snakeBeginSize; i++)
    {
        m_snakeBody.insert(m_snakeBody.begin(), new SnakeItem(i, 0));
    }
    m_snakeDirection = SnakeDirection::right;
}

