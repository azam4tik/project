#include "../s21_matrix.h"

int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  return matrix_addition(A, B, result, 1);
}

int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  return matrix_addition(A, B, result, 2);
}

int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int status = (s21_is_bad_matrix(A) || s21_is_bad_matrix(B));

  if (status == 0 && (status = A->columns != B->rows ? 2 : 0) == 0) {
    if ((status = s21_create_matrix(A->rows, B->columns, result)) == 0) {
      for (int m = 0; m < A->rows; ++m) {
        for (int n = 0; n < B->columns; ++n) {
          for (int z = 0; z < B->rows; z += 1)
            result->matrix[m][n] += A->matrix[m][z] * B->matrix[z][n];
        }
      }
    }
  }

  return status;
}

int s21_mult_number(matrix_t *A, double number, matrix_t *result) {
  int status = s21_is_bad_matrix(A);

  if (status == 0) {
    if ((status = s21_create_matrix(A->rows, A->columns, result)) == 0) {
      for (int m = 0; m < A->rows; ++m) {
        for (int n = 0; n < A->columns; ++n)
          result->matrix[m][n] = A->matrix[m][n] * number;
      }
    }
  }

  return status;
}

int matrix_addition(matrix_t *A, matrix_t *B, matrix_t *result, int sign) {
  int status = s21_is_bad_eq_matrix(A, B);

  if (status == 0) {
    if ((status = s21_create_matrix(A->rows, A->columns, result)) == 0) {
      for (int m = 0; m < A->rows; ++m) {
        for (int n = 0; n < A->columns; ++n) {
          if (sign == 1)
            result->matrix[m][n] = A->matrix[m][n] + B->matrix[m][n];
          else if (sign == 2)
            result->matrix[m][n] = A->matrix[m][n] - B->matrix[m][n];
        }
      }
    }
  }

  return status;
}
