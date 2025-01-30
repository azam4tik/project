#include "s21_decimal.h"

int full_proverka(s21_big_decimal *result_big, s21_decimal *result) {
  int res = not_overfloow(*result_big, result);
  int flag = 0;
  if (res != 0) {
    flag = yes_overflow(result_big);
  }
  if (flag == 3) {
    flag = vozmozhno_delim(result_big, result);
  }
  res = flag;
  return res;
}

int not_overfloow(s21_big_decimal result_big, s21_decimal *result) {
  int res = 0;
  if (result_big.bits[5] == 0 && result_big.bits[4] == 0 &&
      result_big.bits[3] == 0) {
    s21_big_decimal_to_decimal(result_big, result);
  } else {
    res = 1;
  }
  return res;
}

int yes_overflow(s21_big_decimal *result_big) {
  int res = 3;
  int sing = get_sign_big(result_big);
  if (get_big_scale(*result_big) == 0 &&
      finder_the_older_bit(*result_big) > 95) {
    if (sing == 1) {
      res = 2;
    } else if (sing == 0) {
      res = 1;
    }
  }
  return res;
}

int vozmozhno_delim(s21_big_decimal *result_big, s21_decimal *result) {
  int res = 0;
  int scale = get_big_scale(*result_big);
  s21_big_decimal clone_reult = *result_big;
  if (get_big_scale(*result_big) > 1) {
    while (scale > 1 && finder_the_older_bit(*result_big) > 95) {
      clone_reult = *result_big;
      for_big_div_10(result_big);
      scale--;
      set_big_scale(result_big, scale);
    }
  }
  reminder(result_big, &clone_reult);
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

void reminder(s21_big_decimal *result_big, s21_big_decimal *clone_reult) {
  int scale = get_big_scale(*result_big);
  if (finder_the_older_bit(*result_big) > 95) {
    *clone_reult = *result_big;

    for_big_div_10(result_big);
    scale--;
    set_big_scale(result_big, scale);
  }
  s21_big_decimal reminder_dec_big = {0};
  s21_decimal reminder_dec;

  big_normaliz(clone_reult, result_big);
  big_sub(clone_reult, *result_big, &reminder_dec_big);
  for_big_div_10(result_big);

  scale = get_big_scale(*result_big);
  scale--;
  set_big_scale(result_big, scale);
  s21_big_decimal_to_decimal(reminder_dec_big, &reminder_dec);

  reminder_dec.bits[3] = 0;
  int reminder;
  s21_from_decimal_to_int(reminder_dec, &reminder);
  bank_rounding(result_big, reminder);
}

void bank_rounding(s21_big_decimal *result_big, int reminder) {
  s21_big_decimal plus_one = {{1, 0, 0, 0, 0, 0, 0}};
  if (get_bit_big(*result_big, 0) == 0 && reminder > 5) {
    s21_big_add(*result_big, plus_one, result_big);
  } else if (get_bit_big(*result_big, 0) == 1 && reminder > 4) {
    s21_big_add(*result_big, plus_one, result_big);
  }
}
