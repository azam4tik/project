#include "../s21_matrix.h"

int s21_eq_matrix(matrix_t *A, matrix_t *B) {
  int status = s21_is_bad_eq_matrix(A, B) == 0;

  for (int m = 0; status == 1 && m < A->rows; ++m) {
    for (int n = 0; status == 1 && n < A->columns; ++n) {
      if (fabs(A->matrix[m][n] - B->matrix[m][n]) > 1e-07) status = FAILURE;
    }
  }

  return status;
}
