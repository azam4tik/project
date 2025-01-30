#include <math.h>
#include <stdio.h>

#include "s21_decimal.h"

//Округляет указанное Decimal число до ближайшего целого числа в сторону
//отрицательной бесконечности.
int s21_floor(s21_decimal value, s21_decimal *result) {
  int res = 0;
  if (result == NULL) res = 1;
  if (get_scale(value) > 0 && res == 0) {  //сразу проверяем дробное ли число
    s21_decimal big_decimal;
    big_decimal.bits[0] = 4294967295;
    big_decimal.bits[1] = 4294967295;
    big_decimal.bits[2] = 4294967295;
    big_decimal.bits[3] = 0;
    int sign = s21_get_bit(value, 127);  //получаем знак числа + || -
    if (sign == 1) {  //если минус то в конце надо увеличить на 1, если нет, то
                      //просто округляем число как в функции truncate
      *result = value;
      // set_scale(result, 0);
      s21_set_bit(result, 127, 0);  // делаем число положительным и делим на 10
                                    // пока не дойдем до целой части
      for (int i = 0; i < get_scale(value); i++) {
        div_10(result);
      }
      s21_decimal number_1 = {{1, 0, 0, 0}};
      if (s21_is_greater_or_equal(*result,
                                  big_decimal) ==
          1)  //проверяем число не равно ли крайнему значению,
              //если равно, то прибавлять нельзя и выдаем ошибку
        res = 1;
      else
        s21_add(*result, number_1, result);
      s21_set_bit(result, 127, 1);  // делаем число отрицательным
    } else {
      *result = value;
      //можно исползовать truncate
      for (int i = 0; i < get_scale(value); i++) {
        div_10(result);
      }
      // set_scale(result, 0);
    }
  } else if (res == 0)
    *result = value;

  return res;
}
// функция остатка от числа
int s21_my_mod10(s21_decimal one, s21_decimal *result) {
  int res = 0;
  s21_decimal ten = {{10, 0, 0, 0}};
  set_scale(&one, 0);
  s21_set_bit(&one, 127, 0);
  s21_decimal copy_one = one;
  div_10(&copy_one);
  *result = copy_one;
  s21_mul(*result, ten, result);
  s21_sub(one, *result, &copy_one);
  *result = copy_one;
  return res;
}

int s21_my_mod2(s21_decimal one, s21_decimal *result) {
  int res = 0;
  s21_set_bit(&one, 127, 0);
  set_scale(&one, 0);
  s21_decimal copy_one = one;
  s21_big_decimal temp;
  s21_decimal_to_big_decimal(copy_one, &temp);
  s21_big_right_shift(&temp, 1);
  s21_big_left_shift(&temp, 1);
  res = full_provekra_without_arf(&temp, &copy_one);
  *result = copy_one;
  s21_sub(one, *result, &copy_one);
  *result = copy_one;
  return res;
}

//Округляет Decimal до ближайшего целого числа. (Банковское округление)
int s21_round(s21_decimal value, s21_decimal *result) {
  int res = 0;
  s21_decimal number_0 = {{0, 0, 0, 0}};
  if (result == NULL) res = 1;
  if (get_scale(value) > 0 && res == 0) {
    s21_decimal number_5 = {{5, 0, 0, 0}}, number_1 = {{1, 0, 0, 0}},
                big_decimal;  // если получится, то вынести эти числа за пределы
                              // функции или сделать define
    big_decimal.bits[0] = 4294967295;
    big_decimal.bits[1] = 4294967295;
    big_decimal.bits[2] = 4294967295;
    big_decimal.bits[3] = 0;
    *result = value;
    s21_set_bit(result, 127, 0);
    for (int i = 0; i < get_scale(value) - 1; i++) {  //округляем до десятых
      div_10(result);
    }
    s21_decimal copy = *result;
    s21_my_mod10(copy, &copy);
    if (s21_diff(copy, number_5) ==
        0) {  //проверяем не равно ли 5 для банковского округления
      div_10(result);
      s21_my_mod2(*result, &copy);
      if (s21_diff(copy, number_1) == 0) {
        if (s21_is_greater_or_equal(*result, big_decimal) == 1)
          res = 1;
        else {
          if (s21_diff(*result, number_0) == 0) s21_set_bit(result, 127, 0);
          s21_add(*result, number_1, result);
        }
      }
    } else if (s21_diff(copy, number_5) == 1) {  //округляем в большую сторону
      div_10(result);
      if (s21_is_greater_or_equal(*result, big_decimal) == 1) {
        res = 1;
      } else {
        if (s21_diff(*result, number_0) == 0) s21_set_bit(result, 127, 0);
        s21_add(*result, number_1, result);
      }
    } else {  //округляем в меньшую сторону
      div_10(result);
    }
  } else if (res == 0) {
    *result = value;
  }
  if (res == 0 && s21_diff(*result, number_0) != 0)
    s21_set_bit(result, 127, s21_get_bit(value, 127));
  return res;
}

//Возвращает результат умножения указанного Decimal на -1. Просто меняем 127 бит
//на противоположный
int s21_negate(s21_decimal value, s21_decimal *result) {
  int res = 0;
  if (result == NULL)
    res = 1;
  else {
    *result = value;
    int sign = s21_get_bit(*result, 127);
    if (sign == 1)
      sign = 0;
    else
      sign = 1;
    s21_set_bit(result, 127, sign);
  }
  return res;
}

// Возвращает целые цифры указанного Decimal числа; любые дробные цифры
// отбрасываются, включая конечные нули.
int s21_truncate(s21_decimal value, s21_decimal *result) {
  int res = 0;
  if (result == NULL) res = 1;
  for (int i = 96; i < 112 && !res; i++)
    if (s21_get_bit(value, i)) res = 1;
  for (int i = 118; i < 127 && !res; i++)
    if (s21_get_bit(value, i)) res = 1;
  if (get_scale(value) > 0 && res == 0) {
    *result = value;
    for (int i = 0; i < get_scale(value); i++) {  //округляем до целого
      div_10(result);
    }
    for (int i = 112; i < 120;
         i++) {  // обнуляем степень, чтобы не было дробной части
      s21_set_bit(result, i, 0);
    }
  } else if (res == 0)
    *result = value;
  return res;
}
