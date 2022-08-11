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
    2, 1, 3,  1, 0,  4, 3,
    6, 3, 4,  8, 1,  9, 5,
    0, 0, 3, -3, 0,  0, 3,
    8, 4, 2, 14, 1, 13, 3
  }, 6, 7);
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
