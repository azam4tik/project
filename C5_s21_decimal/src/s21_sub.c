#include "s21_decimal.h"

int s21_sub(s21_decimal src1, s21_decimal src2, s21_decimal *result) {
  int res = 0;
  s21_big_decimal first = {0};
  s21_big_decimal second = {0};
  s21_big_decimal result_big = {0};
  s21_decimal_to_big_decimal(src1, &first);
  s21_decimal_to_big_decimal(src2, &second);
  if (get_big_scale(second) != get_big_scale(first)) {
    big_normaliz(&first, &second);
  }

  opredelyaem_znak_sub(first, second, &result_big);
  res = full_proverka(&result_big, result);

  return res;
}

void big_sub(s21_big_decimal *first, s21_big_decimal second,
             s21_big_decimal *result_big) {
  int scale_first = get_big_scale(*first);
  first->bits[6] = 0;
  for (int i = 0; i < 6 * 32 - 1; i++) {
    if (get_bit_big(*first, i) == 1 && get_bit_big(second, i) == 1) {
      // printf("индекс = %d,  %d%d\n",i,get_bit_big(&first,
      // i),get_bit_big(&second, i));
      set_bit_big(result_big, i, 0);
    }
    if (get_bit_big(*first, i) == 1 && get_bit_big(second, i) == 0) {
      // printf("индекс = %d,  %d%d\n",i,get_bit_big(&first,
      // i),get_bit_big(&second, i));
      set_bit_big(result_big, i, 1);
    }
    if (get_bit_big(*first, i) == 0 && get_bit_big(second, i) == 0) {
      // printf("индекс = %d,  %d%d\n",i,get_bit_big(&first,
      // i),get_bit_big(&second, i));
      set_bit_big(result_big, i, 0);
    }
    if (get_bit_big(*first, i) == 0 && get_bit_big(second, i) == 1) {
      int next_old_one = find_next_old_one(*first, i);
      // printf("next_old_one =%d\n",next_old_one);
      set_bit_big(first, next_old_one, 0);
      next_old_one -= 1;
      for (int y = next_old_one; y > i - 1; y--) {
        set_bit_big(first, y, 1);
      }
      set_bit_big(result_big, i, 1);
    }
  }
  set_big_scale(first, scale_first);
}

int find_next_old_one(s21_big_decimal first, int i) {
  int x = i;
  while (x < 192 && !get_bit_big(first, x)) {
    x++;
  }
  return x;
}

void opredelyaem_znak_sub(s21_big_decimal first, s21_big_decimal second,
                          s21_big_decimal *result_big) {
  // -5 - (-9) пример, когда второй элемент больше, + оба отрицательные
  if (s21_big_diff(first, second) == -1 && get_bit_big(second, 223) &&
      get_bit_big(first, 223)) {
    big_sub(&second, first, result_big);
    set_bit_big(result_big, 7 * 32 - 1, get_bit_big(second, 223));
    set_big_scale(result_big, get_big_scale(first));
    // -5 - 9 пример , когда второй элемент больше  + положительный
  } else if (s21_big_diff(first, second) == -1 && !get_bit_big(second, 223) &&
             get_bit_big(first, 223)) {
    s21_big_add(first, second, result_big);
    set_bit_big(result_big, 7 * 32 - 1, 1);
    set_big_scale(result_big, get_big_scale(first));
    // 9 5 - пример, когда оба положительные  и второе больше
  } else if (!get_bit_big(second, 223) && !get_bit_big(first, 223) &&
             s21_big_diff(first, second) == -1) {
    //  printf("  10 минус 20");
    big_sub(&second, first, result_big);
    set_bit_big(result_big, 7 * 32 - 1, 1);
    set_big_scale(result_big, get_big_scale(first));
    // 9 - 5  пример , когда оба положительные и первое бельше
  } else if (!get_bit_big(second, 223) && !get_bit_big(first, 223) &&
             s21_big_diff(second, first) == -1) {
    big_sub(&first, second, result_big);
    set_bit_big(result_big, 7 * 32 - 1, 0);
    set_big_scale(result_big, get_big_scale(first));
    // -9 -5 пример , когда  первое больше и отрицательно , а второе
    // положительное
  } else if (s21_big_diff(first, second) == 1 && get_bit_big(first, 223) &&
             !get_bit_big(second, 223)) {
    s21_big_add(first, second, result_big);
    set_bit_big(result_big, 7 * 32 - 1, 1);
    set_big_scale(result_big, get_big_scale(first));
    // 4 - -8 когда второе отрицательное , а первое - положительное
  } else if (!get_bit_big(first, 223) && get_bit_big(second, 223)) {
    s21_big_add(first, second, result_big);
    set_bit_big(result_big, 7 * 32 - 1, 0);
    set_big_scale(result_big, get_big_scale(first));
    // 2 - 3
  } else if (s21_big_diff(second, first) == 1 && !get_bit_big(first, 223) &&
             !get_bit_big(second, 223)) {
    //  printf(" Работает 3  минус 2   \n");

    s21_big_add(first, second, result_big);
    set_bit_big(result_big, 7 * 32 - 1, 0);
    set_big_scale(result_big, get_big_scale(first));
    // -9 -  -5
  } else if (s21_big_diff(first, second) == 1 && get_bit_big(second, 223) &&
             get_bit_big(first, 223)) {
    // printf(" Работает opredelyaem_znak_sub          \n");
    // print_big(first);
    // print_big(second);
    big_sub(&first, second, result_big);
    set_bit_big(result_big, 7 * 32 - 1, 1);

    set_big_scale(result_big, get_big_scale(first));
  } else if (s21_big_diff(first, second) == 0 && !get_bit_big(second, 223) &&
             !get_bit_big(first, 223)) {
    result_big->bits[0] = 0;
    result_big->bits[1] = 0;
    result_big->bits[2] = 0;
    result_big->bits[3] = 0;
    result_big->bits[4] = 0;
    result_big->bits[5] = 0;
    result_big->bits[6] = 0;
  }
}
/*
  /// 5 + (- 9) - пример , когда второй элемент отрицательный + больше по модули
    if (s21_big_diff(first, second) == -1 && get_bit_big(&second, 223) &&
  !get_bit_big(&first, 223)) { big_sub(second, first, result_big);
    set_bit_big(result_big, 7 * 32 - 1, get_bit_big(&second, 223));
    set_big_scale(result_big,get_big_scale(first));


*/
