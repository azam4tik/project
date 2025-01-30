#include "s21_matrix.h"

void calculation_minor(double **A, double **С, int extra_row, int extra_col, int size) {
  for (int row = 0, m = 0; row < size; ++row) {
    for (int col = 0, n = 0; col < size; ++col) {
      if (row != extra_row && col != extra_col) {
        С[m][n++] = A[row][col];
        if (n == size - 1) {
          n = 0;
          ++m;
        }
      }
    }
  }
}

double сalculation_determinant(matrix_t *A, int size) {
  if (size == 1) return A->matrix[0][0];

  matrix_t С = {0};
  double result = 0;

  s21_create_matrix(size, size, &С);
  for (int m = 0, sign = 1; m < size; sign *= (-1), ++m) {
    calculation_minor(A->matrix, С.matrix, 0, m, size);
    result += sign * A->matrix[0][m] * сalculation_determinant(&С, size - 1);
  }

  s21_remove_matrix(&С);
  return result;
}

int s21_determinant(matrix_t *A, double *result) {
  if (check_matrix(A) == SUCCESS) return INCORRECT_MATRIX;
  if (A->columns != A->rows) return CALCULATION_ERROR;

  *result = сalculation_determinant(A, A->rows);

  return OK;
}

int s21_calc_complements(matrix_t *A, matrix_t *result) {
  if (check_matrix(A) == SUCCESS) return INCORRECT_MATRIX;
  if (A->columns != A->rows) return CALCULATION_ERROR;

  s21_create_matrix(A->columns, A->rows, result);
  if (A->rows != 1) {
    matrix_t С = {0};

    s21_create_matrix(A->rows, A->rows, &С);
    for (int sign = 0, m = 0; m < A->rows; ++m) {
      for (int n = 0; n < A->columns; ++n) {
        calculation_minor(A->matrix, С.matrix, m, n, A->rows);
        sign = ((m + n) % 2 == 0) ? 1 : (-1);
        result->matrix[m][n] = sign * сalculation_determinant(&С, A->rows - 1);
      }
    }

    s21_remove_matrix(&С);
  } else {
    result->matrix[0][0] = 1;
  }

  return OK;
}

int s21_inverse_matrix(matrix_t *A, matrix_t *result) {
  if (check_matrix(A) == SUCCESS) return INCORRECT_MATRIX;
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
