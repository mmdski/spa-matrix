#include <stdio.h>

#include <spam.h>

int
main(void) {

  SPAMatrix a = NULL;
  // clang-format off
  spa_mat_new_arr(&a, (double[]){
    1, 2, 2, 3, 1,
    2, 4, 4, 6, 2,
    3, 6, 6, 9, 6,
    1, 2, 4, 5, 3
  }, 4, 5);
  // clang-format on

  puts("A=");
  spa_mat_print(a);

  SPAMatrix e = NULL;
  spa_mat_new_copy(&e, a);

  spa_gauss_jordan_elim(e, spa_mat_prow_exch_max);
  puts("E_A=");
  spa_mat_print(e);

  size_t basic_col_nums[5];
  size_t n_basic_cols = spa_gauss_basic_col_nos(e, basic_col_nums);

  printf("The rank of A is %zu. The basic column numbers of A are ",
         n_basic_cols);
  printf("%zu", basic_col_nums[0]);
  for (size_t i = 1; i < n_basic_cols - 1; ++i)
    printf(", %zu", basic_col_nums[i]);
  printf(", and %zu.\n", basic_col_nums[n_basic_cols - 1]);

  spa_mat_free(&a);
  spa_mat_free(&e);
}
