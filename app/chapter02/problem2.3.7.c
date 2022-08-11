#include <stdio.h>
#include <stdlib.h>

#include <spam.h>

int
main(void) {

  // clang-format off
  SPAMatrix a = NULL;
  spa_mat_new_arr(&a, (double[]){
    2, 1, 1,
    3, 3, 0,
    5, 4, 1
  }, 3, 3);

  SPAMatrix b = NULL;
  spa_mat_new_arr(&b, (double[]){
    10,
     9,
    19
  }, 3, 1);
  // clang-format on

  SPAMatrix ab = NULL;
  spa_mat_new_colcat(&ab, a, b);
  puts("A|b=");
  spa_mat_print(ab);

  spa_gauss_jordan_elim(ab, spa_mat_prow_exch_max);
  puts("A|b Gauss-Jordan reduced");
  spa_mat_print(ab);

  spa_mat_free(&a);
  spa_mat_free(&b);
  spa_mat_free(&ab);

  return EXIT_SUCCESS;
}
