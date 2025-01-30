#include "s21_matrix.h"

int s21_create_matrix(int rows, int columns, matrix_t *result) {
  int status = INCORRECT_MATRIX;

  if (rows > 0 && columns > 0 && (result->matrix = (double **)calloc(rows, sizeof(double *)))) {
    result->rows = rows;
    result->columns = columns;

    for (int m = 0; m < rows; ++m) {
      if (!(result->matrix[m] = (double *)calloc(columns, sizeof(double)))) {
        s21_remove_matrix(result);
        status = MEMORY_ALLOCATION_ERROR;
      }
    }

    if (status == INCORRECT_MATRIX) {
      for (int m = 0; m < rows; ++m) {
        for (int n = 0; n < columns; ++n) {
          result->matrix[m][n] = 0;
        }
      }
      status = OK;
    }
  } else if (rows > 0 && columns > 0) status = MEMORY_ALLOCATION_ERROR;

  return status;
}

void s21_remove_matrix(matrix_t *A) {
  if (A) {
    for (int m = 0; m < A->rows; ++m) free(A->matrix[m]);
    free(A->matrix);
    A->matrix = NULL;
    A->rows = 0;
    A->columns = 0;
  }
}