
#include <gtest/gtest.h>
#include "../brick_game/snake/game.h"

TEST(GameFieldConstructorTest, VariableInitialization) {
    s21::GameField gameField(100, 80, 10);

    EXPECT_EQ(gameField.getWidth(), 100);
    EXPECT_EQ(gameField.getHeight(), 80);
    EXPECT_EQ(gameField.getSnakeItemSize(), 10);

    EXPECT_TRUE(gameField.isPause());
    EXPECT_FALSE(gameField.isGameOver());
    EXPECT_EQ(gameField.getScore(), 0);
    EXPECT_EQ(gameField.getLevel(), 1);
    EXPECT_EQ(gameField.getRecord(), 0);
}

TEST(GameFieldConstructorTest, FieldAndObjectsCreation) {
    s21::GameField gameField(100, 80, 10);

    EXPECT_NE(gameField.updateField(), nullptr);

    s21::Snake* snake = gameField.getSnake();
    EXPECT_NE(snake, nullptr);

    s21::SnakeItem* food = gameField.getFood();
    EXPECT_NE(food, nullptr);
}

TEST(GameFieldConstructorTest, SnakeDirectionInitialization) {
    s21::GameField gameField(100, 80, 10);

    EXPECT_EQ(gameField.getSnakeDirection(), s21::Snake::SnakeDirection::right);
}

TEST(GameFieldConstructorTest, FieldBounds) {
    s21::GameField gameField(100, 80, 10);

    for (auto& segment : gameField.getSnake()->getSnakeBody()) {
        EXPECT_GE(segment->getX(), 0);
        EXPECT_LT(segment->getX(), gameField.getWidth() / gameField.getSnakeItemSize());
        EXPECT_GE(segment->getY(), 0);
        EXPECT_LT(segment->getY(), gameField.getHeight() / gameField.getSnakeItemSize());
    }

    EXPECT_GE(gameField.getFood()->getX(), 0);
    EXPECT_LT(gameField.getFood()->getX(), gameField.getWidth() / gameField.getSnakeItemSize());
    EXPECT_GE(gameField.getFood()->getY(), 0);
    EXPECT_LT(gameField.getFood()->getY(), gameField.getHeight() / gameField.getSnakeItemSize());
}

TEST(GameFieldTest, SetSnakeDirection) {
    s21::GameField gameField(100, 80, 10);

    gameField.setSnakeDirection(s21::Snake::SnakeDirection::up);
    EXPECT_EQ(gameField.getSnake()->getDirection(), s21::Snake::SnakeDirection::up);

    gameField.setSnakeDirection(s21::Snake::SnakeDirection::down);
    EXPECT_EQ(gameField.getSnake()->getDirection(), s21::Snake::SnakeDirection::down);

    gameField.setSnakeDirection(s21::Snake::SnakeDirection::left);
    EXPECT_EQ(gameField.getSnake()->getDirection(), s21::Snake::SnakeDirection::left);

    gameField.setSnakeDirection(s21::Snake::SnakeDirection::right);
    EXPECT_EQ(gameField.getSnake()->getDirection(), s21::Snake::SnakeDirection::right);
}

TEST(GameFieldTest, LevelUp) {
    s21::GameField gameField(100, 80, 10);

    int start_level = gameField.getLevel();
    int start_speed = gameField.getSpeed();
    EXPECT_EQ(start_level, 1);

    gameField.setScore(5);
    gameField.levelUp();
    EXPECT_EQ(gameField.getLevel(), 2);
    int speed = start_speed/1.1;
    EXPECT_EQ(gameField.getSpeed(), speed);

    gameField.setScore(9);
    gameField.levelUp();
    EXPECT_EQ(gameField.getLevel(), 2);
    EXPECT_EQ(gameField.getSpeed(), speed);

    gameField.setScore(10);
    gameField.levelUp();
    EXPECT_EQ(gameField.getLevel(), 3);
    speed = start_speed/1.1/1.1;
    EXPECT_EQ(gameField.getSpeed(), speed);
}


TEST(GameFieldTest, GameOver) {
    s21::GameField gameField(100, 80, 10);

    gameField.setScore(50);
    gameField.setRecord(30);

    gameField.GameOver();

    EXPECT_TRUE(gameField.isGameOver());

    std::ifstream recordFile("record.txt");
    int recordValue = 0;
    if (recordFile.is_open()) {
        recordFile >> recordValue;
        recordFile.close();
    }
    EXPECT_EQ(recordValue, 50);

}

