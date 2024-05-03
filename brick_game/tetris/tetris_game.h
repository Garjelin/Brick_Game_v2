#ifndef T_GAME_H
#define T_GAME_H

#include <iostream>
#include <ncurses.h>
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <cstring>
#include <unistd.h> 
#include <chrono>
#include <thread>

using namespace std;

const int scr_width = 10;
const int scr_height = 20;

const char c_fig = 'o';
const char c_field = '.';
const char c_figDown = 'o';

typedef char TScreenMap[scr_height][scr_width];
typedef char TFieldMap[scr_height][scr_width];

const int shp_width = 4;
const int shp_height = 4;
typedef char TShape[shp_height][shp_height];


class GameField;

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

class ScreenMap {
public:
    char map[scr_height][scr_width];
    //ScreenMap(char ch) { memset(map, ch, sizeof(map)); }
    char& operator()(int i, int j) { return map[i][j]; }
};

class FieldMap {
public:
    char map[scr_height][scr_width];
    //FieldMap(char ch) { memset(map, ch, sizeof(map)); }
    char& operator()(int i, int j) { return map[i][j]; }
};

class TScreen {
    ScreenMap scr;
public:
    TScreen(char ch) : scr(ch) {}
    ~TScreen() {}
    ScreenMap& getScreenMap() { return scr; }
    void Show() {
        for (int i = 0; i < scr_height; ++i) {
            for (int j = 0; j < scr_width; ++j) {
                mvaddch(i, j, scr(i, j));
            }
        }
    }

};

class TField {
    int m_width;
    int m_height;
public:
    FieldMap fld;
    TField(int width, int height, char ch) : m_width(width), m_height(height), fld(ch) {}
    void FillScr() {
        for (int i = 0; i < m_height; ++i) {
            for (int j = 0; j < m_width; ++j) {
                fld(i, j) = c_field;
            }
        }
    }
    void Put(ScreenMap &scr);
    void Burning();
};

class TFigure {
    int x, y;
    TShape vid;
    char turn;
    int coordCnt;
    struct Coord {
        int x;
        int y;
    } coord[shp_width * shp_height];
    TField *field;
public:
    TFigure(TField *_field) {
        memset(this, 0, sizeof(*this));

        field = _field;
    }
    void Shape(const char _vid[]) {memcpy(vid, _vid, sizeof(vid));}
    void Pos(int _x, int _y) { 
        x = _x; y = _y; 
        CalcCoord();
        }
    char TurnGet() {return turn;}
    void TurnSet(char _turn);
    void Put(ScreenMap &scr);
    void PutField(FieldMap &fld);
    bool MoveFig(int dx, int dy);
    int Check();
private:
    void CalcCoord();
};

class GameField {
    TScreen *screen;
    TField *field;
    TFigure *figure;

    bool m_isGameOver;
    int m_width;
    int m_height;
public:
    GameField(int width, int height);
    ~GameField() {
        for (int i = 0; i < shpArrCnt; ++i) {
            delete[] shpArr[i];
        }
        delete[] shpArr;
        delete screen;
        delete field;
        delete figure;
    }
    char **shpArr;
    int shpArrCnt;
    int shpFrame;
    TFigure* getFigure() { return figure; };
    TField* getField() { return field; };
    TScreen* getScreen() { return screen; };
    void PlayerControl();
    int getWidth() { return m_width; }
    int getHeight() { return m_width; }
    bool isGameOver() { return m_isGameOver; }
    void setGameOver() { m_isGameOver = true; }
};

#endif // T_GAME_H

