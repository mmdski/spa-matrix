#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <spam.h>

int
main(void) {

  // clang-format off
  SPAMatrix a_0 = NULL;
  spa_mat_new_arr(&a_0, (double[]){
    1, 2,  1, 3,  1, 0,
    2, 4, -1, 3,  8, 0,
    1, 2,  3, 5,  7, 0,
    2, 4,  2, 6,  2, 0,
    3, 6,  1, 7, -3, 0
  }, 5, 6);
  // clang-format on

  puts("[A|0]=");
  spa_mat_print(a_0);

  spa_gauss_jordan_elim(a_0, spa_mat_prow_exch_max);
  puts("E_A=");
  spa_mat_print(a_0);

  SPAMatrixSize a_0_size      = spa_mat_size(a_0);
  size_t       *basic_col_nos = malloc(a_0_size.n_cols * sizeof(size_t));
  size_t        n_basic_cols  = spa_gauss_basic_col_nos(a_0, basic_col_nos);

  puts("The basic column numbers of [A|0] are");
  for (size_t i = 0; i < n_basic_cols; ++i)
    printf("%zu ", basic_col_nos[i]);
  puts("");

  size_t n_free_cols = a_0_size.n_cols - n_basic_cols;
  printf("[A|0] has %zu free columns\n", n_free_cols);

  if (n_free_cols > 0) {

    size_t *free_col_nos = malloc(n_free_cols * sizeof(size_t));

    spa_gauss_free_col_nos(free_col_nos, n_free_cols, basic_col_nos);

    puts("The free column numbers of A are:");
    for (size_t i = 0; i < n_free_cols; ++i)
      printf("%zu ", free_col_nos[i]);
    puts("");

    SPAMatrix part_solns = NULL;
    spa_mat_new(&part_solns, a_0_size.n_cols - 1, n_free_cols);
    spa_gauss_part_solns(part_solns, a_0, free_col_nos);

    puts("The particular solution vectors to Ax=0 are");
    spa_mat_print(part_solns);
    spa_mat_free(&part_solns);
    free(free_col_nos);
  }

  free(basic_col_nos);

  spa_mat_free(&a_0);

  return EXIT_SUCCESS;
}
