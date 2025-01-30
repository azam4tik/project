#include "s21_matrix.h"

int check_2_matrix(matrix_t *A, matrix_t *B) {
  int status = 1;

  if (!check_matrix(A) && !check_matrix(B)) {
    if (A->rows == B->rows && A->columns == B->columns) {
      status = 0;
    } else {
      status = 2;
    }
  }

    return status;
}

int check_matrix(matrix_t *A) {
  return (!A || !A->matrix || A->rows <= 0 || A->columns <= 0);
}