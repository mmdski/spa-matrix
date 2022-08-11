#include <stdio.h>
#include <stdlib.h>

#include <spam.h>

int
main(void) {

  // clang-format off
  SPAMatrix a = NULL;
  spa_mat_new_arr(&a, (double[]){
    1, 0, 0, 1,
    1, 1, 1, 3,
    1, 2, 4, 15,
    1, 3, 9, 37
  }, 4, 4);
  // clang-format on

  puts("A=");
  spa_mat_print(a);

  spa_gauss_jordan_elim(a, spa_mat_prow_exch_max);
  puts("A reduced = ");
  spa_mat_print(a);

  spa_mat_free(&a);

  return EXIT_SUCCESS;
}
