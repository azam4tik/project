#include "s21_decimal.h"

void normaliz(s21_decimal *src1, s21_decimal *src2) {
  s21_big_decimal first = {0};
  s21_big_decimal second = {0};
  s21_decimal_to_big_decimal(*src1, &first);
  s21_decimal_to_big_decimal(*src2, &second);
  big_normaliz(&first, &second);
}

void multy_by_10(s21_big_decimal *first, int x_teg) {
  s21_big_decimal clone_min = *first;
  for (int i = 1; i < x_teg + 1; i++) {
    s21_big_left_shift(&clone_min, 1);
    s21_big_left_shift(first, 3);
    s21_big_add(*first, clone_min, first);
    clone_min = *first;
  }
}

void big_normaliz(s21_big_decimal *first, s21_big_decimal *second) {
  int first_scale = get_big_scale(*first),
      second_scale = get_big_scale(*second);
  if (first_scale > 28) {
    int should_circl = first_scale - 28;
    for (int i = 0; i < should_circl; i++) {
      for_big_div_10(first);
      set_big_scale(first, 28);
    }
  }
  if (second_scale > 28) {
    int should_circl = second_scale - 28;
    for (int i = 0; i < should_circl; i++) {
      for_big_div_10(second);
      set_big_scale(second, 28);
    }
  }
  if (get_big_scale(*second) > get_big_scale(*first)) {
    int should_circl = get_big_scale(*second) - get_big_scale(*first);
    multy_by_10(first, should_circl);
    set_big_scale(first, get_big_scale(*second));
  }
  if (get_big_scale(*second) < get_big_scale(*first)) {
    int should_circl = get_big_scale(*first) - get_big_scale(*second);
    multy_by_10(second, should_circl);
    set_big_scale(second, get_big_scale(*first));
  }
}
