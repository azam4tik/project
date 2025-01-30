#include "s21_decimal.h"

void dec_to_zero(s21_decimal *decimal) {
  decimal->bits[0] = 0;
  decimal->bits[1] = 0;
  decimal->bits[2] = 0;
  decimal->bits[3] = 0;
}

double binary_to_denary(s21_decimal src, int N) {
  double sum = 0;
  for (int i = 0; i < N; i++)
    if (s21_get_bit(src, i) == 1) sum += pow(2, i);
  return sum;
}

void float_normalization(float *src, int *scale) {
  float dst = *src;
  int sc = *scale;
  while (!(int)dst || !(int)(dst / 1000000)) {
    dst *= 10;
    sc++;
  }
  dst = roundf(dst);
  while (sc && !((int)dst % 10)) {
    dst /= 10.0;
    sc--;
  }
  *src = dst;
  *scale = sc;
}

void mantissa_to_dec(s21_decimal *src, int exponent, unsigned int bits) {
  s21_set_bit(src, exponent, 1);
  for (int i = 22; i >= 0 && exponent > 0; i--) {
    exponent--;
    if ((bits & 1 << i) >> i) s21_set_bit(src, exponent, 1);
  }
}

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  int sum = 0;
  int res = s21_truncate(src, &src);
  for (int i = 32; i < 127 && res == 0; i++)
    if (s21_get_bit(src, i) == 1) res = 1;
  if (res == 0) {
    int is31bit = s21_get_bit(src, 31);
    int sign = s21_get_bit(src, 127);
    sum = binary_to_denary(src, 31);
    if (is31bit && sum) {
      res = 1;
      sum = 0;
    } else if (sign && is31bit && !sum)
      sum += pow(2, 31);
    if (sign) sum = -sum;
  }
  *dst = sum;
  return res;
}

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  int res = 0;
  dec_to_zero(dst);
  if (src < 0) {
    s21_set_bit(dst, 127, 1);
    src = -src;
  }
  for (int i = 0; i < 96; i++) {
    s21_set_bit(dst, i, src % 2);
    src /= 2;
  }
  return res;
}

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  int res = 0;
  int scale = 0;
  dec_to_zero(dst);
  if (src) {
    if (src < 0) {
      s21_set_bit(dst, 127, 1);
      src = -src;
    }
    unsigned int bits = *(int *)(void *)&src;
    int exp = ((bits << 1) >> 24) - 127;
    if (exp > 95) {
      res = 1;
    } else {
      float_normalization(&src, &scale);
      bits = *(int *)(void *)&src;
      exp = (((bits) << 1) >> 24) - 127;
      if (exp < -94 || scale > 28) res = 1;
      mantissa_to_dec(dst, exp, bits);
      if (scale) dst->bits[3] |= scale << 16;
    }
  } else
    res = 1;
  if (res) dec_to_zero(dst);
  return res;
}

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  double sum = 0;
  int res = 0;
  sum = binary_to_denary(src, 96);
  if (s21_get_bit(src, 127)) sum = -sum;
  *dst = sum / pow(10, get_scale(src));
  return res;
}
