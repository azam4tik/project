#include <math.h>
#include <stdio.h>

#define RESET "\033[0m"
#define RED "\e[0;91m"
#define GREEN "\033[32m"
#define BLUE "\e[0;94m"

#define S21_TRUE 1
#define S21_FALSE 0
#define TRUE 1
#define FALSE 0
#define CONVERTERS_S21_TRUE 0
#define CONVERTERS_S21_FALSE 1
#define U_MAX_INT 4294967295          // 0b11111111111111111111111111111111
#define S21_MAX_UINT 4294967295       // 0b11111111111111111111111111111111
#define INT_MAX 2147483647            // 0b01111111111111111111111111111111
#define EXPONENT_MINUS_1 2147549184   // 0b10000000000000010000000000000000
#define EXPONENT_PLUS_1 65536         // 0b00000000000000010000000000000000
#define EXPONENT_PLUS_2 196608        // 0b00000000000000110000000000000000
#define EXPONENT_MINUS_28 2149318656  // 0b10000000000111000000000000000000
#define EXPONENT_PLUS_28 1835008      // 0b00000000000111000000000000000000
#define MINUS 2147483648              // 0b10000000000000000000000000000000
#define MAX_DECIMAL 79228162514264337593543950335.0F
#define MIN_DECIMAL -79228162514264337593543950335.0F
// #define s21_decimal infinit =

typedef struct {
  unsigned int bits[7];
} s21_big_decimal;

typedef struct {
  unsigned int bits[4];
} s21_decimal;

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
void s21_big_add(s21_big_decimal first, s21_big_decimal second,
                 s21_big_decimal *result);

int s21_get_bit(s21_decimal src1, int index);

void s21_set_bit();
int s21_decimal_to_big_decimal(s21_decimal src1, s21_big_decimal *brc1);
int get_scale(s21_decimal src1);

void printBinaryWithLeadingZeros(unsigned int number, int numBits);
void printBinary(unsigned int number);
void print_big(s21_big_decimal gg);
void print_0b(s21_decimal gg);

int get_bit_big(s21_big_decimal src1, int index);

void set_bit_big(s21_big_decimal *src1, int index, int bit);
int get_big_scale(s21_big_decimal src1);
void s21_big_left_shift(s21_big_decimal *src1, int x_teg);
void s21_big_right_shift(s21_big_decimal *src1, int x_teg);

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int big_mull(s21_big_decimal first, s21_big_decimal second,
             s21_big_decimal *result_big);

int s21_big_x(s21_big_decimal first, s21_big_decimal second,
              s21_big_decimal *result_big);
int finder_the_older_bit(s21_big_decimal first);

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
void big_sub(s21_big_decimal *first, s21_big_decimal second,
             s21_big_decimal *result_big);
int find_next_old_one(s21_big_decimal first, int i);
int get_one_for_sub(s21_big_decimal first, s21_big_decimal second,
                    s21_big_decimal *result_big, int i);

void normaliz(s21_decimal *src1, s21_decimal *src2);
void big_normaliz(s21_big_decimal *first, s21_big_decimal *second);
void multy_by_10(s21_big_decimal *first, int x_teg);
void div_10(s21_decimal *src1);
void for_div_10(s21_decimal *src1);
void for_big_div_10(s21_big_decimal *src1);

void set_big_scale(s21_big_decimal *src1, int scale);
void opredelyaem_znak_i_kto_bolshe(s21_big_decimal first,
                                   s21_big_decimal second,
                                   s21_big_decimal *result_big);
void opredelyaem_znak_sub(s21_big_decimal first, s21_big_decimal second,
                          s21_big_decimal *result_big);

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
void big_div(s21_big_decimal *first, s21_big_decimal *second);

/// Из биг децмал в обычный
int finder_the_older_bit(s21_big_decimal first);

int not_overfloow(s21_big_decimal result_big, s21_decimal *result);
int yes_overflow(s21_big_decimal *result_big);
int vozmozhno_delim(s21_big_decimal *result_big, s21_decimal *result);

int get_sign_big(s21_big_decimal *result);

int s21_big_decimal_to_decimal(s21_big_decimal brc1, s21_decimal *src1);
int full_proverka(s21_big_decimal *result_big, s21_decimal *result);
void bank_rounding(s21_big_decimal *result_big, int reminder);
void reminder(s21_big_decimal *result_big, s21_big_decimal *clone_reult);
int dec_right(s21_decimal *val, int shift);

//// Сравнение ///////////////////////////////////////////////////////////////
int s21_big_diff(s21_big_decimal val_1, s21_big_decimal val_2);
int s21_diff(s21_decimal val_1, s21_decimal val_2);
int s21_comp_log(s21_decimal val_1, s21_decimal val_2);
int s21_is_less(s21_decimal val_1, s21_decimal val_2);
int s21_is_greater(s21_decimal val_1, s21_decimal val_2);
int s21_is_equal(s21_decimal val_1, s21_decimal val_2);
int s21_is_not_equal(s21_decimal val_1, s21_decimal val_2);
int s21_is_less_or_equal(s21_decimal val_1, s21_decimal val_2);
int s21_is_greater_or_equal(s21_decimal val_1, s21_decimal val_2);
int s21_big_decimal_greater_or_equal_visout_normaliz(s21_big_decimal val_1,
                                                     s21_big_decimal val_2);

void set_scale(s21_decimal *src1, int scale);

/// Конвертация /////////////////////////////////////////////////////
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_int_to_decimal(int src, s21_decimal *dst);

int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);

void dec_to_zero(s21_decimal *decimal);
double binary_to_denary(s21_decimal src, int N);
void float_normalization(float *src, int *scale);
void mantissa_to_dec(s21_decimal *src, int exponent, unsigned int bits);
////////////////////////// Другие функции
/////////////////////////////////////////////////
int s21_floor(s21_decimal value, s21_decimal *result);
int s21_round(s21_decimal value, s21_decimal *result);
int s21_truncate(s21_decimal value, s21_decimal *result);
int s21_negate(s21_decimal value, s21_decimal *result);
int s21_my_mod2(s21_decimal one, s21_decimal *result);
int s21_my_mod10(s21_decimal one, s21_decimal *result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
/// Перевод из биг децмала в обучный для всех остальных функций кроме
/// арифметики//
int full_provekra_without_arf(s21_big_decimal *result_big, s21_decimal *result);
int from_big_decimal_delim(s21_big_decimal *result_big, s21_decimal *result);

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_shift(s21_decimal *number);
void s21_unshift(s21_decimal *number);