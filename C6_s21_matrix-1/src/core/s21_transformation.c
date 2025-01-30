#include "../s21_matrix.h"

int s21_transpose(matrix_t *A, matrix_t *result) {
  int status = s21_is_bad_matrix(A);

  if (status == 0) {
    if ((status = s21_create_matrix(A->columns, A->rows, result)) == 0) {
      for (int m = 0; m < A->rows; ++m) {
        for (int n = 0; n < A->columns; ++n) {
          result->matrix[n][m] = A->matrix[m][n];
        }
      }
    }
  }

  return status;
}

int s21_inverse_matrix(matrix_t *A, matrix_t *result) {
  if (s21_is_bad_matrix(A) == SUCCESS) return INCORRECT_MATRIX;
  if (A->columns != A->rows) return CALCULATION_ERROR;

  double det = 0;
  int status = s21_determinant(A, &det);
  if (fabs(det) < 1e-6 || status != OK) return CALCULATION_ERROR;
  matrix_t aux = {0}, aux_transpose = {0};

  s21_calc_complements(A, &aux);
  s21_transpose(&aux, &aux_transpose);
  s21_create_matrix(A->rows, A->rows, result);

  for (int m = 0; m < A->rows; ++m) {
    for (int n = 0; n < A->rows; ++n) {
      result->matrix[m][n] = aux_transpose.matrix[m][n] / det;
    }
  }

  s21_remove_matrix(&aux_transpose);
  s21_remove_matrix(&aux);
  return OK;
}
