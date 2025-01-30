#include "s21_decimal.h"

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int res = 1;
  s21_big_decimal first = {0};
  s21_big_decimal second = {0};
  s21_big_decimal result_big = {0};
  s21_decimal_to_big_decimal(value_1, &first);
  s21_decimal_to_big_decimal(value_2, &second);
  if (get_big_scale(second) != get_big_scale(first)) {
    big_normaliz(&first, &second);
  }

  // s21_big_add( first,  second, &result_big);

  opredelyaem_znak_i_kto_bolshe(first, second, &result_big);

  res = full_proverka(&result_big, result);
  // printf("после фулл проверки секонд ");
  // print_big(second);
  return res;
}

void s21_big_add(s21_big_decimal first, s21_big_decimal second,
                 s21_big_decimal *result_big) {
  //  print_big(first);
  //  print_big(second);
  int keep = 0;
  for (int i = 0; i < 6 * 32 - 1; i++) {
    int bit_first = get_bit_big(first, i) + get_bit_big(second, i) + keep;
    if (bit_first == 3) {
      set_bit_big(result_big, i, 1);
      keep = 1;
    } else if (bit_first == 2) {
      set_bit_big(result_big, i, 0);
      keep = 1;
    } else if (bit_first == 1) {
      set_bit_big(result_big, i, 1);
      keep = 0;
    } else {
      set_bit_big(result_big, i, 0);
      keep = 0;
    }
  }
}

void opredelyaem_znak_i_kto_bolshe(s21_big_decimal first,
                                   s21_big_decimal second,
                                   s21_big_decimal *result_big) {
  /// 5 + (- 9) - пример , когда второй элемент отрицательный + больше по модули
  /// 11000110101010
  if (s21_big_diff(first, second) == -1 && get_bit_big(second, 223) &&
      !get_bit_big(first, 223)) {
    // printf("Не должно притоваться 1\n");
    big_sub(&second, first, result_big);
    set_bit_big(result_big, 6 * 32 - 1, get_bit_big(second, 223));
    set_big_scale(result_big, get_big_scale(first));
    // -5 + 9 - пример , когда второй элемент большье , а отрицательный первый
  } else if (s21_big_diff(first, second) == -1 && get_bit_big(first, 223) &&
             !get_bit_big(second, 223)) {
    // printf("Не должно притоваться 2\n");
    big_sub(&second, first, result_big);
    set_bit_big(result_big, 7 * 32 - 1, get_bit_big(second, 223));
    set_big_scale(result_big, get_big_scale(first));
    // -8 + 7  - пример , когда первый элемент больше и он отрицательный
  } else if (s21_big_diff(first, second) && get_bit_big(first, 223) &&
             !get_bit_big(second, 223)) {
    // printf(" `BIG-add\n");
    big_sub(&first, second, result_big);
    set_bit_big(result_big, 7 * 32 - 1, 1);
    set_big_scale(result_big, get_big_scale(first));
    // 8 + (-7)  - пример, когда первый элемент больше , но второй отрицательный
  } else if (s21_big_diff(first, second) && !get_bit_big(first, 223) &&
             get_bit_big(second, 223)) {
    // printf("Не должно притоваться 3\n");
    big_sub(&first, second, result_big);
    set_bit_big(result_big, 7 * 32 - 1, get_bit_big(first, 223));
    set_big_scale(result_big, get_big_scale(first));
    // Оба отрицательных
  } else if (get_bit_big(first, 223) && get_bit_big(second, 223)) {
    // printf("Не должно притоваться 4\n");
    s21_big_add(first, second, result_big);
    set_bit_big(result_big, 7 * 32 - 1, 1);
    set_big_scale(result_big, get_big_scale(first));
    // Оба положительных
  } else if (!get_bit_big(first, 223) && !get_bit_big(second, 223)) {
    // printf("Не должно притоваться 5\n");
    s21_big_add(first, second, result_big);
    set_bit_big(result_big, 7 * 32 - 1, 0);
    set_big_scale(result_big, get_big_scale(first));
  }
}
