#include "s21_math.h"
#include <math.h>
#include <stdio.h>


int main() {
    double x;
    scanf("%lf", &x);
    double y1 = log(x);
    double y2 = s21_log(x);
    printf("%lf %lf", y1, y2);

    return 0;
}