TEST(GameFieldTest, StartNewGame) {
    s21::GameField gameField(100, 80, 10);

    gameField.setScore(60);
    gameField.GameOver();

    gameField.StartNewGame();

    EXPECT_EQ(gameField.getSpeed(), 300);
    EXPECT_EQ(gameField.getLevel(), 1);
    EXPECT_EQ(gameField.getRecord(), 60);
    EXPECT_TRUE(gameField.isPause());
    EXPECT_FALSE(gameField.isMoveBlocked());
    EXPECT_FALSE(gameField.isGameOver());
    EXPECT_EQ(gameField.getScore(), 0);

    EXPECT_NE(gameField.getSnake(), nullptr);
    EXPECT_NE(gameField.getFood(), nullptr);
}

TEST(GameFieldTest, CreateFood) {
    s21::GameField gameField(100, 80, 10);

    gameField.getSnake()->getSnakeBody().push_back(new s21::SnakeItem(5, 5));
    gameField.getSnake()->getSnakeBody().push_back(new s21::SnakeItem(5, 6));

    gameField.CreateFood();

    int foodX = gameField.getFood()->getX();
    int foodY = gameField.getFood()->getY();

    // Проверяем, что еда находится в пределах игрового поля
    EXPECT_GE(foodX, 0);
    EXPECT_LT(foodX, gameField.getWidth());
    EXPECT_GE(foodY, 0);
    EXPECT_LT(foodY, gameField.getWidth());

    // Проверяем, что еда не находится на теле змеи
    for (auto item : gameField.getSnake()->getSnakeBody()) {
        EXPECT_FALSE(foodX == item->getX() && foodY == item->getY());
    }
}

TEST(GameFieldTest, MoveAndCheck) {
    s21::GameField gameField(100, 80, 10);
    gameField.MoveAndCheck();
    EXPECT_NE(gameField.updateField(), nullptr);
}

TEST(GameFieldTest, MoveSnake) {
    s21::GameField gameField(100, 80, 10);
    
    int initialSnakeLength = gameField.getSnake()->getSnakeBody().size();
    EXPECT_EQ(initialSnakeLength, 4);

    gameField.MoveSnake();
    gameField.MoveSnake();
    gameField.MoveSnake();
    EXPECT_EQ(gameField.getSnake()->getSnakeBody().front()->getX(), 6);
    EXPECT_EQ(gameField.getSnake()->getSnakeBody().front()->getY(), 0);

    gameField.setSnakeDirection(s21::Snake::SnakeDirection::down);
    gameField.MoveSnake();
    EXPECT_EQ(gameField.getSnake()->getSnakeBody().front()->getX(), 6);
    EXPECT_EQ(gameField.getSnake()->getSnakeBody().front()->getY(), 1);

    gameField.setSnakeDirection(s21::Snake::SnakeDirection::left);
    gameField.MoveSnake();
    EXPECT_EQ(gameField.getSnake()->getSnakeBody().front()->getX(), 5);
    EXPECT_EQ(gameField.getSnake()->getSnakeBody().front()->getY(), 1);

    gameField.setSnakeDirection(s21::Snake::SnakeDirection::up);
    gameField.MoveSnake();
    EXPECT_EQ(gameField.getSnake()->getSnakeBody().front()->getX(), 5);
    EXPECT_EQ(gameField.getSnake()->getSnakeBody().front()->getY(), 0);
}

TEST(GameFieldTest, CheckCollisions) {
    s21::GameField gameField(100, 80, 10);

    gameField.setSnakeDirection(s21::Snake::SnakeDirection::up);
    s21::SnakeItem* newSnakeItem = gameField.MoveSnake();
    gameField.CheckCollisions(newSnakeItem);
    EXPECT_TRUE(gameField.isGameOver());

    s21::GameField gameField2(100, 80, 10);
    gameField2.setSnakeDirection(s21::Snake::SnakeDirection::left);
    s21::SnakeItem* newSnakeItem2 = gameField2.MoveSnake();
    gameField2.CheckCollisions(newSnakeItem2);
    EXPECT_TRUE(gameField2.isGameOver());
}


TEST(GameFieldTest, UpdateCurrentState) {
    s21::GameField gameField(10, 10, 1);
    s21::GameInfo_t gameInfo = gameField.updateCurrentState();

    EXPECT_NE(gameInfo.field, nullptr);

    EXPECT_EQ(gameInfo.score, 0);
    EXPECT_EQ(gameInfo.high_score, 60);
    EXPECT_EQ(gameInfo.level, 1);
    EXPECT_EQ(gameInfo.speed, 300); 
    EXPECT_TRUE(gameInfo.pause);
}