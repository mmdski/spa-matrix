#include <stdio.h>
#include <stdlib.h>

#include <spam.h>

int
main(void) {

  // clang-format off
  SPAMatrix a = NULL;
  spa_mat_new_arr(&a, (double[]){
    0.835, 0.667,
    0.333, 0.266
  }, 2, 2);
  SPAMatrix b = NULL;
  spa_mat_new_arr(&b, (double[]){
    0.168,
    0.067
  }, 2, 1);
  // clang-format on

  SPAMatrix ab = NULL;
  spa_mat_new_colcat(&ab, a, b);
  puts("A|b=");
  spa_mat_print(ab);

  size_t basic_col_nums[3];
  size_t n_basic_cols;

  SPAMatrixSize ab_size = spa_mat_size(ab);

  spa_precision_set(5);
  spa_gauss_elim(ab, spa_mat_prow_exch_max);
  puts("(a) A|b reduced (5-digit arithmetic) = ");
  spa_mat_print(ab);

  n_basic_cols = spa_gauss_basic_col_nums(ab, basic_col_nums);
  if (basic_col_nums[n_basic_cols - 1] == ab_size.n_cols)
    printf("The system is not consistent\n");
  else
    printf("The system is consistent\n");

  spa_precision_set(6);
  spa_mat_colcat(ab, a, b);
  spa_gauss_elim(ab, spa_mat_prow_exch_max);
  puts("(b) A|b reduced (6-digit arithmetic) = ");
  spa_mat_print(ab);

  n_basic_cols = spa_gauss_basic_col_nums(ab, basic_col_nums);
  if (basic_col_nums[n_basic_cols - 1] == ab_size.n_cols)
    printf("The system is not consistent\n");
  else
    printf("The system is consistent\n");

  spa_mat_free(&a);
  spa_mat_free(&b);
  spa_mat_free(&ab);

  return EXIT_SUCCESS;
}
