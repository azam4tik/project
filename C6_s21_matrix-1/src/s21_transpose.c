#include "s21_matrix.h"

int s21_transpose(matrix_t *A, matrix_t *result) {
  int status = check_matrix(A);

  if (status == 0 && (status = s21_create_matrix(A->columns, A->rows, result)) == 0) {
    for (int m = 0; m < A->rows; ++m) {
      for (int n = 0; n < A->columns; ++n) {
        result->matrix[n][m] = A->matrix[m][n];
      }
    }
  }

  return status;
}
