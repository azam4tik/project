#include <locale.h>
#include <math.h>
#include <ncurses.h>
#include <stdio.h>

const int SIZE_WIDTH = 80;
const int SIZE_HEIGHT = 25;
const int GOAL = 21;
const double TICK = 1;

void drawPixel(int x, int y);
void update();
void draw();

int PaddleAX = 0;
int PaddleAY = 12;
int PaddleBX = 79;
int PaddleBY = 12;
int BallX = 40;
int BallY = 11;
int SpeedX = -1;
int SpeedY = 0;
int ScoreA = 0;
int ScoreB = 0;
int Key = '-';

void Reset() {
    PaddleAX = 0;
    PaddleAY = 12;
    PaddleBX = 79;
    PaddleBY = 12;
    BallX = 40;
    BallY = 11;
    SpeedX = -1;
    SpeedY = 0;
}
void clearScreen() {
    for (int y = 0; y < SIZE_HEIGHT; y++) {
        for (int x = 0; x <= SIZE_WIDTH; x++) {
            move(y, x);
            printw(" ");
        }
    }
}
int main() {
    setlocale(LC_ALL, "");
    initscr();
    noecho();
    curs_set(0);
    while (ScoreA != GOAL && ScoreB != GOAL) {
        halfdelay(TICK);
        update();
        draw();
        nodelay(stdscr, TRUE);
        Key = getch();
    }
    endwin();
    printf("Player %s won!\n", ScoreA == 21 ? "A" : "B");
    return 0;
}
void update() {
    BallX += SpeedX;
    BallY += SpeedY;
    if (BallX < 0) {
        ScoreA++;
        Reset();
        SpeedX = -1;
    }
    if (BallX > SIZE_WIDTH) {
        ScoreB++;
        Reset();
        SpeedX = 1;
    }
    if (BallY <= 0 || BallY + 1 >= SIZE_HEIGHT) SpeedY *= -1;
    if ((BallX - 1 == PaddleAX && BallY == PaddleAY) || (BallX + 1 == PaddleBX && BallY == PaddleBY))
        SpeedX *= -1;
    if ((BallX - 1 == PaddleAX && BallY == PaddleAY - 1) ||
        (BallX + 1 == PaddleBX && BallY == PaddleBY - 1)) {
        SpeedX *= -1;
        SpeedY = -1;
    }
    if ((BallX - 1 == PaddleAX && BallY == PaddleAY + 1) ||
        (BallX + 1 == PaddleBX && BallY == PaddleBY + 1)) {
        SpeedX *= -1;
        SpeedY = 1;
    }
    if ((Key == 'a' || Key == 'A') && PaddleAY > 1) PaddleAY--;
    if ((Key == 'z' || Key == 'Z') && PaddleAY < SIZE_HEIGHT - 2) PaddleAY++;
    if ((Key == 'k' || Key == 'K') && PaddleBY > 1) PaddleBY--;
    if ((Key == 'm' || Key == 'M') && PaddleBY < SIZE_HEIGHT - 2) PaddleBY++;
}
void draw() {
    clearScreen();
    refresh();
    for (int y = 0; y < SIZE_HEIGHT; y++) {
        for (int x = 0; x < SIZE_WIDTH; x++) {
            move(y, x);
            if (x == PaddleAX && (y == PaddleAY || y == PaddleAY - 1 || y == PaddleAY + 1))
                printw("█");
            else if (x == PaddleBX && (y == PaddleBY || y == PaddleBY - 1 || y == PaddleBY + 1))
                printw("█");
            else if (x == BallX && y == BallY)
                printw("●");
            else if (x == 0 && y == 0)
                printw("┌");
            else if (x == 0 && y == 24)
                printw("└");
            else if (x == 79 && y == 0)
                printw("┐");
            else if (x == 79 && y == 24)
                printw("┘");
            else if (y == 0 || y == SIZE_HEIGHT - 1)
                printw("─");
            else if (x == 0 || x == SIZE_WIDTH - 1)
                printw("│");
            else if (x == 39)
                printw("│");
            else
                printw(" ");
        }
    }
    move(2, 35);
    printw("%d", ScoreA);
    move(2, 43);
    printw("%d", ScoreB);
    move(0, 90);
}
