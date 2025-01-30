#include "s21_decimal.h"

int full_provekra_without_arf(s21_big_decimal *result_big,
                              s21_decimal *result) {
  int res = not_overfloow(*result_big, result);
  int flag = 0;
  if (res != 0) {
    flag = yes_overflow(result_big);
  }
  if (flag == 3) {
    flag = from_big_decimal_delim(result_big, result);
  }
  res = flag;
  return res;
}

int from_big_decimal_delim(s21_big_decimal *result_big, s21_decimal *result) {
  int res = 0;
  int scale = get_big_scale(*result_big);
  while (scale > 0 && finder_the_older_bit(*result_big) > 95) {
    for_big_div_10(result_big);
    scale--;
    set_big_scale(result_big, scale);
  }
  if (finder_the_older_bit(*result_big) > 95 && get_sign_big(result_big) == 0) {
    res = 1;
  } else if (finder_the_older_bit(*result_big) > 95 &&
             get_sign_big(result_big) == 1) {
    res = 2;
  } else if (finder_the_older_bit(*result_big) < 96) {
    s21_big_decimal_to_decimal(*result_big, result);
  }
  return res;
}