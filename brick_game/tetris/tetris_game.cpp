#include "tetris_game.h"

GameField::GameField(int width, int height): m_width(width), m_height(height) {
    m_isGameOver = false;
    field = new TField(m_width, m_height, c_field);
    figure = new TFigure(field);
    screen = new TScreen('\0');

    shpArrCnt = 7;
    shpFrame = 16;
    shpArr = new char*[shpArrCnt];
    for (int i = 0; i < shpArrCnt; i++) {
        shpArr[i] = new char[shpFrame+1];
    }
    strcpy(shpArr[0], ".....oo..oo.....");
    strcpy(shpArr[1], "....oooo........");
    strcpy(shpArr[2], "....ooo..o......");
    strcpy(shpArr[3], "....ooo.o.......");
    strcpy(shpArr[4], "....ooo...o.....");
    strcpy(shpArr[5], ".....oo.oo......");
    strcpy(shpArr[6], "....oo...oo.....");

    figure->Shape(shpArr[rand() % shpArrCnt]);
    figure->Pos(m_width / 2 - shp_width / 2, 0);
}

void TFigure::Put(ScreenMap &scr) {
    for (int i = 0; i < coordCnt; i++) {
        scr(coord[i].y, coord[i].x) = c_fig;
    }
}

void TFigure::PutField(FieldMap &fld) {
    for (int i = 0; i < coordCnt; i++) {
        fld(coord[i].y, coord[i].x) = c_figDown;
    }
}

void TFigure::TurnSet(char _turn) {
    int oldTurn = turn;
    turn = (_turn > 3 ? 0 : (_turn < 0 ? 3 : _turn));
    int chk = Check();
    if (chk == 0) return;
    if (chk == 1) {
        int xx = x;
        int k = (x > (scr_width / 2) ? -1 : +1);
        for (int i = 1; i < 3; i++) {
            x += k;
            if (Check() == 0) return;
        }
        x = xx;
    }
    turn = oldTurn;
    CalcCoord();
}

bool TFigure::MoveFig(int dx, int dy) {
    int oldX = x;
    int oldY = y;
    int newX = x + dx;
    int newY = y + dy;
    Pos(newX, newY);
    int chk = Check();
    if (chk >= 1) {
        Pos(oldX, oldY);
        if (chk == 2) return false;
    }
    erase();
    return true;
}

int TFigure::Check(){
    CalcCoord();
    for (int i = 0; i < coordCnt; i++) {
        if (coord[i].x < 0 || coord[i].x >= scr_width) return 1;
    }
    for (int i = 0; i < coordCnt; i++) {
        if (coord[i].y >= scr_height || field->fld(coord[i].y,coord[i].x) == c_figDown) return 2;
    }
    return 0;
}

void TFigure::CalcCoord() {
    int xx, yy;
    coordCnt = 0;
    for (int i = 0; i < shp_width; i++) {
        for (int j = 0; j < shp_height; j++) {
            if (vid[j][i] == 'o') {
                if (turn == 0) {
                    xx = x+i;
                    yy = y+j;
                }
                if (turn == 1) {
                    xx = x+(shp_height-j-1);
                    yy = y+i;
                }
                if (turn == 2) {
                    xx = x + (shp_width-i-1);
                    yy = y+(shp_height-j-1);
                }
                if (turn == 3) {
                    xx = x+j;
                    yy = y+(shp_height-i-1) + (shp_width - shp_height);
                }
                coord[coordCnt].x = xx;
                coord[coordCnt].y = yy;
                coordCnt++;
            }
        }
    }
}

void TField::Put(ScreenMap &scr) {
    for (int i = 0; i < scr_width; i++) {
        for (int j = 0; j < scr_height; j++) {
            scr(j, i)= fld(j,i);
        }
    }
}

void TField::Burning() {
    for (int j = scr_height - 1; j >= 0; j--) {
        static bool fillLine;
        fillLine = true;
        for (int i = 0; i < scr_width; i++) {
            if (fld(j,i) != c_figDown) fillLine = false;
        }
        if (fillLine) {
            for (int y = j; y >= 1; y--) {
                for (int x = 0; x < scr_width; x++) {
                    fld(y, x) = fld(y - 1, x);
                }
            }
            for (int x = 0; x < scr_width; x++) {
                fld(0, x) = c_field;
            }
            return;
        }
    }
}