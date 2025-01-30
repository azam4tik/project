#include <math.h>
#include <stdio.h>

#define width 80
#define height 25

int reading(int a[height][width]);
int death(int a[height][width]);
void draw(int a[height][width]);
void clear();
void physics(int a[height][width], int b[height][width]);
void copy(int a[height][width], int b[height][width]);
void timer(int t);

int main() {
    int array_a[height][width], array_b[height][width];
    int spead = reading(array_a);

    while (death(array_a) != 1) {
        draw(array_a);
        physics(array_a, array_b);
        copy(array_a, array_b);
        timer(spead);
    }

    draw(array_a);
    printf("Game over");
}

int reading(int a[height][width]) {
    int spead_r, number;
    scanf("%d", &spead_r);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            scanf("%d", &number);
            a[i][j] = number;
        }
    }

    return spead_r;
}

int death(int a[height][width]) {
    int br = 0, death_check = 1;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (a[i][j] == 1) {
                death_check = 0;
                br = 1;
                break;
            }
        }
        if (br == 1) break;
    }

    return death_check;
}

void draw(int a[height][width]) {
    clear();
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (a[i][j] == 1) {
                printf("○");
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }
}

void clear() { printf("\033[H\033[J"); }

void physics(int a[height][width], int b[height][width]) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int up, down, right, left;

            if (i == 24)
                up = 0;
            else
                up = i + 1;
            if (i == 0)
                down = 24;
            else
                down = i - 1;
            if (j == 0)
                left = 79;
            else
                left = j - 1;
            if (j == 79)
                right = 0;
            else
                right = j + 1;

            int sum = 0;

            if (a[up][j]) sum++;
            if (a[up][left]) sum++;
            if (a[up][right]) sum++;
            if (a[i][left]) sum++;
            if (a[i][right]) sum++;
            if (a[down][j]) sum++;
            if (a[down][left]) sum++;
            if (a[down][right]) sum++;

            if ((a[i][j] == 1 && sum == 2) || sum == 3) {
                b[i][j] = 1;
            } else
                b[i][j] = 0;
        }
    }
}

void copy(int a[height][width], int b[height][width]) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            a[i][j] = b[i][j];
        }
    }
}

void timer(int t) {
    int tik = 0;
    while (tik < pow(10, t)) {
        tik++;
    }
}
