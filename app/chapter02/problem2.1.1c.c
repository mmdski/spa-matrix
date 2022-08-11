#include <stdio.h>
#include <stdlib.h>

#include <spam.h>

int
main(void) {

  // clang-format off
  SPAMatrix a = NULL;
  spa_mat_new_arr(&a, (double[]){
    2, 1, 1,  3, 0,  4, 1,
    4, 2, 4,  4, 1,  5, 5,
    6, 3, 4,  8, 1,  9, 5,
    0, 0, 3, -3, 0,  0, 3,
    8, 4, 2, 14, 1, 13, 3
  }, 5, 7);
  // clang-format on

  SPAMatrix e = NULL;
  spa_mat_new_copy(&e, a);

  puts("A=");
  spa_mat_print(a);

  spa_gauss_elim(e, spa_mat_prow_exch_max);
  puts("E=");
  spa_mat_print(e);

  size_t basic_col_nums[4];
  size_t n_basic_cols = spa_gauss_basic_col_nums(e, basic_col_nums);

  printf("The rank of A is %zu. ", n_basic_cols);

  if (n_basic_cols == 1)
    printf("The basic column number of A is %zu.\n", basic_col_nums[0]);
  else if (n_basic_cols == 2) {
    printf("The basic column numbers of A are %zu and %zu.\n",
           basic_col_nums[0],
           basic_col_nums[1]);
  } else {
    printf("The basic column numbers of A are %zu", basic_col_nums[0]);
    for (size_t i = 1; i < n_basic_cols - 1; ++i)
      printf(", %zu", basic_col_nums[i]);
    printf(", and %zu.\n", basic_col_nums[n_basic_cols - 1]);
  }

  spa_mat_free(&a);
  spa_mat_free(&e);

  return EXIT_SUCCESS;
}
