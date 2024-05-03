#ifndef GAME_H
#define GAME_H

#include <random>
#include <functional>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

namespace s21 {
typedef enum {
    Start,
    Pause,
    Terminate,
    Left,
    Right,
    Up,
    Down,
    Action,
    None
} UserAction_t;

typedef struct {
    int **field;
    int **next;
    int score;
    int high_score;
    int level;
    int speed;
    int pause;
} GameInfo_t;

class SnakeItem;
class Snake;
class GameField;

class SnakeItem
{
    friend Snake;
    friend GameField;
public:
    SnakeItem(int x, int y);
    int getX() {return m_x;}
    int getY() {return m_y;}
private:
    int m_x;
    int m_y;
};

class Snake
{
    friend GameField;
private:
    std::vector<SnakeItem*> m_snakeBody;
    int m_snakeBeginSize;

public:
    Snake();
    ~Snake(){
        for (auto& item : m_snakeBody) {
            delete item;
        }
        m_snakeBody.clear();
    }
    enum SnakeDirection
    {
        up,
        left,
        down,
        right
    };
    SnakeDirection m_snakeDirection;
    void setDirection(SnakeDirection direction) {m_snakeDirection = direction;}
    SnakeDirection getDirection() {return m_snakeDirection;}
    std::vector<SnakeItem*>& getSnakeBody(){return m_snakeBody;}
};

class GameField
{
public:
    GameField(int width, int height, int itemSize);
    ~GameField() {
        deleteField();
        delete m_snake;
        delete m_food;
        m_food = nullptr;
    }

    void setUpdateViewFunction(std::function<void()> func) { m_updateViewFunction = std::move(func); }
    void setUpdateTextFunction(std::function<void(std::string)> func) { m_updateTextFunction = std::move(func); }
    void setGameOverFunction(std::function<void()> func) { m_gameOverFunction = std::move(func); }
    void setSpeedUpFunction(std::function<void()> func) { m_speedUpFunction = std::move(func); }

    bool isGameOver() { return m_isGameOver; }
    void StartNewGame();
    bool isPause() { return m_isPause; }
    void ReversePause() { m_isPause = !m_isPause; }
    void SetGameStatus();
    bool isMoveBlocked() { return m_isMoveBlocked; }
    void setIsMoveBlocked_TRUE() { m_isMoveBlocked = true; }
    void setIsMoveBlocked_FALSE() { m_isMoveBlocked = false; }
    int GetScore() { return m_score; }
    int getSnakeItemSize() { return m_snakeItemSize; }
    void GameOver();
    void setScore(int score) {m_score = score;}
    void setRecord(int record) {m_record = record;}
    void CreateFood();
    SnakeItem* MoveSnake();
    void CheckCollisions(SnakeItem* newSnakeItem);

    Snake* getSnake() const { return m_snake; }

    void setSnakeDirection(Snake::SnakeDirection direction);
    Snake::SnakeDirection getSnakeDirection() {return m_snake->getDirection();}
    SnakeItem* getFood() {return m_food;}
    void MoveAndCheck();
    int getSpeed() { return m_speed; }
    int getScore() { return m_score; }
    std::function<void()> m_updateViewFunction;
    std::function<void(std::string)> m_updateTextFunction;
    std::function<void()> m_gameOverFunction;
    std::function<void()> m_speedUpFunction;
    void (*m_timerCallback)();
    int getWidth() { return m_width; }
    int getHeight() { return m_height; }
    void setGameOver(bool cond) {m_isGameOver = cond;}
    int getLevel() { return m_level; }
    int getRecord() { return m_record; }
    void levelUp();

    int** updateField();
    void printField() const;
    GameInfo_t updateCurrentState();

private:
    int m_width;
    int m_height;
    Snake *m_snake;
    SnakeItem *m_food;
    int m_snakeItemSize;
    int m_fieldSize_hor;
    int m_fieldSize_ver;
    int m_score;
    int m_speed;
    bool m_isPause;
    bool m_isMoveBlocked;
    bool m_isGameOver;
    bool m_collision;
    int m_level;
    int m_record;

    int** m_field;



    
    

    void createField();
    void deleteField();
    void clearField();
    void setCell(int x, int y, int value);
    GameInfo_t gameInfo;

};
};

#endif // GAME_H

