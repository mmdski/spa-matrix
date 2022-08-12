#include <stdio.h>
#include <stdlib.h>

#include <spam.h>

int
main(void) {
  // clang-format off
  SPAMatrix a = NULL;
  spa_mat_new_arr(&a, (double[]){
    1, 1, 2, 2, 1,
    2, 2, 4, 4, 3,
    2, 2, 4, 4, 2,
    3, 5, 8, 6, 5
  }, 4, 5);
  SPAMatrix b = NULL;
  spa_mat_new_arr(&b, (double[]){
    1,
    1,
    2,
    3
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

  // determine if the system is inconsistent by checking the last basic column
  // number in the augmented matrix
  if (basic_col_nums[n_basic_cols - 1] == 6)
    puts("The system is not consistent.");
  else
    puts("The system is consistent.");

  spa_mat_free(&a);
  spa_mat_free(&b);
  spa_mat_free(&aug);

  return EXIT_SUCCESS;
}
