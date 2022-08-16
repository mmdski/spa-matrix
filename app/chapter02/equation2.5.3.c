#include <stdio.h>
#include <stdlib.h>

#include <spam.h>

int
main(void) {

  // clang-format off
  SPAMatrix a_b = NULL;
  spa_mat_new_arr(&a_b, (double[]) {
    1, 2, 2, 3, 4,
    2, 4, 1, 3, 5,
    3, 6, 1, 4, 7
  }, 3, 5);
  // clang-format on

  puts("[A|b]=");
  spa_mat_print(a_b);

  SPAMatrix e_c = NULL;
  spa_mat_new_copy(&e_c, a_b);

  spa_gauss_jordan_elim(e_c, spa_mat_prow_exch_max);
  puts("[E|c]=");
  spa_mat_print(e_c);

  SPAMatrixSize e_c_size      = spa_mat_size(e_c);
  size_t       *basic_col_nos = malloc(sizeof(size_t) * e_c_size.n_cols);
  size_t        n_basic_cols  = spa_gauss_basic_col_nos(e_c, basic_col_nos);
  size_t        n_free_cols   = e_c_size.n_cols - n_basic_cols;
  size_t       *free_col_nos  = malloc(sizeof(size_t) * n_free_cols);
  spa_gauss_free_col_nos(free_col_nos, n_free_cols, basic_col_nos);

  SPAMatrix part_solns = NULL;
  spa_mat_new(&part_solns, e_c_size.n_cols - 1, n_free_cols);
  spa_gauss_part_solns(part_solns, e_c, free_col_nos);

  puts("The general solution columns are");
  spa_mat_print(part_solns);

  spa_mat_free(&part_solns);
  free(free_col_nos);
  free(basic_col_nos);
  spa_mat_free(&e_c);
  spa_mat_free(&a_b);

  return EXIT_SUCCESS;
}
