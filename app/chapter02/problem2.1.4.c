#include <stdio.h>
#include <stdlib.h>

#include <spam.h>

int
main(void) {

  // clang-format off
  SPAMatrix a = NULL;
  spa_mat_new_arr(&a, (double[]){
    0.1, 0.2, 0.3,
    0.4, 0.5, 0.6,
    0.7, 0.8, 0.901
  }, 3, 3);
  // clang-format on

  puts("A=");
  spa_mat_print(a);

  SPAMatrix e = NULL;
  spa_mat_new_like(&e, a);

  size_t basic_col_nums[3];
  size_t n_basic_cols;

  // "exact" arithmetic
  spa_mat_copy(e, a);
  spa_gauss_elim(e, spa_mat_prow_exch_max);
  n_basic_cols = spa_gauss_basic_col_nos(e, basic_col_nums);
  printf("(a) The rank of A using \"exact\" arithmetic is %zu.\n",
         n_basic_cols);

  // 3-digit, no partial pivoting
  spa_precision_set(3);
  spa_mat_copy(e, a);
  spa_gauss_elim(e, spa_mat_prow_exch_no);
  n_basic_cols = spa_gauss_basic_col_nos(e, basic_col_nums);
  printf("(b) The rank of A using 3-digit arithmetic and no pivoting is %zu.\n",
         n_basic_cols);

  // 3-digit, partial pivoting
  spa_precision_set(3);
  spa_mat_copy(e, a);
  spa_gauss_elim(e, spa_mat_prow_exch_max);
  n_basic_cols = spa_gauss_basic_col_nos(e, basic_col_nums);
  printf("(b) The rank of A using 3-digit arithmetic and pivoting is %zu.\n",
         n_basic_cols);

  spa_mat_free(&a);
  spa_mat_free(&e);

  return EXIT_SUCCESS;
}
