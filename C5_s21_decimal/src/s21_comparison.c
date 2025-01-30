
#include "s21_decimal.h"
//сравнение децималов по модулю
int s21_diff(s21_decimal val_1, s21_decimal val_2) {
  int n = -1, res = 0;
  s21_big_decimal first = {0}, second = {0};
  s21_decimal_to_big_decimal(val_1, &first);
  s21_decimal_to_big_decimal(val_2, &second);
  if (get_scale(val_1) != get_scale(val_2)) {
    big_normaliz(&first, &second);
  }
  for (int i = 191; i > n; i--) {
    if (get_bit_big(first, i) > get_bit_big(second, i)) {
      res = 1;
      n = i;
    }
    if (get_bit_big(first, i) < get_bit_big(second, i)) {
      res = -1;
      n = i;
    }
  }
  return res;
}
//определение по знакам разницы децималов
int s21_comp_log(s21_decimal val_1, s21_decimal val_2) {
  int res = 0;
  if ((s21_get_bit(val_1, 127) == 0) && (s21_get_bit(val_2, 127) == 1)) res = 1;
  if ((s21_get_bit(val_1, 127) == 1) && (s21_get_bit(val_2, 127) == 0))
    res = -1;
  if ((s21_get_bit(val_1, 127) == 0) && (s21_get_bit(val_2, 127) == 0))
    res = s21_diff(val_1, val_2);
  if ((s21_get_bit(val_1, 127) == 1) && (s21_get_bit(val_2, 127) == 1))
    res = -s21_diff(val_1, val_2);
  return res;
}

int s21_is_less(s21_decimal val_1, s21_decimal val_2) {
  int res = 0;
  if (s21_comp_log(val_1, val_2) == -1) res = 1;
  return res;
}

int s21_is_greater(s21_decimal val_1, s21_decimal val_2) {
  int res = 0;
  if (s21_comp_log(val_1, val_2) == 1) res = 1;
  return res;
}

int s21_is_equal(s21_decimal val_1, s21_decimal val_2) {
  int res = 0;
  if (s21_comp_log(val_1, val_2) == 0) res = 1;
  return res;
}

int s21_is_not_equal(s21_decimal val_1, s21_decimal val_2) {
  int res = 0;
  if (s21_comp_log(val_1, val_2) != 0) res = 1;
  return res;
}

int s21_is_less_or_equal(s21_decimal val_1, s21_decimal val_2) {
  int res = 0;
  int log = s21_comp_log(val_1, val_2);
  if (log == -1 || log == 0) res = 1;
  return res;
}

int s21_is_greater_or_equal(s21_decimal val_1, s21_decimal val_2) {
  int res = 0;
  int log = s21_comp_log(val_1, val_2);
  if (log == 1 || log == 0) res = 1;
  return res;
}
