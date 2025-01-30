#include "s21_decimal.h"

void s21_set_bit(s21_decimal *src1, int index, int bit) {
  int mask = 1 << (index % 32);
  int nom_of_matisa_int = index / 32;
  if (bit) {
    src1->bits[nom_of_matisa_int] = (src1->bits[nom_of_matisa_int] | mask);
  } else {
    src1->bits[nom_of_matisa_int] = src1->bits[nom_of_matisa_int] & ~mask;
  }
}
void set_bit_big(s21_big_decimal *src1, int index, int bit) {
  int mask = 1 << (index % 32);
  int nom_of_matisa_int = index / 32;
  if (bit) {
    src1->bits[nom_of_matisa_int] = (src1->bits[nom_of_matisa_int] | mask);
  } else {
    src1->bits[nom_of_matisa_int] = src1->bits[nom_of_matisa_int] & ~mask;
  }
}

int s21_get_bit(s21_decimal src1, int index) {
  int res = 1;
  if (index > -1 && index < 129) {
    int mask = 1 << (index % 32);
    int nom_of_matisa_int = index / 32;
    int new_mask = mask & src1.bits[nom_of_matisa_int];
    if (new_mask != mask) {
      res = 0;
    }
  }
  return res;
}

int get_bit_big(s21_big_decimal brc1, int index) {
  int res = 1;
  if (index > -1 && index < 225) {
    int mask = 1 << (index % 32);
    int nom_of_matisa_int = index / 32;
    int new_mask = mask & brc1.bits[nom_of_matisa_int];
    if (new_mask != mask) res = 0;
  }
  return res;
}
/////////////////////////////////////////////////////////////////////////////
/// big_decimal [32......][32......][32........]  [32........].[32........]
int s21_decimal_to_big_decimal(s21_decimal src1, s21_big_decimal *brc1) {
  brc1->bits[0] = src1.bits[0];
  brc1->bits[1] = src1.bits[1];
  brc1->bits[2] = src1.bits[2];
  brc1->bits[3] = 0;
  brc1->bits[4] = 0;
  brc1->bits[5] = 0;
  brc1->bits[6] = src1.bits[3];
  if (brc1->bits[3] || brc1->bits[4] || brc1->bits[5]) {
    // printf("Ошибка переполнения");
    return 1;
  }
  return 0;
}

int s21_big_decimal_to_decimal(s21_big_decimal brc1, s21_decimal *src1) {
  src1->bits[0] = brc1.bits[0];
  src1->bits[1] = brc1.bits[1];
  src1->bits[2] = brc1.bits[2];
  src1->bits[3] = brc1.bits[6];
  if (brc1.bits[3] || brc1.bits[4] || brc1.bits[5]) {
    // printf("Ошибка переполнения");
    return 1;
  }
  return 0;
}

/////////////////////////////////////////////

int get_big_scale(s21_big_decimal first) {
  int mask = 127 << 16;
  int new_mask = (first.bits[6] & mask) >> 16;
  return new_mask;
}

int get_scale(s21_decimal src1) {
  int mask = 127 << 16;
  int new_mask = (src1.bits[3] & mask) >> 16;
  return new_mask;
}

void set_scale(s21_decimal *src1, int scale) {
  int znak = s21_get_bit(*src1, 127);
  scale <<= 16;
  src1->bits[3] = scale;
  s21_set_bit(src1, 127, znak);
}

void set_big_scale(s21_big_decimal *first, int scale) {
  int zn = get_bit_big(*first, 7 * 32 - 1);
  scale <<= 16;
  first->bits[6] = scale;
  set_bit_big(first, 7 * 32 - 1, zn);
}

