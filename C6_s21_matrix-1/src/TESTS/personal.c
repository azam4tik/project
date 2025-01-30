#include "../s21_matrix.h"

int main() {
  matrix_t A = {0};
  matrix_t B = {0};
  matrix_t C = {0};

  s21_create_matrix(4, 4, &A);
  A.matrix[0][0] = -1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[0][2] = 7.0;
  A.matrix[0][3] = 9.0;
  A.matrix[1][0] = 1.0;
  A.matrix[1][1] = 0.0;
  A.matrix[1][2] = 0.0;
  A.matrix[1][3] = 0.0;
  A.matrix[2][0] = 47.0;
  A.matrix[2][1] = 13.0;
  A.matrix[2][2] = 17.0;
  A.matrix[2][3] = 21.0;
  A.matrix[3][0] = 22.0;
  A.matrix[3][1] = 7.0;
  A.matrix[3][2] = 1.0;
  A.matrix[3][3] = 3.0;

  s21_inverse_matrix(&A, &B);

  s21_create_matrix(4, 4, &C);

  C.matrix[0][0] = 0.0;
  C.matrix[0][1] = 1.0;
  C.matrix[0][2] = 0.0;
  C.matrix[0][3] = 0.0;
  C.matrix[1][0] = -5.0 / 23.0;
  C.matrix[1][1] = -121.0 / 23.0;
  C.matrix[1][2] = 2.0 / 23.0;
  C.matrix[1][3] = 1.0 / 23.0;
  C.matrix[2][0] = -18.0 / 23.0;
  C.matrix[2][1] = -379.0 / 46.0;
  C.matrix[2][2] = 19.0 / 46.0;
  C.matrix[2][3] = -25.0 / 46.0;
  C.matrix[3][0] = 53.0 / 69.0;
  C.matrix[3][1] = 1061.0 / 138.0;
  C.matrix[3][2] = -47.0 / 138.0;
  C.matrix[3][3] = 19.0 / 46.0;

  s21_eq_matrix(&C, &B);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&C);

  return 0;
}
