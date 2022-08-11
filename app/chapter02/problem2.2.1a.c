#include <stdio.h>
#include <stdlib.h>

#include <spam.h>

int
main(void) {

  // clang-format off
  SPAMatrix a = NULL;
  spa_mat_new_arr(&a, (double[]){
    1, 2, 3, 3,
    2, 4, 6, 9,
    2, 6, 7, 6
  }, 3, 4);
  // clang-format on

  SPAMatrix e = NULL;
  spa_mat_new_copy(&e, a);

  puts("A=");
  spa_mat_print(a);

  spa_gauss_jordan_elim(e, spa_mat_prow_exch_max);
  puts("E_A=");
  spa_mat_print(e);

  return EXIT_SUCCESS;
}
