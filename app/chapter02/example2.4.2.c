#include <stdio.h>
#include <stdlib.h>

#include <spam.h>

int
main(void) {

  // clang-format off
  SPAMatrix a = NULL;
  spa_mat_new_arr(&a, (double[]) {
    1, 2, 2,
    2, 5, 7,
    3, 6, 6
  }, 3, 3);
  // clang-format on

  puts("A=");
  spa_mat_print(a);

  spa_gauss_jordan_elim(a, spa_mat_prow_exch_max);
  puts("E_A=");
  spa_mat_print(a);

  SPAMatrixSize a_size        = spa_mat_size(a);
  size_t       *basic_col_nos = malloc(a_size.n_cols * sizeof(size_t));
  size_t        n_basic_cols  = spa_gauss_basic_col_nos(a, basic_col_nos);

  puts("The basic column numbers of A are");
  for (size_t i = 0; i < n_basic_cols; ++i)
    printf("%zu ", basic_col_nos[i]);
  puts("");

  size_t n_free_cols = a_size.n_cols - n_basic_cols;
  printf("A has %zu free columns\n", n_free_cols);

  if (n_free_cols > 0) {

    size_t *free_col_nos = malloc(n_free_cols * sizeof(size_t));

    spa_gauss_free_col_nos(free_col_nos, n_free_cols, basic_col_nos);

    puts("The free column numbers of A are:");
    for (size_t i = 0; i < n_free_cols; ++i)
      printf("%zu ", free_col_nos[i]);
    puts("");

    SPAMatrix part_solns = NULL;
    spa_mat_new_zeros(&part_solns, a_size.n_cols, n_free_cols);
    double value;
    size_t free_col;
    for (size_t i_free_col = 0; i_free_col < n_free_cols; ++i_free_col) {
      free_col = free_col_nos[i_free_col];
      spa_mat_set(part_solns, free_col, i_free_col + 1, 1);
      for (size_t i = 1; i < free_col; ++i) {
        value = -spa_mat_get(a, i, free_col);
        spa_mat_set(part_solns, i, i_free_col + 1, value);
      }
    }

    puts("The particular solution vectors to Ax=0 are");
    spa_mat_print(part_solns);
    spa_mat_free(&part_solns);
    free(free_col_nos);
  }

  free(basic_col_nos);

  spa_mat_free(&a);

  return EXIT_SUCCESS;
}
