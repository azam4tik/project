#include "s21_decimal.h"

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int res = 0;
  s21_big_decimal first = {0};
  s21_big_decimal second = {0};
  s21_big_decimal result_big = {0};
  s21_decimal_to_big_decimal(value_1, &first);
  s21_decimal_to_big_decimal(value_2, &second);
  int sign_1 = s21_get_bit(value_1, 127);
  int sign_2 = s21_get_bit(value_2, 127);
  int sign = 0;
  if (sign_1 == 1 && sign_2 == 1)
    sign = 0;
  else if (sign_1 == 1 || sign_2 == 1)
    sign = 1;
  int scale_1 = get_scale(value_1);
  int scale_2 = get_scale(value_2);
  int scale = scale_1 + scale_2;
  s21_big_decimal clone_second = second;
  for (int i = 0; i < 32 * 6; i++) {
    if (get_bit_big(first, i) == 1) {
      s21_big_left_shift(&clone_second, i);
      s21_big_add(clone_second, result_big, &result_big);
      clone_second = second;
    }
  }
  res = full_proverka(&result_big, result);
  s21_set_bit(result, 127, sign);
  set_scale(result, scale);
  return res;
}