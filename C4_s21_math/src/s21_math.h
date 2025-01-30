#define E 2.7182818284
#define Pi 3.1415926536
#define LN2 0.6931471806
#include <stdio.h>
#include <math.h>

int s21_abs(int x);
long double s21_acos(double x);
long double s21_asin(double x);
long double s21_atan(double x);
long double s21_ceil(double x);
long double s21_cos(double x);
long double s21_exp(double x);
long double s21_fabs(double x);
long double s21_floor(double x);
long double s21_fmod(double x, double y);
long double s21_log(double x);
long double s21_pow(double base, double exp);
long double s21_sin(double x);
long double s21_sqrt(double x);
long double s21_tan(double x);

int s21_abs (int x) {
    if (x < 0) x = -x;

    return x; 
}
/*
long double s21_acos(double x) {

}

long double s21_asin(double x) {

}

long double s21_atan(double x) {

}
*/
long double s21_ceil(double x) {
    if ((int)x != x)  x = (int)x + 1;

    return x;
}

long double s21_cos(double x) {
    int flag = 0;
    x = s21_fmod(x, 6.28318530718);
    if (x > 1.57079632679 && x < 4.71238898038) flag = 1;
    x = s21_sqrt(1.0 - s21_pow(s21_sin(x), 2));
    if (flag) x = -x;
    return x;
}

long double s21_exp(double x) {
    double y = 1;
    while( x != 0) {
        y = y * E;
        --x;
    }

    return y;
}

long double s21_fabs(double x) {
    if (x < 0.0) x = -x;

    return x; 
}

long double s21_floor(double x) {
    x = (int)x;

    return x; 
}

long double s21_fmod(double x, double y) {
    while (x >= y) {
        x = x - y; 
    }

    return x;
}

long double s21_log(double x) {
    /*x = x - 1.0;
    double sum = 0;
    int n = 1;
    double s = x;
    for(; s21_fabs(s) > 0.000000001;) {
        sum += s;
        s = s * (-1.0) * x * (double)n / (double)(n + 1) ;
        printf("%d %lf\n", n, s);
        ++n;
        if (n == 1001) break;
    }*/

    /*double a = 1, b = 4 / (x*s21_pow(2, 8)), last_a, last_b;
    while (s21_fabs(a-b) > 0.0000001) {
        last_a = a, last_b = b;
        a = (last_a + last_b) / 2;
        b = s21_sqrt(last_a * last_b);
    }
    x = Pi / ( 2 * a) - 8 * LN2;
    return x;*/
    int n = 1;
    double s = x, s1;
    s1 = 2 * (x - exp(s)) / (x + exp(s));
    for(; s21_fabs(s1) > 0.00000001;) {
        s = s + s1;
        s1 = 2 * (x - exp(s1)) / (x + exp(s1));
        printf("%d %lf %lf\n", n, s, s1);
        ++n;
    }

    return s;
}

long double s21_pow(double base, double exp) {
    double x = 1;
    if (exp < 0) {
        base = 1 / base;
        exp = -exp;
    }
    while( exp != 0) {
        x = x * base;
        --exp;
    }

    return x;
}

long double s21_sin(double x) {
    x = s21_fmod(x, 6.28318530718);
    double sum = 0;
    int n = 1;
    double s = x;
    for(; s21_fabs(s) > 0.000000001;) {
        sum += s;
        s = s * (-1) * (x*x) / ((2*n+1) * (2 * n));
        ++n;
    }

    return sum;
}

long double s21_sqrt(double x) {
    double height = 1, width = x;
    while (s21_fabs(height-width) > 0.000001) {
        height = (height + width) / 2;
        width = x / height;
    }

    return height;
}

long double s21_tan(double x) {
    x = s21_sin(x) / s21_cos(x);

    return x;
}