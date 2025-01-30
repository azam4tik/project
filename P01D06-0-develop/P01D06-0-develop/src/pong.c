#include <math.h>
#include <stdio.h>

#define clear() printf("\033[H\033[J")

const int SIZE_WIDTH = 80;
const int SIZE_HEIGHT = 25;
const int GOAL = 21;

void drawPixel(int x, int y);
void update();
void draw();

int PaddleAX = 0;
int PaddleAY = 12;
int PaddleBX = 79;
int PaddleBY = 12;
int BallX = 39;
int BallY = 11;
int SpeedX = -1;
int SpeedY = 0;
int ScoreA = 0;
int ScoreB = 0;
int Key = '-';
int Turn = 0;

int KeyPressed() {
    return (!Turn && (Key == 'a' || Key == 'A' || Key == 'z' || Key == 'Z')) ||
           (Turn && (Key == 'k' || Key == 'K' || Key == 'm' || Key == 'M')) || Key == ' ';
}
void Reset() {
    PaddleAX = 0;
    PaddleAY = 12;
    PaddleBX = 79;
    PaddleBY = 12;
    BallX = 39;
    BallY = 11;

    SpeedX = -1;
    SpeedY = 0;
}
void SwapTurns() { Turn = !Turn; }
int main() {
    while (ScoreA != GOAL && ScoreB != GOAL) {
        if (KeyPressed()) update();
        draw();
        printf("%d : %d\n", ScoreA, ScoreB);
        Key = getchar();
        while (getchar() != '\n') {
        }
    }
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
    if ((BallX - 1 == PaddleAX && BallY == PaddleAY) || (BallX + 1 == PaddleBX && BallY == PaddleBY)) {
        SwapTurns();
        SpeedX *= -1;
    }
    if ((BallX - 1 == PaddleAX && BallY == PaddleAY - 1) ||
        (BallX + 1 == PaddleBX && BallY == PaddleBY - 1)) {
        SpeedX *= -1;
        SpeedY = -1;
        SwapTurns();
    }
    if ((BallX - 1 == PaddleAX && BallY == PaddleAY + 1) ||
        (BallX + 1 == PaddleBX && BallY == PaddleBY + 1)) {
        SpeedX *= -1;
        SpeedY = 1;
        SwapTurns();
    }
    if ((Key == 'a' || Key == 'A') && PaddleAY > 1 && !Turn) PaddleAY--;
    if ((Key == 'z' || Key == 'Z') && PaddleAY < SIZE_HEIGHT - 2 && !Turn) PaddleAY++;
    if ((Key == 'k' || Key == 'K') && PaddleBY > 1 && Turn) PaddleBY--;
    if ((Key == 'm' || Key == 'M') && PaddleBY < SIZE_HEIGHT - 2 && Turn) PaddleBY++;
}
void draw() {
    clear();
    for (int y = 0; y < SIZE_HEIGHT; y++) {
        for (int x = 0; x < SIZE_WIDTH; x++) {
            if (x == PaddleAX && (y == PaddleAY || y == PaddleAY - 1 || y == PaddleAY + 1))
                printf("█");
            else if (x == PaddleBX && (y == PaddleBY || y == PaddleBY - 1 || y == PaddleBY + 1))
                printf("█");
            else if (x == BallX && y == BallY)
                printf("●");
            else if (x == 0 && y == 0)
                printf("┌");
            else if (x == 0 && y == 24)
                printf("└");
            else if (x == 79 && y == 0)
                printf("┐");
            else if (x == 79 && y == 24)
                printf("┘");
            else if (y == 0 || y == SIZE_HEIGHT - 1)
                printf("─");
            else if (x == 0 || x == SIZE_WIDTH - 1)
                printf("│");
            else if (x == 39)
                printf("│");
            else
                printf(" ");
        }
        printf("\n");
        Key = '-';
    }
}
