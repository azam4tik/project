#include "s21_decimal.h"
int test_bit(unsigned int value, int BitNumber) {
  return value & (1 << BitNumber);
}
void set_0_bit(unsigned int *value, int BitNumber) {
  *value &= ~(1 << BitNumber);
}
void set_1_bit(unsigned int *value, int BitNumber) {
  *value |= (1 << BitNumber);
}
void s21_unshift(s21_decimal *number) {
  int vault_1 = 0;  // Хранит значение предыдущего бита
  int vault_2 = 0;  // Сохраняет нынешний бит
  for (int i = 2; i >= 0; i--) {
    for (int j = 31; j >= 0; j--) {
      vault_2 = test_bit((number->bits[i]), j);
      if (vault_1)
        set_1_bit(&number->bits[i], j);
      else
        set_0_bit(&number->bits[i], j);
      vault_1 = vault_2;
    }
  }
}
int s21_shift(s21_decimal *number) {
  int vault_1 = 0;  // Хранит значение предыдущего бита
  int vault_2 = 0;  // Сохраняет нынешний бит
  int error = test_bit(number->bits[2], 31);
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 32; j++) {
      vault_2 = test_bit((number->bits[i]), j);
      if (vault_1)
        set_1_bit(&number->bits[i], j);
      else
        set_0_bit(&number->bits[i], j);
      vault_1 = vault_2;
    }
  }
  return error;
}

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  s21_big_decimal first = {0};
  s21_big_decimal second = {0};
  s21_big_decimal result_big = {0};
  s21_decimal_to_big_decimal(value_1, &first);
  s21_decimal_to_big_decimal(value_2, &second);
  if (get_big_scale(second) != get_big_scale(first)) {
    big_normaliz(&first, &second);
  }

  full_proverka(&result_big, result);
  return 0;
}