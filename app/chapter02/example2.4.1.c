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
    3, 6, 8
  }, 3, 3);
  // clang-format on

  puts("A=");
  spa_mat_print(a);

  spa_gauss_elim(a, spa_mat_prow_exch_zero);
  puts("A reduced = ");
  spa_mat_print(a);

  SPAMatrixSize a_size       = spa_mat_size(a);
  size_t        n_basic_cols = spa_gauss_basic_col_nos(a, NULL);

  printf("n = %zu, rank(A) = %zu\n", a_size.n_cols, n_basic_cols);

  spa_mat_free(&a);

  return EXIT_SUCCESS;
}