void div_10(s21_decimal *src1) {
  s21_decimal result = {0};
  long long unsigned rem = src1->bits[2];
  long long unsigned last_degits = rem % 10;
  result.bits[2] = (rem - last_degits) / 10;
  rem = src1->bits[1];
  long long unsigned last_digits_step_2 = last_degits * pow(2, 32) + rem;
  last_digits_step_2 = last_digits_step_2 % 10;
  result.bits[1] = ((last_degits * pow(2, 32) + rem) - last_digits_step_2) / 10;
  rem = src1->bits[0];
  long long unsigned last_digits_step_3 = last_digits_step_2 * pow(2, 32) + rem;
  last_digits_step_3 = last_digits_step_3 % 10;
  result.bits[0] =
      ((last_digits_step_2 * pow(2, 32) + rem) - last_digits_step_3) / 10;
  result.bits[3] = src1->bits[3];
  if (get_scale(*src1) > 0) set_scale(&result, get_scale(*src1) - 1);
  *src1 = result;
}

void for_div_10(s21_decimal *src1) {
  src1->bits[2] = src1->bits[2] / 10;
  long long unsigned buff = src1->bits[2] % 10;  // остаток

  for (int i = 2; i >= 0; i--) {
    long long unsigned last =
        (long long unsigned)src1->bits[i] +
        (buff * pow(2, 32));  // временный младший бит чтобы не было

    buff = last % 10;
    last -= buff;
    src1->bits[i] = last / 10;
  }
  src1->bits[0] += buff / 10;
  src1->bits[3] = src1->bits[3];
}

void for_big_div_10(s21_big_decimal *src1) {
  src1->bits[5] = src1->bits[5] / 10;
  long long unsigned buff = src1->bits[5] % 10;  // остаток

  for (int i = 4; i >= 0; i--) {
    long long unsigned last =
        (long long unsigned)src1->bits[i] +
        (buff * pow(2, 32));  // временный младший бит чтобы не было

    buff = last % 10;
    last -= buff;
    src1->bits[i] = last / 10;
  }
  src1->bits[0] += buff / 10;
  src1->bits[6] = src1->bits[6];
}

void s21_big_left_shift(s21_big_decimal *src1, int x_teg) {
  s21_big_decimal res = {0};
  for (int i = 0; i < 32 * 6 - 1; i++) {
    set_bit_big(&res, i + x_teg, get_bit_big(*src1, i));
  }
  for (int i = 32 * 6; i < 32 * 7; i++) {
    set_bit_big(&res, i, get_bit_big(*src1, i));
  }
  *src1 = res;
}

void s21_big_right_shift(s21_big_decimal *src1, int x_teg) {
  s21_big_decimal res = {0};
  for (int i = 32 * 6 - 1; i > -1; i--) {
    set_bit_big(&res, i, get_bit_big(*src1, i + x_teg));
  }
  // ВАЖНО : Возможно нужно добавить  минус (- 1) на границы i ;int i = 32 * 6
  // ; i < 32 * 7 -1 <---- вот сюда .
  for (int i = 32 * 6; i < 32 * 7; i++) {
    set_bit_big(&res, i, get_bit_big(*src1, i));
  }
  *src1 = res;
}

int s21_big_diff(s21_big_decimal val_1, s21_big_decimal val_2) {
  int n = -1, res = 0;

  for (int i = 6 * 32 - 1; i > n; i--) {
    if (get_bit_big(val_1, i) > get_bit_big(val_2, i)) {
      res = 1;
      n = i;
    }
    if (get_bit_big(val_1, i) < get_bit_big(val_2, i)) {
      res = -1;
      n = i;
    }
  }
  return res;
}

int finder_the_older_bit(s21_big_decimal first) {
  int res = 6 * 32 - 1;
  while (!get_bit_big(first, res)) {
    res--;
  }
  return res;
}

int zero_decimal(s21_decimal *src1) {
  src1->bits[0] = src1->bits[1] = src1->bits[2] = src1->bits[3] = 0;
  return 0;
}

int get_sign_big(s21_big_decimal *result) {
  int sign = get_bit_big(*result, 223);
  return sign;
}
