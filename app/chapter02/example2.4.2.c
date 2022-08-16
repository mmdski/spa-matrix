#include <stdio.h>
#include <stdlib.h>

#include <spam.h>

int
main(void) {

  // clang-format off
  SPAMatrix a_b = NULL;
  spa_mat_new_arr(&a_b, (double[]) {
    1, 2, 2, 0,
    2, 5, 7, 0,
    3, 6, 6, 0
  }, 3, 4);
  // clang-format on

  puts("[A|0]=");
  spa_mat_print(a_b);

  spa_gauss_jordan_elim(a_b, spa_mat_prow_exch_max);
  puts("[E_A|0]=");
  spa_mat_print(a_b);

  SPAMatrixSize a_size        = spa_mat_size(a_b);
  size_t       *basic_col_nos = malloc(a_size.n_cols * sizeof(size_t));
  size_t        n_basic_cols  = spa_gauss_basic_col_nos(a_b, basic_col_nos);

  puts("The basic column numbers of [A|0] are");
  for (size_t i = 0; i < n_basic_cols; ++i)
    printf("%zu ", basic_col_nos[i]);
  puts("");

  size_t n_free_cols = a_size.n_cols - n_basic_cols;
  printf("A has %zu free columns\n", n_free_cols);

  if (n_free_cols > 0) {

    size_t *free_col_nos = malloc(n_free_cols * sizeof(size_t));

    spa_gauss_free_col_nos(free_col_nos, n_free_cols, basic_col_nos);

    puts("The free column numbers of [A|0] are:");
    for (size_t i = 0; i < n_free_cols; ++i)
      printf("%zu ", free_col_nos[i]);
    puts("");

    SPAMatrix part_solns = NULL;
    spa_mat_new(&part_solns, a_size.n_cols - 1, n_free_cols);
    spa_gauss_part_solns(part_solns, a_b, free_col_nos);

    puts("The particular solution vectors to Ax=0 are");
    spa_mat_print(part_solns);
    spa_mat_free(&part_solns);
    free(free_col_nos);
  }

  free(basic_col_nos);

  spa_mat_free(&a_b);

  return EXIT_SUCCESS;
}
