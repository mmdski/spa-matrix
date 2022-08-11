#include <stdio.h>
#include <stdlib.h>

#include <spam.h>

int
main(void) {

  SPAMatrix a = NULL;
  spa_mat_new_arr(&a, (double[]){2, 2, 6, 4, 2, 1, 7, 6, -2, -6, -7, -1}, 3, 4);

  puts("A=");
  spa_mat_print(a);

  spa_gauss_jordan_elim(a, spa_mat_prow_exch_zero);
  puts("After Gauss-Jordan method");
  spa_mat_print(a);

  spa_mat_free(&a);

  return EXIT_SUCCESS;
}
