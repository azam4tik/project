#ifndef SRC_S21_MATRIX_H_
#define SRC_S21_MATRIX_H_

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define FAILURE 0
#define SUCCESS 1

#define OK 0
#define INCORRECT_MATRIX 1
#define CALCULATION_ERROR 2
#define MEMORY_ALLOCATION_ERROR 3

typedef struct matrix_struct {
  double **matrix;
  int rows;
  int columns;
} matrix_t;

int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result);
int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result);
int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result);
int s21_mult_number(matrix_t *A, double number, matrix_t *result);

int s21_eq_matrix(matrix_t *A, matrix_t *B);

int s21_create_matrix(int rows, int columns, matrix_t *result);
void s21_remove_matrix(matrix_t *A);

int s21_determinant(matrix_t *A, double *result);
int s21_calc_complements(matrix_t *A, matrix_t *result);

int s21_transpose(matrix_t *A, matrix_t *result);
int s21_inverse_matrix(matrix_t *A, matrix_t *result);

int check_matrix(matrix_t *A);
double сalculation_determinant(matrix_t *A, int size);
int check_2_matrix(matrix_t *A, matrix_t *B);
int matrix_addition(matrix_t *A, matrix_t *B, matrix_t *result, int sign);
void calculation_minor(double **A, double **aux, int extra_row, int extra_col, int size);

#endif  // SRC_S21_MATRIX_H_
