#include <stdio.h>
#include <stdlib.h>

#include <spam.h>

int
main(void) {

  // clang-format off
  SPAMatrix a = NULL;
  spa_mat_new_arr(&a, (double[]) {
    1, 2, 2, 3,
    2, 4, 1, 3,
    3, 6, 1, 4
  }, 3, 4);
  // clang-format on

  puts("A=");
  spa_mat_print(a);

  spa_gauss_jordan_elim(a, spa_mat_prow_exch_max);
  puts("E_A=");
  spa_mat_print(a);

  spa_mat_free(&a);

  return EXIT_SUCCESS;
}
