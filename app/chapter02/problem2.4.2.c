#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <spam.h>

int
main(void) {

  // clang-format off
  SPAMatrix a = NULL;
  spa_mat_new_arr(&a, (double[]){
    1, 2,  1,
    2, 4,  1,
    1, 2, -1
  }, 3, 3);
  // clang-format on

  SPAMatrix e_a = NULL;
  spa_mat_new_copy(&e_a, a);

  puts("A=");
  spa_mat_print(a);

  spa_gauss_jordan_elim(e_a, spa_mat_prow_exch_max);
  puts("E_A=");
  spa_mat_print(e_a);

  SPAMatrixSize a_size        = spa_mat_size(a);
  size_t       *basic_col_nos = malloc(a_size.n_cols * sizeof(size_t));
  size_t        n_basic_cols  = spa_gauss_basic_col_nos(e_a, basic_col_nos);

  puts("The basic column numbers of A are");
  for (size_t i = 0; i < n_basic_cols; ++i)
    printf("%zu ", basic_col_nos[i]);
  puts("");

  size_t n_free_cols = a_size.n_cols - n_basic_cols;
  printf("A has %zu free columns\n", n_free_cols);

  size_t *free_col_nos = malloc(n_free_cols * sizeof(size_t));

  spa_gauss_free_col_nos(free_col_nos, n_free_cols, basic_col_nos);

  puts("The free column numbers of A are:");
  for (size_t i = 0; i < n_free_cols; ++i)
    printf("%zu ", free_col_nos[i]);
  puts("");

  SPAMatrix part_solns = NULL;
  spa_mat_new_zeros(&part_solns, a_size.n_cols, n_free_cols);
  spa_gauss_part_solns(part_solns, e_a, free_col_nos);

  puts("The particular solution vector to Ax=0 is");
  spa_mat_print(part_solns);

  // product of A and a particular solution
  SPAMatrix prod = NULL;
  spa_mat_new(&prod, a_size.n_rows, 1);

  double c = -0.5;
  printf("c = %f\n", c);
  puts("x=");
  SPAMatrix x_mat = NULL;
  spa_mat_new_like(&x_mat, prod);
  spa_mat_scalar_mult(x_mat, c, part_solns);
  spa_mat_print(x_mat);

  puts("Ax=");
  spa_mat_matrix_mult(prod, a, x_mat);
  spa_mat_print(prod);

  double x = spa_mat_get(x_mat, 1, 1);
  double y = spa_mat_get(x_mat, 2, 1);
  double z = spa_mat_get(x_mat, 3, 1);
  printf(
      "x=%f, y=%f, z=%f; y - xy = 2z => %f = %f\n", x, y, z, y - x * y, 2 * z);

  spa_mat_free(&prod);
  spa_mat_free(&part_solns);
  free(free_col_nos);

  free(basic_col_nos);

  spa_mat_free(&e_a);
  spa_mat_free(&a);

  return EXIT_SUCCESS;
}
