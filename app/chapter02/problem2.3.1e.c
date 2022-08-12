#include <stdio.h>

#include <spam.h>

int
main(void) {

  // clang-format off
  SPAMatrix a = NULL;
  spa_mat_new_arr(&a, (double[]){
    2, 1, 3, 5,
    4, 0, 4, 8,
    1, 1, 2, 3,
    0, 1, 1, 1,
  }, 4, 4);

  SPAMatrix b = NULL;
  spa_mat_new_arr(&b, (double[]){
    1,
    0,
    0,
    0
  }, 4, 1);
  // clang-format on

  SPAMatrix aug = NULL;
  spa_mat_new_colcat(&aug, a, b);
  puts("Augmented matrix:");
  spa_mat_print(aug);

  spa_gauss_elim(aug, spa_mat_prow_exch_zero);
  puts("Reduced augmented matrix:");
  spa_mat_print(aug);

  size_t basic_col_nums[5];
  size_t n_basic_cols = spa_gauss_basic_col_nos(aug, basic_col_nums);
  if (basic_col_nums[n_basic_cols - 1] == 5)
    puts("The system is not consistent.");
  else
    puts("The system is consistent.");

  return EXIT_SUCCESS;
}
